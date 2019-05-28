#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"
//
using namespace vex;

vex::brain Brain;
vex::sonar Sonar = vex::sonar(Brain.ThreeWirePort.B);
vex::motor LeftMotor (vex::PORT1, vex::gearSetting::ratio18_1,false);
vex::motor ClawMotor (vex::PORT5, vex::gearSetting::ratio18_1,false);
vex::motor ArmMotor (vex::PORT10, vex::gearSetting::ratio18_1,false);
vex::motor RightMotor (vex::PORT9, vex::gearSetting::ratio18_1,true);