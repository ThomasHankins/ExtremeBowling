# CS 3GC3 - Project

To run the file: enter make command in the local directory. This will compile and run automatically.
To clean and recompile, type make clean followed by make again.

## Features Implemented

* Lighting 
    - Local lighting following the ball

* Textures and materials
    - Added for all 3D objects, including character class, boombas, sweepers, pins, powerups, and floor

* Alpha blending
    - Implemented for objects during character class "ghost" mode 

* Animated characters
    - Sweepers and boombas have default movements in map
    - Power ups have a "floating" animation
    - Ball rolls based on its movement

* Non-geometric primitives 
    - Start menu, instructions menu, pause menu, end menu
        - Clickable buttons to navigate between menus
    - HUD interface

* Advanced camera control
    - Complex camera which moves around ball based on mouse movement
    - Quaternion-based camera

* Shaders
    - Smooth shading

* OBJ file parsing and drawing
    - Done for all 3D objects

* Character class - ball.h/ball.cpp
    - Contains physics values (position, bounding box, etc), loaded obj, controls, active power-up
    - Keeps track of last checkpoint

* Pin class:
    - Have physics values (position, bounding box, etc)
    - Disappear after collision with character class
    - Collecting pins increases score

* Power up class:
    - Have physics values (position, bounding box, etc)
    - Disappear after collision with character class
    - Three types of power ups:
        - Extra life
        - Halve size of player 
        - Make player invulnerable to enemies
    - Have floating animation

* Enemy classes (sweepers, boombas):
    - Follow predetermined path
    - Collide with player
    - Boombas themselves also get bounced back after collision

* Physics
    - Values including position, velocity, acceleration etc.
    - Bounding boxes and collision detection between boxes and spheres
    - Calculates deflections from collisions

* Advanced world
    - Floor loaded from CSV data
    - Moveable character class with acceleration and jump
    - Enemies spawned and have animation
    - 3 different maps/ levels

* Terrain generation
    - JSON file containing data for all world entities
    - Terrain tiles have position, friction and can be angled
    - Terrain also has checkpoints and finish tile

* File IO
    - Level details are stored in JSON files: 
        - Terrain generation with CSV
        - Spawn locations
        - High score
    
Running make will compile and run the program.

# Controls
| Command  | Description  |
|---|---|
| up arrow or w | ball moves forward |
| down arrow or s | ball moves backward  |
| left arrow or a | ball moves left |
| right arrow or d | ball moves right  |
| left-click  | ball jumps  |   
| space  | game pauses  |
| mouse is outside window | game pauses  |
| moving mouse | camera moves around ball|
| ` | toggle FPS display |
| r | reset ball position |
