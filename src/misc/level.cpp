#include "level.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

Level::Level() {}

void Level::init(string filename){

    levelFilename = filename;
    ifstream f(levelFilename);

    if (f.is_open())
    {
        json level_data;
        f >> level_data;
        f.close();

        this->map = Floor::fromJson(level_data.find("Floor").value());
        map.getPointers(this->worldObjects);
        this->floorLength = worldObjects.size();

    
        json enemies_data = level_data.find("Enemies").value();

        vector<json> boombas_data = enemies_data.find("Boombas").value();
        this->boombas = Boomba::fromJson(boombas_data, this->map.getTileSize());
        for (Boomba *i : boombas)
        {
            this->worldObjects.push_back(i->getPhysicsPointer());
        }

        vector<json> sweepers_data = enemies_data.find("Sweepers").value();
        this->sweepers = Sweeper::fromJson(sweepers_data, this->map.getTileSize());
        for(Sweeper *i: sweepers){
            this->worldObjects.push_back(i->getPhysicsPointer());
        }

        vector<json> pins_data = level_data.find("Pins").value();
        this->pins = Pin::fromJson(pins_data, this->map.getTileSize());
        for(Pin *i: pins){
            this->worldObjects.push_back(i->getPhysicsPointer());
        }

        vector<json> powerups_data = level_data.find("PowerUps").value();
        this->powerUps = PowerUp::fromJson(powerups_data, this->map.getTileSize());
        for (PowerUp *i : powerUps)
        {
            this->worldObjects.push_back(i->getPhysicsPointer());
        }

        this->ball = Ball(this->map.spawnX(),this->map.spawnY(), this->map.spawnZ(), 5);
        this->ball.init();

        highScore = level_data.find("HighScore").value();
    }
    

    this->currentTime = 0;

    this->ended = false;
    this->score = 0;
    this->powerUpStart = 0;

    
}

void Level::runLevel(int timePassed){
    this->ball.runPhysics(timePassed, worldObjects); //time passed will be the same argument as is previously called, need chandler to advise

    this->currentTime += timePassed;

    
    for(Sweeper *i : sweepers){
        i->animate(timePassed);
    }

    for(Boomba *i : boombas){
        i->animate(timePassed);
    }

    for(PowerUp *i : powerUps){
        i->defaultAnimation(timePassed);
    }

    vector<vector<PowerUp*>::iterator> deletePowerUps;
    for (vector<PowerUp*>::iterator it = powerUps.begin(); it != powerUps.end(); it++)
    {
        if((*it)->checkCollision() == true){
            this->powerUpStart = currentTime;
            this->ball.activatePowerUp(*(*it));

            int localID = (*it)->getPhysicsPointer()->getLocalId();
            int objectID = (*it)->getPhysicsPointer()->getId();
            
            this->worldObjects.erase(std::remove_if(
            this->worldObjects.begin() + this->floorLength, this->worldObjects.end(), //start at number of tiles
            [localID, objectID](PhysicsObject3D * &j){
                return (j->getLocalId() == localID && j->getId() == objectID);
            }), this->worldObjects.end());

            // delete object
            delete *it;

            // remove from powerUps
            deletePowerUps.push_back(it);
        }
    }
    reverse(deletePowerUps.begin(), deletePowerUps.end());
    for (vector<PowerUp*>::iterator it : deletePowerUps)
    {
        powerUps.erase(it);
    }

    vector<vector<Pin*>::iterator> deletePins;
    for (vector<Pin*>::iterator it = pins.begin(); it != pins.end(); it++)
    {
        if((*it)->checkCollision() == true){
            this->score += 1;

            int localID = (*it)->getPhysicsPointer()->getLocalId();
            int objectID = (*it)->getPhysicsPointer()->getId();
            
            this->worldObjects.erase(std::remove_if(
            this->worldObjects.begin() + this->floorLength, this->worldObjects.end(), //start at number of tiles
            [localID, objectID](PhysicsObject3D * &j){
                return (j->getLocalId() == localID && j->getId() == objectID);
            }), this->worldObjects.end());

            // delete object
            delete *it;

            // remove from powerUps
            deletePins.push_back(it);
        }
    }
    reverse(deletePins.begin(), deletePins.end());
    for (vector<Pin*>::iterator it : deletePins)
    {
        pins.erase(it);
    }

    //clearing powerups
    if(currentTime - powerUpStart > 10000){
        ball.clearPowerUp();
    }

    //check ball position then respawns
    if(this->ball.getY() < -20){ // can adjust based on preferences
        if(!ball.respawn()){
            this->ended = true;
        }
    }

    // check if ball reached the finish
    if (this->ball.finished())
        this->ended = true;
}

void Level::displayAssets(bool hitbox){
    float lightPos[] ={ ball.getX(), 50, ball.getZ() - 5, 1 };
    float lightAmb[] = { 1, 1, 1, 1 };
    float lightDif[] = { 0.8, 0.8, 0.8, 1 };
    float lightSpc[] = { 1, 1, 1, 1 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpc);
    

    for(Boomba* i : boombas){
        i->displayAsset(hitbox);
    }

    for(Sweeper* i : sweepers){
        i->displayAsset(hitbox);
    }
    for(Pin* i : pins){
        i->displayAsset(hitbox);
    }


    for(PowerUp* i : powerUps){
        i->displayAsset(hitbox);
    }

    this->map.displayFloor(hitbox);


    if (ball.blend) 
    {
        glEnable(GL_BLEND);					// Turn Blending On
    }


    this->ball.displayAsset(hitbox);
    
    glDisable(GL_BLEND);					// Turn Blending Off
	glEnable(GL_DEPTH_TEST);				// Turn Depth Testing On
    
}

int Level::getScore(){
    return (score*100 - currentTime/1000 + 1000) * (ball.getLives() > 0);
}

int Level::getTime(){
    return this->currentTime;
}

int Level::getLives(){
    return ball.getLives();
}

bool Level::getEnded()
{
    return ended;
}

bool Level::endLevel(){
    int lives = ball.getLives();

    for(Boomba* i : boombas)
        delete i;
    for(Sweeper* i : sweepers)
        delete i;
    for(PowerUp* i : powerUps)
        delete i;
    for(Pin* i : pins)
        delete i;

    this->boombas.clear();
    this->sweepers.clear();
    this->powerUps.clear();
    this->pins.clear();
    this->map.clearTiles();   

    if(this->getScore() > this->highScore){
        this->saveHighScore();
    }

    if(lives == 0){
        return false;
    } else {
        return true;
    }
    
    
}
int Level::getHighScore(){
    return this->highScore;
}

void Level::saveHighScore()
{
    // read JSON file
    std::ifstream i(levelFilename);
    json j;
    i >> j;
    i.close();

    j["HighScore"] = getScore();
    // write prettified JSON to another file
    std::ofstream o(levelFilename);
    o << std::setw(4) << j << std::endl;
    o.close();
}

//gameplay methods
float Level::getBallX(){
    return this->ball.getX();
}

float Level::getBallY(){
    return this->ball.getY();
}

float Level::getBallZ(){
    return this->ball.getZ();
}

void Level::ballJump(){
    this->ball.jump();
}

void Level::ballMove(Vec3D direction){
    int mult = 10;

    Vec3D acc = direction.multiply(mult);
    
    ball.accelerate(acc.x, acc.y, acc.z);
}

void Level::ballReset()
{
    if(!ball.respawn()){
        this->ended = true;
    }
}

bool Level::getBlend(){
    return this->ball.blend;
}
