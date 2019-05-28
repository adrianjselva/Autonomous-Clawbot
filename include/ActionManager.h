#include "constants.h"
#include "Objects.h"
#include <string>
#include "v5.h"
#include "v5_vcs.h"

class ActionManager {
  public: 

  int ACTION_STATUS;
  int OBJECT_STATUS;
  int ARM_STATUS;
  int CLAW_STATUS;
  int TASK_STATUS;
  int TRACK_AXIS;

  int TARGET_OBJECT_TYPE;

  int placeCounter;

  Objects trackingObject;

  vex::motor& rightMotorRef;
  vex::motor& leftMotorRef;
  vex::motor& clawMotorRef;
  vex::motor& armMotorRef;

  vex::brain::lcd screenAction;

  vex::color clawColor;
  vex::color armColor;
  vex::color objectColor;
  vex::color actionColor;
  vex::color taskColor;
  vex::color trackColor;

  void setClawStatus(int statusUpdate);
  void setArmStatus(int statusUpdate);
  void setObjectStatus(int statusUpdate);
  void setActionStatus(int statusUpdate);
  void setTaskStatus(int statusUpdate);
  void setTrackStatus(int statusUpdate);

  void setTargetObjectType(int status);

  int getClawStatusCode();
  int getArmStatusCode();
  int getObjectStatusCode();
  int getActionStatusCode();
  int getTaskStatusCode();
  int getTrackStatusCode();

  std::string getClawStatus();
  std::string getArmStatus();
  std::string getObjectStatus();
  std::string getActionStatus();
  std::string getTaskStatus();
  std::string getTrackStatus();

  void moveArm(int pos);
  void moveClaw(int pos);

  void centerX();
  void centerY();

  void grabObject();
  void doTask();

  void update(Objects& currentT);
  void track();
  void placeObject();
  void searchFor();
  void initialize();
  void stopEverything();
  void printStatus();

  void moveArmTrack();

  bool isReadyToTrack();
  void driveBack(double inches);

  ActionManager(vex::motor& right, vex::motor& left, vex::motor& claw, vex::motor& arm, vex::brain::lcd& screenRef);
};