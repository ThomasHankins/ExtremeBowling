#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "../characters/boomba.h"
#include "../characters/pin.h"
#include "../characters/powerup.h"
#include "../characters/sweeper.h"
#include "../characters/ball.h"
#include "graphics.h"
#include "floor.h"


using namespace std;

class Level {
    private:
        string levelFilename;
        int highScore;

        vector <PowerUp*> powerUps;
        vector <Pin*> pins;
        vector <Boomba*> boombas;
        vector <Sweeper*> sweepers;

        Ball ball;
        Floor map; 
        
        vector<PhysicsObject3D *> worldObjects;
        
        int floorLength;

        int currentTime; //current time in ms
        
        int score;

        int powerUpStart;
        
        bool ended;

        void saveHighScore(); // called by endLevel() 
    public:

        
        Level();
        void init(string filename); 
        /*
            create a new ball object
            load map - call constructor
                pass json data through


            init enemies and powerups and pins

        */
        void runLevel(int timePassed);
            //run physics
            //update time
            //check power ups
            //check collisions/delete where appropriate
            //clear overdue powerups
            

        void displayAssets(bool hitbox);

        bool endLevel(); //return true if good ending, return false if bad ending

        /*
            called if ball.finished() = true;
            check/set high score
            delete all objects/vectors
            call main menu
            delete map

        */

        int getScore(); //calculates and returns score -- put in the corner of the level
        int getLives();
        int getTime();
        int getHighScore(); //get high score in json

        float getBallX();
        float getBallY();
        float getBallZ();
        void ballJump();
        void ballMove(Vec3D direction);
        void ballReset();

        bool getBlend();

        bool getEnded();
};

#endif