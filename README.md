# Autonomous-Clawbot

## Classes

### Objects

This class contains all of the information about an object gathered from the Vision sensor, functions to output those properties to the screen, and calculations for navigating towards the object

### ActionManager

This class handles all of action events and motor outputs to control the robot. The event handling is done by setting `int` variables that communicate event changes by their value.

### ObjectManager

The ObjectManager class is responsible for handling the output of the `Vision.objects` array and determing which object to track based on the ActionManager

## Important Information

All commits after the second one are untested.

If using first commit [4571a9b](https://github.com/adrianjselva/Autonomous-Clawbot/commit/4571a9b90e173ba4116d6d3df7c6af577512a91f), change `isInBoundDistance = false` in line 7 of src/Objects.cpp to ```isInBoundVision = false```

If using second commit [14c262e](https://github.com/adrianjselva/Autonomous-Clawbot/commit/14c262ede51f8c1480aa494d0f114e8e9910a16e), change `isInBoundDistance = false` in line 19 of src/Objects.cpp to ```isInBoundVision = false```
