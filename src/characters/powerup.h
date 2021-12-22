#include "asset.h"
#include <string>

#ifndef POWERUP_H
#define POWERUP_H

/*
Power ups will spawn in certain areas of the map, and will not respawn unless
the map is reset. They will have a similar look to the Mario kart power up boxes. 
*/

class PowerUp : public Asset {
    private:
        bool collided;
        float startHeight;
        float endHeight;
    public:
        PowerUp(float inX, float inY, float inZ, int type, int local_id);
        void defaultAnimation(float time_ms); // box slowly rotates and hovers up and down
        int powerUpType(); // will let player class know which type of power up to apply
        using Asset::Asset;
        bool checkCollision();
        void setGraphics();
        static int hitBall(void* context, Vec3D deflection, void* obj);
        static vector<PowerUp*> fromJson(vector<json> jsonData, float tileSize);
};

#endif