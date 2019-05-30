#include "ActionManager.h"
#include "ObjectManager.h"
#include "constants.h"
#include "robot-config.h"
#include "visionYellow.h"

/** Function that draws the screen map **/
void drawActionManagerView();
void drawVisionMapView();
void checkActionButton();
void checkVisionButton();

/** Thread functions **/
void printInfo();
void dataThread();
void actionThread();

void driveBac(double inches);

bool isObjectReadyToGrab();
void drawLines();

int CURRENT_VIEW;

vex::thread dataThreadObject;
vex::thread actionThreadObject;
vex::thread printThreadObject;

/** Hierarchical functions **/
void init();
void exit();

// Robot dimensions
ObjectManager objectManager(Brain.Screen);
ActionManager actionManager(RightMotor, LeftMotor, ClawMotor, ArmMotor,
                            Brain.Screen);

int main() {
  init();                                                   // Function to set some initial values

  dataThreadObject = vex::thread (dataThread);              // Initialize thread objects dataThreadObject, actionThreadObject, and printThreadObject
  actionThreadObject = vex::thread (actionThread);          // dataThread, actionThread, and printInfo are the functions executed by each thread object
  printThreadObject = vex::thread (printInfo);

  dataThreadObject.join();                                  // join() makes each thread wait until the others are completed
  actionThreadObject.join();                                // Unsure if necessary
  printThreadObject.join();

  return 0;
}

void init() {
  Brain.Screen.render(true, false);                         // Disable automatic rendering, prevents flickering
  CURRENT_VIEW = ACTION_V;                                  // Sets current screen view to ActionManager view
  actionManager.initialize();                               // Moves robot arm to ARM_UP position and begins searching for objects
}

void dataThread() {
  while (1) {                         
    Vision1.takeSnapshot(0);                                                                                                 // Updates Vision1.objects[] array with all objects detected by Vision sensor
    objectManager.update(Vision1.objects, Sonar.distance(vex::distanceUnits::in), actionManager.TARGET_OBJECT_TYPE);         // Calls update() function of ObjectManager and passes Vision1.objects[] array, distance recorded from ultrasound sensor, and the type of object to search for
    
    vex::this_thread::sleep_for(25);                        // Sleep for 25 ms
  }
}

void printInfo() {
  while (1) {
    switch (CURRENT_VIEW) {
    case ACTION_V:
      drawActionManagerView();                      // Draws the ActionManagerView which displays the state of the robot
      actionManager.printStatus();                  // Prints information from the ActionManager class about the state of the robot
      objectManager.printObjects(ACTION_V);         // Draws the objects to the display in ACTION_V form
      break;
    case VISION_V:
      drawVisionMapView();                          // Draws a visual representation of the Vision sensor data
      objectManager.printObjects(VISION_V);         // Draws the objects as rectangles as interpreted by the vision sensor
      drawLines();                                  // Draws the lines where the object is still in bounds 
      break;
    default:
      drawActionManagerView();
      actionManager.printStatus();
      objectManager.printObjects(ACTION_V);
      break;
    }
    Brain.Screen.render();

    if (Brain.Screen.pressing()) {          // Returns true if the screen is being pressed
      switch (CURRENT_VIEW) {
      case ACTION_V:
        checkActionButton();                // Checks if the button to switch to the vision view or the stop button is being pressed
        break;
      case VISION_V:
        checkVisionButton();                // Checks if the button to return to Action View is being pressed
        break;
      }
    }

    vex::this_thread::sleep_for(150);       // Sleep for 150 ms
  }
}

void checkActionButton() {
  if ((Brain.Screen.xPosition() > 295) &&               // Checks if the touch position was in the bounds of the Vision Map button and
      (Brain.Screen.xPosition() < (295 + 110))) {       // sets the current view to Vision_V
    if ((Brain.Screen.yPosition() > 173) &&
        (Brain.Screen.yPosition() < (173 + 30))) {
      CURRENT_VIEW = VISION_V;
    }
  }

  if ((Brain.Screen.xPosition() > xVisionOffset + 21) &&          // Checks if the touch was in the bounds of the Stop button and exits the program if it is
      (Brain.Screen.xPosition() < (xVisionOffset + 61))) {
    if ((Brain.Screen.yPosition() > 10) &&
        (Brain.Screen.yPosition() < (50))) {
      exit();
    }
  }
}

void checkVisionButton() {
  if ((Brain.Screen.xPosition() > xVisionOffset + 21) &&          // Checks if the touch was in the bounds of the Exit button and sets the current view to Action_V
      (Brain.Screen.xPosition() < (xVisionOffset + 61))) {
    if ((Brain.Screen.yPosition() > 10) &&
        (Brain.Screen.yPosition() < (50))) {
      CURRENT_VIEW = ACTION_V;
    }
  }
}

