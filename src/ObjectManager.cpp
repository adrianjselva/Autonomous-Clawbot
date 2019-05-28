#include "ObjectManager.h"

void ObjectManager::update(vex::safearray<vex::vision::object, 16>& vexArray, double dist, int OTYPE) {
  for(int x = 0; x < 2; x++) {
    objectVector.at(x).updateValues(vexArray[x].centerX, vexArray[x].centerY, vexArray[x].originX, vexArray[x].originY, dist, vexArray[x].id, vexArray[x].height, vexArray[x].width);

    if(objectVector.at(x).idZ == OTYPE) {
      currentTrack = objectVector.at(x);
    }
  }
  //sortObjects();
  //setCurrentTrack(OTYPE);
}

void ObjectManager::sortObjects() {
  for(int i = 0; i < 5; i ++) {
    switch(objectVector.at(i).idZ){
      case YELLOW_OBJECT:
        sortYellow(objectVector.at(i));
        break;
      case RED_OBJECT:
        sortRed(objectVector.at(i));
        break;
      case RED_YELLOW_OBJECT:
        sortYellowRed(objectVector.at(i));
        break;
    }
  }
}

void ObjectManager::setCurrentTrack(int OTYPE) {
  switch(OTYPE){
    case YELLOW_OBJECT:
      currentTrack = yellowObjects.at(0);
      break;
    case RED_OBJECT:
      currentTrack = redObjects.at(0);
      break;
    case RED_YELLOW_OBJECT:
      currentTrack = yellowAndRed.at(0);
      break;
  }
}

void ObjectManager::sortYellow(Objects &yellow) {
  yellowObjects.at(0) = yellow;
}

void ObjectManager::sortRed(Objects &red) {
  redObjects.at(0) = red;
}

void ObjectManager::sortYellowRed(Objects &yellowRed) {
  yellowAndRed.at(0) = yellowRed;
}

void ObjectManager::printObjects(int VIEW) {
  for(auto x : objectVector) {
    x.printObject(VIEW);
  }
}

ObjectManager::ObjectManager(vex::brain::lcd& brainD) : objectVector(5, Objects(brainD)), yellowObjects(5, Objects(brainD)),
        redObjects(1, Objects(brainD)), yellowAndRed(1, Objects(brainD)){
  screen = brainD;

  redCounter = 0;
  yellowAndRedCounter = 0;
  yellowCounter = 0;
}