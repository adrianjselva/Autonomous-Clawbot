#include "Objects.h"
#include "constants.h"
#include "v5.h"
#include "v5_vcs.h"
#include <vector>

class ObjectManager {
private:
  std::vector<Objects> objectVector;
public:
  Objects *currentTrack;
  Objects emptyObject;

  void printObjects(int VIEW);
  void update(vex::safearray<vex::vision::object, 16> &vexArray, double dist, int OTYPE);

  ObjectManager(vex::brain::lcd *brainD); 
};