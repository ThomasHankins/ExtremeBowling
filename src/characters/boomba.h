#include "enemy.h"

#ifndef BOOMBA_H
#define BOOMBA_H

class Boomba : public Enemy {
    private:
        Vec3D deflect;
    public:
        Boomba(float inX, float inY, float inZ, float endX, float endY, float endZ, int id, float spd=3);

        void animate(float time);
        static int hitBall(void* context, Vec3D deflection, void* obj);
        static vector<Boomba*> fromJson(vector<json> jsonData, float tileSize);
};

#endif