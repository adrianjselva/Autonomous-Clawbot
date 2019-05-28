#include "ActionManager.h"

std::string ActionManager::getClawStatus() {
  switch (CLAW_STATUS) {
  case CLAW_OPEN_STATUS:
    return "Claw Open";
    break;
  case CLAW_CLOSED_STATUS:
    return "Claw Closed";
    break;
  }
  return "Unknown";
}

std::string ActionManager::getArmStatus() {
  switch (ARM_STATUS) {
  case ARM_UP_STATUS:
    return "Arm Up";
    break;
  case ARM_DOWN_STATUS:
    return "Arm Down";
    break;
  }
  return "Unknown";
}
std::string ActionManager::getObjectStatus() {
  switch (OBJECT_STATUS) {
  case EMPTY_HANDED:
    return "Empty Handed";
    break;
  case OBJECT_GRABBED:
    return "Object Grabbed";
    break;
  case OBJECT_PLACED:
    return "Object Placed";
    break;
  }
  return "Unknown";
}
std::string ActionManager::getActionStatus() {
  switch (ACTION_STATUS) {
  case STATUS_TRACK:
    return "Tracking object";
    break;
  case STATUS_SEARCH:
    return "Searching for an object";
    break;
  case STATUS_NOTHING:
    return "Doing Nothing";
    break;
  }
  return "Unknown";
}

std::string ActionManager::getTaskStatus() {
  switch (TASK_STATUS) {
  case TASK_PLACE:
    return "Placing object";
    break;
  case TASK_GRAB:
    return "Grabbing object";
    break;
  }
  return "Unknown";
}

std::string ActionManager::getTrackStatus() {
  switch (TRACK_AXIS) {
  case XDIRECTION:
    return "Rotating...";
    break;
  case YDIRECTION:
    return "Moving...";
    break;
  }
  return "Unknown";
}

int ActionManager::getClawStatusCode() { return CLAW_STATUS; }

int ActionManager::getArmStatusCode() { return ARM_STATUS; }

int ActionManager::getActionStatusCode() { return ACTION_STATUS; }

int ActionManager::getObjectStatusCode() { return OBJECT_STATUS; }

int ActionManager::getTaskStatusCode() { return TASK_STATUS; }

int ActionManager::getTrackStatusCode() { return TRACK_AXIS; }

void ActionManager::setClawStatus(int statusUpdate) {
  CLAW_STATUS = statusUpdate;
  switch (CLAW_STATUS) {
  case CLAW_OPEN_STATUS:
    clawColor.rgb(0, 255, 0);
    break;
  case CLAW_CLOSED_STATUS:
    clawColor.rgb(255, 0, 0);
    break;
  }
}

void ActionManager::setArmStatus(int statusUpdate) {
  ARM_STATUS = statusUpdate;

  switch (ARM_STATUS) {
  case ARM_UP_STATUS:
    armColor.rgb(0, 255, 0);
    break;
  case ARM_DOWN_STATUS:
    armColor.rgb(255, 0, 0);
    break;
  }
}

void ActionManager::setObjectStatus(int statusUpdate) {
  OBJECT_STATUS = statusUpdate;

  switch (OBJECT_STATUS) {
  case EMPTY_HANDED:
    objectColor.rgb(255, 0, 0);
    break;
  case OBJECT_GRABBED:
    objectColor.rgb(255, 255, 0);
    break;
  case OBJECT_PLACED:
    objectColor.rgb(0, 255, 0);
    break;
  }
}

void ActionManager::setActionStatus(int statusUpdate) {
  ACTION_STATUS = statusUpdate;

  switch (ACTION_STATUS) {
  case STATUS_TRACK:
    actionColor.rgb(0, 0, 255);
    break;
  case STATUS_SEARCH:
    actionColor.rgb(255, 255, 0);
    break;
  case STATUS_NOTHING:
    actionColor.rgb(255, 0, 0);
    break;
  }
}

void ActionManager::setTaskStatus(int statusUpdate) {
  TASK_STATUS = statusUpdate;
  switch (TASK_STATUS) {
  case TASK_GRAB:
    taskColor.rgb(255, 255, 0);
    break;
  case TASK_PLACE:
    taskColor.rgb(0, 255, 0);
    break;
  }
}

void ActionManager::setTrackStatus(int statusUpdate) {
  TRACK_AXIS = statusUpdate;
  switch (TRACK_AXIS) {
  case XDIRECTION:
    trackColor.rgb(0, 0, 255);
    break;
  case YDIRECTION:
    trackColor.rgb(0, 255, 0);
    break;
  }
}

