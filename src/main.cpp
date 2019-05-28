#include "ActionManager.h"
#include "ObjectManager.h"
#include "constants.h"
#include "robot-config.h"
#include "visionYellow.h"

/** Robot status conditionals **/
bool isArmUp = false;
bool isClawClosed = true;
bool isGrabbed = false;

/** Function that draws the screen map **/
void drawActionView();
void drawVisionMap();
void checkActionButton();
void checkVisionButton();

/** Thread functions **/
int printInfo();
int dataThread();
void actionThread();

void driveBac(double inches);

bool isObjectReadyToGrab();
void drawLines();

int VIEW;
const int ACTION_V = 0;
const int VISION_V = 1;

vex::thread dt;
vex::thread ac;
vex::thread pr;

/** Hierarchical functions **/
void init();
void exit();

// Robot dimensions
ObjectManager objectManager(Brain.Screen);
ActionManager actionManager(RightMotor, LeftMotor, ClawMotor, ArmMotor,
                            Brain.Screen);

int main() {
  init();

  dt = vex::thread (dataThread); // Initialize thread objects dt, ac, and pr
  ac = vex::thread (actionThread);                 // dataThread, actionThread,
  // and printInfo are functions
  pr = vex::thread (printInfo);

  dt.join();
  ac.join();
  pr.join();
  return 0;
}

void init() { // Set the font of the screen
  Brain.Screen.render(
      true, false); // Disable automatic rendering, prevents flickering
  VIEW = ACTION_V;
  actionManager
      .initialize(); // Move arm to the upwards position and open the claw
}

int dataThread() {
  while (1) {
    Vision1.takeSnapshot(0);
    objectManager.update(Vision1.objects,
                         Sonar.distance(vex::distanceUnits::in), actionManager.TARGET_OBJECT_TYPE);
    vex::this_thread::sleep_for(25); // Sleep for 25 ms
  }
  return (0);
}

int printInfo() {
  while (1) {
    switch (VIEW) {
    case ACTION_V:
      drawActionView();
      actionManager.printStatus();
      objectManager.printObjects(ACTION_V);
      break;
    case VISION_V:
      drawVisionMap();
      objectManager.printObjects(VISION_V);
      drawLines();
      break;
    default:
      drawActionView();
      actionManager.printStatus();
      objectManager.printObjects(ACTION_V);
      break;
    }
    Brain.Screen.render();

    if (Brain.Screen.pressing()) {
      switch (VIEW) {
      case ACTION_V:
        checkActionButton();
        break;
      case VISION_V:
        checkVisionButton();
        break;
      }
    }

    vex::this_thread::sleep_for(150);
  }
  return (0);
}

void checkActionButton() {
  if ((Brain.Screen.xPosition() > 295) &&
      (Brain.Screen.xPosition() < (295 + 110))) {
    if ((Brain.Screen.yPosition() > 173) &&
        (Brain.Screen.yPosition() < (173 + 30))) {
      VIEW = VISION_V;
    }
  }

  if ((Brain.Screen.xPosition() > xVisionOffset + 21) &&
      (Brain.Screen.xPosition() < (xVisionOffset + 61))) {
    if ((Brain.Screen.yPosition() > 10) &&
        (Brain.Screen.yPosition() < (50))) {
      exit();
    }
  }
}

void checkVisionButton() {
  if ((Brain.Screen.xPosition() > xVisionOffset + 21) &&
      (Brain.Screen.xPosition() < (xVisionOffset + 61))) {
    if ((Brain.Screen.yPosition() > 10) &&
        (Brain.Screen.yPosition() < (50))) {
      VIEW = ACTION_V;
    }
  }
}

void drawVisionMap() {
  Brain.Screen.clearScreen(color::black);
  Brain.Screen.setPenColor(color::blue);
  Brain.Screen.drawLine(xOffset, 0, xOffset, visionHeight);
  Brain.Screen.drawLine(xOffset, visionHeight, xVisionOffset, visionHeight);
  Brain.Screen.drawLine(xVisionOffset, visionHeight, xVisionOffset, 0);

  Brain.Screen.setPenColor(color::white);
  Brain.Screen.drawRectangle(xVisionOffset + 21, 10, 40, 40, vex::color::red);
  Brain.Screen.setPenColor(color::white);
  Brain.Screen.printAt(xVisionOffset + 25, 35, false, "Exit");
}

void drawLines() {
  Brain.Screen.setPenColor(vex::color::purple);
  Brain.Screen.drawLine(168 + xOffset, 0, 249 + xOffset, visionHeight);
  Brain.Screen.drawLine(168 + xOffset, 0, 261 + xOffset, visionHeight);
  Brain.Screen.drawLine(168 + xOffset, 0, 237 + xOffset, visionHeight);
}

void drawActionView() {
  Brain.Screen.setFont(fontType::mono15);
  Brain.Screen.clearScreen(color::black);
  Brain.Screen.setPenColor(vex::color::white);
  Brain.Screen.drawLine(xOffset, 0, xVisionOffset, 0);
  Brain.Screen.drawLine(xOffset, displayHeight, xVisionOffset, displayHeight);
  Brain.Screen.drawLine(xOffset, 0, xOffset, displayHeight);
  Brain.Screen.drawLine(xVisionOffset, 0, xVisionOffset, 50);
  Brain.Screen.setPenColor(vex::color::orange);
  Brain.Screen.drawLine(leftBound, 0, leftBound, displayHeight);
  Brain.Screen.drawLine(rightBound, 0, rightBound, displayHeight);
  Brain.Screen.setPenColor(vex::color::red);
  Brain.Screen.drawLine(middleBound, 0, middleBound, displayHeight);

  Brain.Screen.setPenColor(vex::color::purple);
  Brain.Screen.setCursor(17, 1);
  Brain.Screen.print("Distance Sensor: %.2f inches",
                     Sonar.distance(vex::distanceUnits::in));

  Brain.Screen.setPenColor(objectManager.currentTrack.objectColor);
  Brain.Screen.setCursor(8, 45);
  Brain.Screen.print("PowerX: %.2f", objectManager.currentTrack.powerX);

  Brain.Screen.setCursor(9, 45);
  Brain.Screen.print("Object ID: %d", objectManager.currentTrack.idZ);

  Brain.Screen.setCursor(10, 45);
  Brain.Screen.print("Exists: %d", objectManager.currentTrack.doesObjectExist);

  Brain.Screen.setPenColor(vex::color::white);
  Brain.Screen.drawRectangle(295, 173, 110, 30);
  Brain.Screen.setCursor(13, 45);
  Brain.Screen.print("Vision View");

  Brain.Screen.setPenColor(color::white);
  Brain.Screen.drawRectangle(xVisionOffset + 21, 10, 40, 40, vex::color::red);
  Brain.Screen.setPenColor(color::white);
  Brain.Screen.printAt(xVisionOffset + 25, 35, false, "Stop");
}

void actionThread() {
  while (1) {
    actionManager.update(objectManager.currentTrack);
    vex::this_thread::sleep_for(25);
  }
}

void exit() {
  actionManager.stopEverything();
  ac.interrupt();
  dt.interrupt();
  pr.interrupt();
  vexSystemExitRequest();
}
