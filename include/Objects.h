#ifndef OBJECTS
#define OBJECTS
#include "constants.h"
#include "v5.h"
#include "v5_vcs.h"
#include <math.h>

class Objects
{
private:
  /** Perform calculations **/
  void updateArrays(int visionX, int visionY, int height, int width, double distanceTo);
  void setValues(int id, int height, int width, int originX, int originY);
  void setConditions();
  void setPower();
  void setPowerX();
  void setPowerY();

  /** Averaging functions **/
  double averageArray(int a[]);
  double averageArray(double a[]);

  /** Counter variables **/
  int arrayLoopCounter;
  int sufficientDataCounter;

  /** Arrays of values **/
  int xPositionArray[10];
  int yPositionArray[10];
  int widthArray[10];
  int heightArray[10];
  double distanceArray[10];
  
  /** Check if the object is ready to grab **/
  void setBoundVision();
  void setBoundDistance();

  /** Screen Reference **/
  vex::brain::lcd screen;

  void setExistence();
  void setDefaultValues();

  int upperVisionBoundX();
  int lowerVisionBoundX();
  int middleVisionBoundX();
  void setObjectType();

public:
  /** Object variables **/
  bool doesObjectExist;
  bool isInBoundVision;
  bool isInBoundDistance;
  bool isBoundVisionDistance;
  bool isSufficientData;

  double averageDistanceToObject;
  double powerX;
  double powerY;
  int area;

  int averagedX;
  int averagedY;
  int objectType;
  int averagedHeight;
  int averagedWidth;
  int idZ;
  int originX;
  int originY;

  vex::color objectColor;

  /** Functions **/
  void updateValues(int visionX, int visionY, int originX, int originY, double distanceTo, int id, int height, int width);
  void setScreen(vex::brain::lcd& t);
  void printObject(int VIEW);

  /** CONSTRUCTORS **/
  Objects();
  Objects(vex::brain::lcd& brainD);
};

#endif