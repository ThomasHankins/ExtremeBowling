#include "enemy.h"

#ifndef SWEEPER_H
#define SWEEPER_H

class Sweeper : public Enemy {
    public:
        Sweeper(float inX, float inY, float inZ, float endX, float endY, float endZ, int id);
        void animate(float time);
        static int hitBall(void* context, Vec3D deflection, void* obj);
        static vector<Sweeper*> fromJson(vector<json> jsonData, float tileSize);
};

#endif