void drawVisionMapView() {
  Brain.Screen.clearScreen(color::black);                                             // Sets the background color to black
  Brain.Screen.setPenColor(color::blue);                                              // Sets the pen color to blue
  Brain.Screen.drawLine(xOffset, 0, xOffset, visionHeight);                           // Draws the left bound of the vision map
  Brain.Screen.drawLine(xOffset, visionHeight, xVisionOffset, visionHeight);          // Draws the bottom bound of the vision map
  Brain.Screen.drawLine(xVisionOffset, visionHeight, xVisionOffset, 0);               // Draws the right bound of the vision map

  Brain.Screen.setPenColor(color::white);                                             // Sets the pen color to white
  Brain.Screen.drawRectangle(xVisionOffset + 21, 10, 40, 40, vex::color::red);        // Draws the rectangle for the Exit button                                            
  Brain.Screen.printAt(xVisionOffset + 25, 35, false, "Exit");                        // Draws the "Exit" text to the screen
}

void drawLines() {
  Brain.Screen.setPenColor(vex::color::purple);                                  // Sets pen color to purple
  Brain.Screen.drawLine(168 + xOffset, 0, 249 + xOffset, visionHeight);          // Draws middle bound line for the centered x
  Brain.Screen.drawLine(168 + xOffset, 0, 261 + xOffset, visionHeight);          // Draws the upper bound line
  Brain.Screen.drawLine(168 + xOffset, 0, 237 + xOffset, visionHeight);          // Draws the lower bound line
}

void drawActionManagerView() {
  Brain.Screen.setFont(fontType::mono15);       
  Brain.Screen.clearScreen(color::black);          // Clears the screen and sets the font of 

  Brain.Screen.setPenColor(vex::color::white);
  Brain.Screen.drawLine(xOffset, 0, xVisionOffset, 0);
  Brain.Screen.drawLine(xOffset, displayHeight, xVisionOffset, displayHeight);          // Creates a white bounding box that displays the objects x positions relative to the "center"          
  Brain.Screen.drawLine(xOffset, 0, xOffset, displayHeight);
  Brain.Screen.drawLine(xVisionOffset, 0, xVisionOffset, 50);

  Brain.Screen.setPenColor(vex::color::orange);
  Brain.Screen.drawLine(leftBound, 0, leftBound, displayHeight);
  Brain.Screen.drawLine(rightBound, 0, rightBound, displayHeight);          // Creates the left and right bounds of the center in orange
                                
  Brain.Screen.setPenColor(vex::color::red);
  Brain.Screen.drawLine(middleBound, 0, middleBound, displayHeight);          // Creates the middle target line 

  Brain.Screen.setPenColor(vex::color::purple);
  Brain.Screen.setCursor(17, 1);
  Brain.Screen.print("Distance Sensor: %.2f inches", Sonar.distance(vex::distanceUnits::in));         // Prints output of the distance sensor in purple

  Brain.Screen.setPenColor(objectManager.currentTrack.objectColor);
  Brain.Screen.setCursor(8, 45);
  Brain.Screen.print("PowerX: %.2f", objectManager.currentTrack.powerX);          // Displays the PowerX variable of the object currently being tracked

  Brain.Screen.setCursor(9, 45);
  Brain.Screen.print("Object ID: %d", objectManager.currentTrack.idZ);          // Object ID of the object currently being tracked

  Brain.Screen.setCursor(10, 45);
  Brain.Screen.print("Exists: %d", objectManager.currentTrack.doesObjectExist);          // Does the current object exist

  Brain.Screen.setPenColor(vex::color::white); 
  Brain.Screen.drawRectangle(295, 173, 110, 30);           // Vision view button 
  Brain.Screen.setCursor(13, 45);
  Brain.Screen.print("Vision View");                       // Vision view button text

  Brain.Screen.setPenColor(color::white);
  Brain.Screen.drawRectangle(xVisionOffset + 21, 10, 40, 40, vex::color::red);           // Draw stop button 
  Brain.Screen.setPenColor(color::white);
  Brain.Screen.printAt(xVisionOffset + 25, 35, false, "Stop"); 
}

void actionThread() {
  while (1) {
    actionManager.update(objectManager.currentTrack);           // Update ActionManager and pass the object currently being tracked to it
    vex::this_thread::sleep_for(25);                            // Sleep for 25ms
  }
}

void exit() {
  actionManager.stopEverything();          // Stop all action events
  actionThreadObject.interrupt();          // interrupt() stops the execution of each thread
  dataThreadObject.interrupt();
  printThreadObject.interrupt();
  vexSystemExitRequest();                  // System call to exit program
}
