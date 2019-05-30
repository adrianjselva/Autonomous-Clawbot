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
const int ARM_DOWN = 7;
const int CLAW_OPEN = 5;
const int CLAW_CLOSED = 230;

/** Vision dimensions **/
const int VISION_VIEW_WIDTH = 316;
const int VISION_VIEW_HEIGHT = 212;

/** Screen Offset **/
const int LEFT_BOUND_OFFSETTED = 82;
const int RIGHT_BOUND_OFFSETTED = (LEFT_BOUND_OFFSETTED + VISION_VIEW_WIDTH);
const int LEFT_X_CTR = LEFT_BOUND_OFFSETTED + MIN_X;
const int RIGHT_X_CTR = LEFT_BOUND_OFFSETTED + MAX_X;
const int MIDDLE_X_CTR = LEFT_BOUND_OFFSETTED + MID_X;

const int DISPLAY_HEIGHT = 50;

/** Size of circles **/
const int CIRCLE_RADIUS = 2;

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

/** Object type ID **/
const int UNKNOWN_OBJECT = 0;
const int YELLOW_OBJECT = 1;
const int RED_OBJECT = 2;
const int RED_YELLOW_OBJECT = 3;

/** Views **/
const int ACTION_V = 0;
const int VISION_V = 1;

/** Vision Map Button Bounds **/
const int VISION_MAP_BUTTON_LOWER_BOUND_X = 295;
const int VISION_MAP_BUTTON_UPPER_BOUND_X = 405;
const int VISION_MAP_BUTTON_LOWER_BOUND_Y = 173;
const int VISION_MAP_BUTTON_UPPER_BOUND_Y = 203;

/** Vision map button position and size **/
const int VISION_BUTTON_X = 295;
const int VISION_BUTTON_Y = 173;
const int VISION_BUTTON_WIDTH = 110;
const int VISION_BUTTON_HEIGHT = 30;

/** Stop Button Bounds **/
const int STOP_BUTTON_LOWER_BOUND_X = RIGHT_BOUND_OFFSETTED + 21;
const int STOP_BUTTON_UPPER_BOUND_X = RIGHT_BOUND_OFFSETTED + 61;
const int STOP_BUTTON_LOWER_BOUND_Y = 10;
const int STOP_BUTTON_UPPER_BOUND_Y = 50;

/** Exit button bounds **/
const int EXIT_BUTTON_LOWER_BOUND_X = RIGHT_BOUND_OFFSETTED + 21;
const int EXIT_BUTTON_UPPER_BOUND_X = RIGHT_BOUND_OFFSETTED + 61;
const int EXIT_BUTTON_LOWER_BOUND_Y = 10;
const int EXIT_BUTTON_UPPER_BOUND_Y = 50;

/** Exit button position and size **/
const int EXIT_BUTTON_X = RIGHT_BOUND_OFFSETTED + 21;
const int EXIT_BUTTON_Y = 10;
const int EXIT_BUTTON_WIDTH = 40;
const int EXIT_BUTTON_HEIGHT = 40;

/** Exit text position **/
const int EXIT_TEXT_X = RIGHT_BOUND_OFFSETTED + 25;
const int EXIT_TEXT_Y = 35;

/** Stop text position **/
const int STOP_TEXT_X = RIGHT_BOUND_OFFSETTED + 25;
const int STOP_TEXT_Y = 35;

/** Stop button position and size **/
const int STOP_BUTTON_X = RIGHT_BOUND_OFFSETTED + 21;
const int STOP_BUTTON_Y = 10;
const int STOP_BUTTON_WIDTH = 40;
const int STOP_BUTTON_HEIGHT = 40;

/** Distance bound limits **/
const double DISTANCE_UPPER = 8.1;
const double DISTANCE_LOWER = 7.9;
const double DISTANCE_TARGET = 8.0;

#endif