#include "Objects.h"
#include "constants.h"
#include "v5.h"
#include "v5_vcs.h"
#include <unordered_map>
#include <vector>

class ObjectManager {
private:
  vex::brain::lcd screen;

public:
  std::vector<Objects> objectVector;
  std::vector<Objects> yellowObjects;
  std::vector<Objects> redObjects;
  std::vector<Objects> yellowAndRed;

  int redCounter;
  int yellowCounter;
  int yellowAndRedCounter;

  std::unordered_map<int, std::vector<Objects>> objectMap;

  Objects currentTrack;

  void sortObjects();
  void sortRed(Objects &red);
  void sortYellow(Objects &yellow);
  void sortYellowRed(Objects &redYellow);

  void setCurrentTrack(int OTYPE);
  void printObjects(int VIEW);
  void update(vex::safearray<vex::vision::object, 16> &vexArray, double dist, int OTYPE);
  ObjectManager(vex::brain::lcd *brainD); 
};