void ActionManager::moveArm(int pos) {
  armMotorRef.setStopping(vex::brakeType::brake);
  armMotorRef.rotateTo(pos, vex::rotationUnits::deg, 20,
                       vex::velocityUnits::pct, true);

  switch (pos) {
  case ARM_UP:
    setArmStatus(ARM_UP_STATUS);
    break;
  case ARM_DOWN:
    setArmStatus(ARM_DOWN_STATUS);
    break;
  }
}

void ActionManager::moveClaw(int pos) {
  clawMotorRef.setStopping(vex::brakeType::brake);
  clawMotorRef.rotateTo(pos, vex::rotationUnits::deg, 20,
                        vex::velocityUnits::pct, true);

  switch (pos) {
  case CLAW_OPEN:
    setClawStatus(CLAW_OPEN_STATUS);
    break;
  case CLAW_CLOSED:
    setClawStatus(CLAW_CLOSED_STATUS);
    break;
  }
}

void ActionManager::grabObject() {
  rightMotorRef.stop(vex::brakeType::brake);
  leftMotorRef.stop(vex::brakeType::brake);

  moveArm(ARM_DOWN);
  moveClaw(CLAW_CLOSED);

  setObjectStatus(OBJECT_GRABBED);
  moveArm(ARM_UP);
  setTaskStatus(TASK_PLACE);

  if (placeCounter == 0) {
    setTargetObjectType(RED_OBJECT);
  } else {
    setTargetObjectType(RED_YELLOW_OBJECT);
  }
}

void ActionManager::update(Objects &currentT) {
  trackingObject = currentT;
  switch (ACTION_STATUS) {
  case STATUS_TRACK:
    track();
    break;
  case STATUS_SEARCH:
    searchFor();
    break;
  }
}

void ActionManager::placeObject() {
  rightMotorRef.stop(vex::brakeType::brake);
  leftMotorRef.stop(vex::brakeType::brake);

  moveArm(ARM_DOWN);
  moveClaw(CLAW_OPEN);
  setObjectStatus(OBJECT_PLACED);
  driveBack(4.0);
  moveArm(ARM_UP);
  setObjectStatus(EMPTY_HANDED);
  setActionStatus(STATUS_SEARCH);
  placeCounter = placeCounter + 1;
  setTaskStatus(TASK_GRAB);
  setTargetObjectType(YELLOW_OBJECT);
}

void ActionManager::doTask() {
  switch (TASK_STATUS) {
  case TASK_GRAB:
    grabObject();
    break;
  case TASK_PLACE:
    placeObject();
    break;
  }
}

void ActionManager::track() {
  switch (TRACK_AXIS) {
  case XDIRECTION:
    centerX();
    break;
  case YDIRECTION:
    centerY();
    break;
  }
}

void ActionManager::centerX() {
  if (!trackingObject.isInBoundVision && trackingObject.doesObjectExist) {
    if (trackingObject.powerX > 0.5) {
      rightMotorRef.spin(vex::directionType::rev, trackingObject.powerX,
                         vex::velocityUnits::pct);
      leftMotorRef.spin(vex::directionType::fwd, trackingObject.powerX,
                        vex::velocityUnits::pct);
    } else if ((trackingObject.powerX <= 0.5) &&
               (trackingObject.powerX >= -0.5)) {
      rightMotorRef.stop(vex::brakeType::brake);
      leftMotorRef.stop(vex::brakeType::brake);
    } else if (trackingObject.powerX < -0.5) {
      rightMotorRef.spin(vex::directionType::fwd, -trackingObject.powerX,
                         vex::velocityUnits::pct);
      leftMotorRef.spin(vex::directionType::rev, -trackingObject.powerX,
                        vex::velocityUnits::pct);
    }
  } else if (!trackingObject.doesObjectExist) {
    setActionStatus(STATUS_SEARCH);
  } else if (trackingObject.isInBoundVision && trackingObject.doesObjectExist) {
    rightMotorRef.stop(vex::brakeType::brake);
    leftMotorRef.stop(vex::brakeType::brake);
    setTrackStatus(YDIRECTION);
  }
}

