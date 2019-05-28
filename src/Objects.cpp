#include "Objects.h"

void Objects::setBoundVision() {
  if((averagedY < upperVisionBoundX()) && (averagedY > lowerVisionBoundX())) {
    isInBoundVision = true;
  } else {
    isInBoundDistance = false;
  }
}

int Objects::lowerVisionBoundX() {
  return static_cast<int>((round((2.2564 * averagedX) - 379.0852)));
}

int Objects::upperVisionBoundX() {
  return static_cast<int>((round((3.0345 * averagedX) - 509.793)));
}

int Objects::middleVisionBoundX() {
  return round((static_cast<double>(averagedY) + 441.2344) / 2.6154);
}

void Objects::setBoundDistance() {
  if((averageDistanceToObject < 8.2) && (averageDistanceToObject > 7.8)) {
    isInBoundDistance = true;
  } else {
    isInBoundDistance = false;
  }
}

void Objects::setScreen(vex::brain::lcd& t) {
  screen = t;
}

void Objects::setConditions() {
  setBoundVision();
  setBoundDistance();

  (isInBoundDistance && isInBoundVision) ? isBoundVisionDistance = true
                                         : isBoundVisionDistance = false;
}

void Objects::setPower() {
  setPowerX();
  setPowerY();
}

void Objects::setPowerX() {
  double errorX = static_cast<double>(averagedX - middleVisionBoundX());
  double previousError = errorX;
  double derivativeX = errorX - previousError;
  powerX = (errorX * kP) + (derivativeX * kD);
}

void Objects::setPowerY() {
  if (isInBoundVision) {
    double errorY = (averageDistanceToObject - 8.0);
    double previousErrorY = errorY;
    double derivativeY = errorY - previousErrorY;

    if ((errorY < .01) && (errorY > -.01)) {
      powerY = 0;
    } else if (errorY > 15) {
      powerY = 0;
    } else {
      powerY = (errorY * kPY) + (derivativeY * kDY);
    }
  } else {
    powerY = 0;
  }
}

double Objects::averageArray(int a[]) {
  int sum = 0;

  for (int z = 0; z < 10; z++) {
    sum += a[z];
  }

  return static_cast<double>(sum / 10);
}

double Objects::averageArray(double a[]) {
  double sum = 0;

  for (int z = 0; z < 10; z++) {
    sum += a[z];
  }

  return (sum / 10.0);
}

void Objects::setValues(int id, int height, int width, int originX, int originY) {
  averagedX = static_cast<int>(round(averageArray(xPositionArray)));
  averagedY = static_cast<int>(round(averageArray(yPositionArray)));
  averagedWidth = static_cast<int>(round(averageArray(widthArray)));
  averagedHeight = static_cast<int>(round(averageArray(heightArray)));
  averageDistanceToObject = round(averageArray(distanceArray));
  idZ = id;
  this->area = width * height;
  this->originX = originX;
  this->originY = originY;
}

void Objects::updateArrays(int visionX, int visionY, int height, int width, double distanceTo) {
  xPositionArray[arrayLoopCounter] = visionX;
  yPositionArray[arrayLoopCounter] = visionY;
  heightArray[arrayLoopCounter] = height;
  widthArray[arrayLoopCounter] = width;
  distanceArray[arrayLoopCounter] = distanceTo;

  arrayLoopCounter++;

  if (arrayLoopCounter >= 10) {
    arrayLoopCounter = 0;
  }

  if (sufficientDataCounter <= 10) {
    sufficientDataCounter++;
  } else {
    isSufficientData = true;
  }
}

void Objects::setExistence() {
  if ((area == 0) && isSufficientData) {
    doesObjectExist = false;
    setDefaultValues();
  } else if ((area > 0) && !isSufficientData) {
    doesObjectExist = false;
  } else if ((area > 0) && isSufficientData) {
    doesObjectExist = true;
  }
}

void Objects::setObjectType() {
  switch(idZ) {
    case 1:
      objectType = YELLOW_OBJECT;
      objectColor.rgb(255, 255, 0);
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
    this->updateArrays(visionX, visionY, height, width, distanceTo);
    this->setValues(id, height, width, originX, originY);
    this->setObjectType();
    this->setConditions();
    this->setExistence();
    if(doesObjectExist) {
      this->setPower();
    }
}

void Objects::setDefaultValues() {
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

  arrayLoopCounter = 0;
  sufficientDataCounter = 0;
}

Objects::Objects() {
  setDefaultValues();
}

Objects::Objects(vex::brain::lcd &brainD) {
  screen = brainD;
  setDefaultValues();
}

void Objects::printObject(int VIEW) {
  if(doesObjectExist) {
    switch(VIEW){
      case 0:
        screen.drawCircle(averagedX + xOffset, 25, 2, objectColor);
        break;
      case 1:
        screen.setPenColor(objectColor);
        screen.printAt(originX + xOffset, originY + averagedHeight + 10, true, "X: %d, Y:%d BoundX:%d", averagedX, averagedY, isInBoundVision);
        screen.drawRectangle(originX + xOffset, originY, averagedWidth, averagedHeight, objectColor);
        screen.drawCircle(averagedX + xOffset, averagedY, 2, vex::color::black);
        break;
    }
  }
}