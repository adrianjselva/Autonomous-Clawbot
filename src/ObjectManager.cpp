#include "ObjectManager.h"

void ObjectManager::update(vex::safearray<vex::vision::object, 16> &vexArray, double dist, int OTYPE) {         // Called every frame and updates the objectVector with data from the array
  for(int x = 0; x < 5; x++) {
    objectVector.at(x).updateValues(vexArray[x].centerX, vexArray[x].centerY, vexArray[x].originX, vexArray[x].originY, dist, vexArray[x].id, vexArray[x].height, vexArray[x].width);
  }

  for(int j = 0; j < 5; j++) {
    if(objectVector.at(j).idZ == OTYPE) {
      currentTrack = &objectVector.at(j);
      break;
    }
  }
}

void ObjectManager::printObjects(int VIEW) {
  for(auto x : objectVector) {
    x.printObject(VIEW);
  }
}

ObjectManager::ObjectManager(vex::brain::lcd *brainD) : objectVector(5, Objects(brainD)), currentTrack(nullptr) {}