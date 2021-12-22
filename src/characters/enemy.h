#include <cstring>
#include <string>
#include "asset.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public Asset {
    protected:
        Point3D startPos;
        Point3D endPos;
        float speed;
        bool movingToEnd;
        bool collided;
    public:
        Enemy(float inX, float inY, float inZ, float endX, float endY, float endZ, int id);
        virtual void animate(float time);
        bool checkCollision();
};

#endif