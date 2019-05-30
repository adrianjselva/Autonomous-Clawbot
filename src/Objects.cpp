#include "Objects.h"

int Objects::lowerVisionBoundX() {
  return static_cast<int>((round((2.2564 * averagedX) - 379.0852)));          // Calculate linear equation for line that represents lower bound of acceptable values
}

int Objects::upperVisionBoundX() {
  return static_cast<int>((round((3.0345 * averagedX) - 509.793)));           // Calculate linear equation for line that represents upper bound of acceptable values      
}

int Objects::middleVisionBoundX() {
  return round((static_cast<double>(averagedY) + 441.2344) / 2.6154);         // Calculate linear equation for line that represents middle of acceptable values
}

void Objects::setBoundVision() {
  if((averagedY < upperVisionBoundX()) && (averagedY > lowerVisionBoundX())) {          // If value is in between upper and lower values, set equal to true
    isInBoundVision = true;
  } else {
    isInBoundVision = false;
  }
}

void Objects::setBoundDistance() { 
  if((averageDistanceToObject < DISTANCE_UPPER) && (averageDistanceToObject > DISTANCE_LOWER)) {          // If distance is between range of acceptable values, set equal to true
    isInBoundDistance = true;
  } else {
    isInBoundDistance = false;
  }
}

void Objects::setConditions() {
  setBoundVision();
  setBoundDistance();

  (isInBoundDistance && isInBoundVision) ? isBoundVisionDistance = true
                                         : isBoundVisionDistance = false;           // If both booleans return true, return true. Otherwise, return false
}

void Objects::setPower() {
  setPowerX();
  setPowerY();
}

void Objects::setPowerX() {
  if(doesObjectExist) {
    double errorX = static_cast<double>(averagedX - middleVisionBoundX());          // Set error equal to x-value minus the middle x value
    integralX = integralX + std::abs(errorX);                                       // Keep adding to integral
    if(errorX == 0) {                                                               // Reset integral
      integralX = 0;
    } else if(std::abs(errorX) > 5) {                                               // Integral is not useful with large errors
      integralX = 0;
    }
    previousErrorX = errorX;                                                        // Storing previous error
    double derivativeX = errorX - previousErrorX;                                   // Calculating derivative by subtracting error from previous error
    if(errorX < 0) {                                                                // Make integral negative if error is negative
      integralX = -1 * integralX;
    }
    powerX = (errorX * kP) + (integralX * kI) + (derivativeX * kD);                 // Assigning value to powerX using PID equation
  } else {
    powerX = 0;
    integralX = 0;
    previousErrorX = 0;
  }
}

void Objects::setPowerY() {
  if (isInBoundVision && doesObjectExist) {
    double errorY = (averageDistanceToObject - DISTANCE_TARGET);           // Set error equal to distance sensor value minus the desired distance of 8 inches
    previousErrorY = errorY;                                               // Storing previous error
    integralY = integralY + std::abs(errorY);                              // Keep adding to integral
    if(errorY == 0) {                                                               // Reset integral
      integralY = 0;
    } else if(std::abs(errorY) > 5) {                                               // Integral is not useful with large errors
      integralY = 0;
    }

    if(errorY < 0) {                                                                // Make integral negative if error is negative
      integralY = -1 * integralY;
    }
    double derivativeY = errorY - previousErrorY;                          // Calculating derivative by subtracting error from previous error

    if ((errorY < .01) && (errorY > -.01)) {
      powerY = 0;
    } else if (errorY > 15) {
      powerY = 0;
    } else {
      powerY = (errorY * kPY) + (integralY * kIY) + (derivativeY * kDY);
    }
  } else {
    powerY = 0;
  }
}

double Objects::averageArray(int a[]) {          // Function that returns an average of all the values in an array
  int sum = 0;

  for (int z = 0; z < 10; z++) {                 // Loops through each value in array and adds each value to the variable "sum"
    sum += a[z];
  }

  return static_cast<double>(sum / 10);          // Returns the sum divided by the number of elements
}

double Objects::averageArray(double a[]) {       // Same as above but takes double array as argument
  double sum = 0;

  for (int z = 0; z < 10; z++) {
    sum += a[z];
  }

  return (sum / 10.0);
}

void Objects::setValues(int id, int height, int width, int originX, int originY) {
  averagedX = static_cast<int>(round(averageArray(xPositionArray)));              // Sets each member variable to an averaged value of each array
  averagedY = static_cast<int>(round(averageArray(yPositionArray)));
  averagedWidth = static_cast<int>(round(averageArray(widthArray)));
  averagedHeight = static_cast<int>(round(averageArray(heightArray)));           
  averageDistanceToObject = round(averageArray(distanceArray));
  idZ = id;                              // Sets object variables to those passed by updateValues()
  area = width * height;
  this->originX = originX;
  this->originY = originY;
}

