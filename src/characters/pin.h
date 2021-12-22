#include "asset.h"

#ifndef PIN_H
#define PIN_H

/* 
Pins will spawn in certain areas of the map, and will not respawn unless
the map is reset. 
*/

class Pin : public Asset {
    private:
        bool collided;
    public:
        Pin(float inX, float inY, float inZ, int local_id);
        bool checkCollision();
        static int hitBall(void* context, Vec3D deflection, void* obj);
        static vector<Pin*> fromJson(vector<json> jsonData, float tileSize);
};

#endif