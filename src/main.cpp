#include "ActionManager.h"
#include "ObjectManager.h"
#include "constants.h"
#include "robot-config.h"
#include "visionYellow.h"

/** Startup and exit functions **/
void init();
void exit();

/** Drawing functions **/
void drawActionManagerView();
void drawVisionMapView();
void checkActionButton();
void checkVisionButton();
void drawLines();

/** Button bound check **/
bool isInBounds(int c, int low, int high);

/** Thread functions **/
void printInfo();
void dataThread();
void actionThread();

/** Variable to keep track of current view **/
int CURRENT_VIEW;

/** Thread objects **/
vex::thread dataThreadObject;
vex::thread actionThreadObject;
vex::thread printThreadObject;

/** Manager objects **/
ObjectManager objectManager(&Brain.Screen);
ActionManager actionManager(&RightMotor, &LeftMotor, &ClawMotor, &ArmMotor, &Brain.Screen);

int main() {
  init();                                                   // Function to set some initial values

  dataThreadObject = vex::thread (dataThread);              // Initialize thread objects dataThreadObject, actionThreadObject, and printThreadObject
  actionThreadObject = vex::thread (actionThread);          // dataThread, actionThread, and printInfo are the functions executed by each thread object
  printThreadObject = vex::thread (printInfo);

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

void actionThread() {
  while (1) {
    actionManager.update(&objectManager.currentTrack);       // Update ActionManager and pass the object currently being tracked to it
    vex::this_thread::sleep_for(25);                        // Sleep for 25ms
  }
}

void printInfo() {
  while (1) {
    switch (CURRENT_VIEW) {
    case ACTION_V:
      drawActionManagerView();                              // Draws the ActionManagerView which displays the state of the robot
      actionManager.printStatus();                          // Prints information from the ActionManager class about the state of the robot
      objectManager.printObjects(ACTION_V);                 // Draws the objects to the display in ACTION_V form
      break;
    case VISION_V:
      drawVisionMapView();                                  // Draws a visual representation of the Vision sensor data
      objectManager.printObjects(VISION_V);                 // Draws the objects as rectangles as interpreted by the vision sensor
      drawLines();                                          // Draws the lines where the object is still in bounds 
      break;
    default:
      drawActionManagerView();
      actionManager.printStatus();
      objectManager.printObjects(ACTION_V);
      break;
    }
    Brain.Screen.render();                                  // Draws views to screen

    if (Brain.Screen.pressing()) {                          // Returns true if the screen is being pressed
      switch (CURRENT_VIEW) {
      case ACTION_V:
        checkActionButton();                                // Checks if the button to switch to the vision view or the stop button is being pressed
        break;
      case VISION_V:
        checkVisionButton();                                // Checks if the button to return to Action View is being pressed
        break;
      }
    }

    vex::this_thread::sleep_for(150);                       // Sleep for 150 ms
  }
}

bool isInBounds(int c, int low, int high) {                 // Return true if coordinate passed is in between the two values
  return((c <= high) && (c >= low));
}

void checkActionButton() {
  if(isInBounds(Brain.Screen.xPosition(), VISION_MAP_BUTTON_LOWER_BOUND_X, VISION_MAP_BUTTON_UPPER_BOUND_X) &&          // Checks if the touch position was in the bounds of the Vision Map button and
  isInBounds(Brain.Screen.yPosition(), VISION_MAP_BUTTON_LOWER_BOUND_Y, VISION_MAP_BUTTON_UPPER_BOUND_Y)) {             // sets the current view to Vision_V
    CURRENT_VIEW = VISION_V;
  }

  if(isInBounds(Brain.Screen.xPosition(), STOP_BUTTON_LOWER_BOUND_X, STOP_BUTTON_UPPER_BOUND_X) &&                      // Checks if the touch was in the bounds of the Stop button and exits the program if it is
  isInBounds(Brain.Screen.yPosition(), STOP_BUTTON_LOWER_BOUND_Y, STOP_BUTTON_UPPER_BOUND_Y)) {
    exit();
  }
}

void checkVisionButton() {
  if(isInBounds(Brain.Screen.xPosition(), EXIT_BUTTON_LOWER_BOUND_X, EXIT_BUTTON_UPPER_BOUND_X) &&                      // Checks if the touch was in the bounds of the Exit button and sets the current view to Action_V
  isInBounds(Brain.Screen.yPosition(), EXIT_BUTTON_LOWER_BOUND_Y, EXIT_BUTTON_UPPER_BOUND_Y)) {
    CURRENT_VIEW = ACTION_V;
  }
}

void drawVisionMapView() {
  Brain.Screen.clearScreen(color::black);                                                                                          // Sets the background color to black
  Brain.Screen.setPenColor(color::blue);                                                                                           // Sets the pen color to blue
  Brain.Screen.drawLine(LEFT_BOUND_OFFSETTED, 0, LEFT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT);                                                                        // Draws the left bound of the vision map
  Brain.Screen.drawLine(LEFT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT, RIGHT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT);                                                       // Draws the bottom bound of the vision map
  Brain.Screen.drawLine(RIGHT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT, RIGHT_BOUND_OFFSETTED, 0);                                                            // Draws the right bound of the vision map

  Brain.Screen.setPenColor(color::white);                                                                                          // Sets the pen color to white
  Brain.Screen.drawRectangle(EXIT_BUTTON_X, EXIT_BUTTON_Y, EXIT_BUTTON_WIDTH, EXIT_BUTTON_LOWER_BOUND_X, vex::color::red);         // Draws the rectangle for the Exit button                                            
  Brain.Screen.printAt(EXIT_TEXT_X, EXIT_TEXT_Y, false, "Exit");                                                                   // Draws the "Exit" text to the screen
}

void drawLines() {
  Brain.Screen.setPenColor(vex::color::purple);                                       // Sets pen color to purple
  Brain.Screen.drawLine(168 + LEFT_BOUND_OFFSETTED, 0, 249 + LEFT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT);               // Draws middle bound line for the centered x
  Brain.Screen.drawLine(168 + LEFT_BOUND_OFFSETTED, 0, 261 + LEFT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT);               // Draws the upper bound line
  Brain.Screen.drawLine(168 + LEFT_BOUND_OFFSETTED, 0, 237 + LEFT_BOUND_OFFSETTED, VISION_VIEW_HEIGHT);               // Draws the lower bound line
} /** Based on calculations of linear equations in Objects.cpp **/

void drawActionManagerView() {
  Brain.Screen.setFont(fontType::mono15);       
  Brain.Screen.clearScreen(color::black);                                             // Clears the screen and sets the font

  Brain.Screen.setPenColor(vex::color::white);
  Brain.Screen.drawLine(LEFT_BOUND_OFFSETTED, 0, RIGHT_BOUND_OFFSETTED, 0);
  Brain.Screen.drawLine(LEFT_BOUND_OFFSETTED, DISPLAY_HEIGHT, RIGHT_BOUND_OFFSETTED, DISPLAY_HEIGHT);        // Creates a white bounding box that displays the objects x positions relative to the "center"          
  Brain.Screen.drawLine(LEFT_BOUND_OFFSETTED, 0, LEFT_BOUND_OFFSETTED, DISPLAY_HEIGHT);
  Brain.Screen.drawLine(RIGHT_BOUND_OFFSETTED, 0, RIGHT_BOUND_OFFSETTED, DISPLAY_HEIGHT);

  Brain.Screen.setPenColor(vex::color::orange);
  Brain.Screen.drawLine(LEFT_X_CTR, 0, LEFT_X_CTR, DISPLAY_HEIGHT);
  Brain.Screen.drawLine(RIGHT_X_CTR, 0, RIGHT_X_CTR, DISPLAY_HEIGHT);                    // Creates the left and right bounds of the center in orange
                                
  Brain.Screen.setPenColor(vex::color::red);
  Brain.Screen.drawLine(MIDDLE_X_CTR, 0, MIDDLE_X_CTR, DISPLAY_HEIGHT);                  // Creates the middle target line 

  Brain.Screen.setPenColor(vex::color::purple);
  Brain.Screen.setCursor(17, 1);
  Brain.Screen.print("Distance Sensor: %.2f inches", Sonar.distance(vex::distanceUnits::in));         // Prints output of the distance sensor in purple

  Brain.Screen.setPenColor(objectManager.currentTrack.objectColor);
  Brain.Screen.setCursor(8, 45);
  Brain.Screen.print("PowerX: %.2f", objectManager.currentTrack.powerX);              // Displays the PowerX variable of the object currently being tracked

  Brain.Screen.setCursor(9, 45);
  Brain.Screen.print("Object ID: %d", objectManager.currentTrack.idZ);                // Object ID of the object currently being tracked

  Brain.Screen.setCursor(10, 45);
  Brain.Screen.print("Exists: %d", objectManager.currentTrack.doesObjectExist);       // Does the current object exist

  Brain.Screen.setPenColor(vex::color::white); 
  Brain.Screen.drawRectangle(VISION_BUTTON_X, VISION_BUTTON_Y, VISION_BUTTON_WIDTH, VISION_BUTTON_HEIGHT);                        // Vision view button 
  Brain.Screen.setCursor(13, 45);
  Brain.Screen.print("Vision View");                                                  // Vision view button text

  Brain.Screen.setPenColor(color::white);
  Brain.Screen.drawRectangle(STOP_BUTTON_X, STOP_BUTTON_Y, STOP_BUTTON_WIDTH, STOP_BUTTON_LOWER_BOUND_X, vex::color::red);        // Draw stop button 
  Brain.Screen.setPenColor(color::white);
  Brain.Screen.printAt(STOP_TEXT_X, STOP_TEXT_Y, false, "Stop"); 
}

void exit() {
  actionManager.stopEverything();                                                     // Stop all action events
  actionThreadObject.interrupt();                                                     // interrupt() stops the execution of each thread
  dataThreadObject.interrupt();
  printThreadObject.interrupt();
  vexSystemExitRequest();                                                             // System call to exit program
}