void Objects::updateArrays(int visionX, int visionY, int height, int width, double distanceTo) {
  xPositionArray[arrayLoopCounter] = visionX;            // arrayLoopCounter functions as a common index between all of the different arrays and allows each one to get updated simultaneously
  yPositionArray[arrayLoopCounter] = visionY;           
  heightArray[arrayLoopCounter] = height;
  widthArray[arrayLoopCounter] = width;
  distanceArray[arrayLoopCounter] = distanceTo;

  arrayLoopCounter++;                                   // arrayLoopCounter is incremented every cycle

  if (arrayLoopCounter >= 10) {
    arrayLoopCounter = 0;              // If arrayLoopCounter reaches the maximum value of samples to be taken, reset to 0
  }

  if (sufficientDataCounter >= 10) {
    isSufficientData = true;           // There need to be at least 10 values taken in order to have sufficient data for calculations
  } else {
    sufficientDataCounter++;
  }
}

void Objects::setExistence() {                              // This function determines whether or not an object detected by the vision sensor is real or not
  if ((area < 100) && isSufficientData) {                   // If the vision loses track of the object and all properties revert to 0, the object does not exist
    doesObjectExist = false;
    setDefaultValues();
  } else if ((area >= 100) && !isSufficientData) {          // If a valid object is detected, but there isn't sufficient data, the object does not exist
    doesObjectExist = false;
  } else if ((area >= 100) && isSufficientData) {           // If a valid object is detected with sufficient data, the object does exist
    doesObjectExist = true;
  } else {
    doesObjectExist = false;
  }
}

void Objects::setObjectType() {           // Determine the type of object based on the color id signature that is passed to the class
  switch(idZ) {
    case 1:
      objectType = YELLOW_OBJECT;
      objectColor.rgb(255, 255, 0);      // Sets the color of the object in (r, g, b) format
      break;
    case 2:
      objectType = RED_OBJECT;
      objectColor.rgb(255, 0, 0);
      break;
    case 10:
      objectType = RED_YELLOW_OBJECT;
      objectColor.rgb(0, 0, 255);
      break;
    default: 
      objectType = UNKNOWN_OBJECT;
      objectColor.rgb(0, 255, 0);
      break;
  }
}

void Objects::updateValues(int visionX, int visionY, int originX, int originY, double distanceTo, int id, int height, int width) {
  this->updateArrays(visionX, visionY, height, width, distanceTo);         // Function that calls all of the smaller functions
  this->setValues(id, height, width, originX, originY);
  this->setObjectType();
  this->setConditions();
  this->setExistence();
  this->setPower();
}

void Objects::setDefaultValues() {          // This function sets all the values back to default values. It is called in the constructor as an initialization for variables as well.
  doesObjectExist = false;
  isInBoundVision = false;
  isInBoundDistance = false;
  isBoundVisionDistance = false;
  isSufficientData = false;

  averageDistanceToObject = 0;
  powerX = 0;
  powerY = 0;

  averagedX = 0;
  averagedY = 0;
  objectType = 0;
  idZ = 0;
  area = 0;
  averagedHeight = 0;
  averagedWidth = 0;
  originX = 0;
  originY = 0;

  previousErrorX = 0;
  previousErrorY = 0;
  integralX = 0;
  integralY = 0;

  arrayLoopCounter = 0;
  sufficientDataCounter = 0;
}

Objects::Objects() : screen(nullptr) {           // Constructor
  setDefaultValues();
}

Objects::Objects(vex::brain::lcd *brainD) : screen(brainD) {           // Overloaded constructor that takes a reference to the Screen object as a parameter
  setDefaultValues();
}

void Objects::printObject(int VIEW) {            // This function prints the object to the screen and differs depending on the current view of the screen
  if(doesObjectExist) {
    switch(VIEW){
      case ACTION_V:
        screen->drawCircle(averagedX + LEFT_BOUND_OFFSETTED, 25, 2, objectColor);           // Prints object as a colored dot that moves only in the x direction
        break;
      case VISION_V:                                                                        // Prints the object as a colored rectangular blob with black circle in the center
        screen->setPenColor(objectColor);
        screen->printAt(originX + LEFT_BOUND_OFFSETTED, originY + averagedHeight + 10, true, "X: %d, Y:%d BoundX:%d", averagedX, averagedY, isInBoundVision);
        screen->drawRectangle(originX + LEFT_BOUND_OFFSETTED, originY, averagedWidth, averagedHeight, objectColor);
        screen->drawCircle(averagedX + LEFT_BOUND_OFFSETTED, averagedY, CIRCLE_RADIUS, vex::color::black);
        break;
    }
  }
}