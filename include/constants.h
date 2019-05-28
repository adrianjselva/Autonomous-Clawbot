#ifndef CONSTANTS_H
#define CONSTANTS_H

/** X-Bound values **/
const int MAX_X = 225;
const int MIN_X = 205;
const int MID_X = 215;

/** PID CONSTANTS - X **/
const double kP = 0.08;
const double kI = 0.0006;
const double kD = 0.0;

/** PID CONSTANTS - Y **/
const double kPY = 1.8;
const double kIY = 0;
const double kDY = 0;

/** Motor Position Values **/
const int ARM_UP = 500;
const int ARM_DOWN = 0;
const int CLAW_OPEN = 5;
const int CLAW_CLOSED = 230;

/** Vision dimensions **/
const int visionWidth = 316;
const int visionHeight = 212;

/** Screen Offset **/
const int xOffset = 82;
const int xVisionOffset = (xOffset + visionWidth);
const int leftBound = xOffset + MIN_X;
const int rightBound = xOffset + MAX_X;
const int middleBound = xOffset + MID_X;
const int displayHeight = 50;

/** Screen drawing variables **/
const int circleRadius = 2;
const int textOffset = 4;

/** Distance Bounds **/
const int distanceUpper = 200;
const int distanceLower = 190;

const int redDistanceUpper = 190;
const int redDistanceLower = 180;

/** Status Constants **/
const int STATUS_TRACK = 0;
const int STATUS_SEARCH = 1;
const int STATUS_NOTHING = 2;

const int TASK_GRAB = 0;
const int TASK_PLACE = 1;

const int EMPTY_HANDED = 0;
const int OBJECT_GRABBED = 1;
const int OBJECT_PLACED = 2;

const int ARM_DOWN_STATUS = 0;
const int ARM_UP_STATUS = 1;

const int CLAW_CLOSED_STATUS = 0;
const int CLAW_OPEN_STATUS = 1;

const int XDIRECTION = 0;
const int YDIRECTION = 1;

/** **/
const int UNKNOWN_OBJECT = 0;
const int YELLOW_OBJECT = 1;
const int RED_OBJECT = 2;
const int RED_YELLOW_OBJECT = 3;

#endif