void ActionManager::centerY() {
  if (!trackingObject.isInBoundDistance && trackingObject.doesObjectExist &&
      trackingObject.isInBoundVision) {
    if (trackingObject.powerY > .5) {
      rightMotorRef.spin(vex::directionType::fwd, trackingObject.powerY,
                         vex::velocityUnits::pct);
      leftMotorRef.spin(vex::directionType::fwd, trackingObject.powerY,
                        vex::velocityUnits::pct);
    } else if ((trackingObject.powerY <= .5) &&
               (trackingObject.powerY >= -.5)) {
      rightMotorRef.stop(vex::brakeType::brake);
      leftMotorRef.stop(vex::brakeType::brake);
    } else if (trackingObject.powerY < -.5) {
      rightMotorRef.spin(vex::directionType::rev, -trackingObject.powerY,
                         vex::velocityUnits::pct);
      leftMotorRef.spin(vex::directionType::rev, -trackingObject.powerY,
                        vex::velocityUnits::pct);
    }
  } else if (trackingObject.doesObjectExist &&
             trackingObject.isBoundVisionDistance) {
    rightMotorRef.stop(vex::brakeType::brake);
    leftMotorRef.stop(vex::brakeType::brake);
    doTask();
    setTrackStatus(XDIRECTION);
  }

  if (!trackingObject.isInBoundVision) {
    setTrackStatus(XDIRECTION);
  }

  if (!trackingObject.doesObjectExist) {
    setActionStatus(STATUS_SEARCH);
  }
}

void ActionManager::searchFor() {
  if (trackingObject.doesObjectExist) {
    setActionStatus(STATUS_TRACK);
  } else {
    rightMotorRef.spin(vex::directionType::rev, 10, vex::velocityUnits::pct);
    leftMotorRef.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
  }
}

void ActionManager::printStatus() {
  screenAction.setPenColor(vex::color::white);
  screenAction.setCursor(5, 1);
  screenAction.print("Claw: ");
  screenAction.setCursor(6, 1);
  screenAction.print("Arm: ");
  screenAction.setCursor(7, 1);
  screenAction.print("Object: ");
  screenAction.setCursor(8, 1);
  screenAction.print("Action: ");
  screenAction.setCursor(9, 1);
  screenAction.print("Task ");

  screenAction.setPenColor(clawColor);
  screenAction.setCursor(5, 12);
  screenAction.print("%s", getClawStatus().c_str());

  screenAction.setPenColor(armColor);
  screenAction.setCursor(6, 12);
  screenAction.print("%s", getArmStatus().c_str());

  screenAction.setPenColor(objectColor);
  screenAction.setCursor(7, 12);
  screenAction.print("%s", getObjectStatus().c_str());

  screenAction.setPenColor(actionColor);
  screenAction.setCursor(8, 12);
  screenAction.print("%s", getActionStatus().c_str());

  screenAction.setPenColor(taskColor);
  screenAction.setCursor(9, 12);
  screenAction.print("%s", getTaskStatus().c_str());
}

void ActionManager::initialize() {
  moveArm(ARM_UP);

  setActionStatus(STATUS_SEARCH);
}

bool ActionManager::isReadyToTrack() {
  return (trackingObject.isSufficientData &&
          !trackingObject.isBoundVisionDistance &&
          trackingObject.doesObjectExist);
}

void ActionManager::driveBack(double inches) {
  double degreesToRotate = (360.0 * inches) / (4.0 * M_PI);

  leftMotorRef.setVelocity(30, vex::velocityUnits::pct);
  rightMotorRef.setVelocity(30, vex::velocityUnits::pct);

  leftMotorRef.rotateFor(-degreesToRotate, vex::rotationUnits::deg, false);
  rightMotorRef.rotateFor(-degreesToRotate, vex::rotationUnits::deg);
}

void ActionManager::stopEverything() {
  if (rightMotorRef.isSpinning()) {
    rightMotorRef.stop();
  }

  if (leftMotorRef.isSpinning()) {
    leftMotorRef.stop();
  }

  if (ARM_STATUS == ARM_UP_STATUS) {
    moveArm(ARM_DOWN);
  }

  if (CLAW_STATUS == CLAW_CLOSED_STATUS) {
    moveClaw(CLAW_OPEN);
  }
}

void ActionManager::setTargetObjectType(int status) {
  TARGET_OBJECT_TYPE = status;
}

ActionManager::ActionManager(vex::motor &right, vex::motor &left,
                             vex::motor &claw, vex::motor &arm,
                             vex::brain::lcd &screenRef)
    : rightMotorRef(right), leftMotorRef(left), clawMotorRef(claw),
      armMotorRef(arm) {

  setClawStatus(CLAW_OPEN_STATUS);
  setArmStatus(ARM_DOWN_STATUS);
  setObjectStatus(EMPTY_HANDED);
  setActionStatus(STATUS_NOTHING);
  setTaskStatus(TASK_GRAB);
  setTrackStatus(XDIRECTION);

  setTargetObjectType(YELLOW_OBJECT);

  screenAction = screenRef;

  placeCounter = 0;
}