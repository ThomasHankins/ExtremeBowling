#include "enemy.h"

Enemy::Enemy(float inX, float inY, float inZ, float endX, float endY, float endZ, int id) : Asset(inX, inY, inZ)
{
    this->collided = false;
    this->physics.setLocalId(id);

    startPos = Point3D(inX, inY, inZ);
    endPos = Point3D(endX, endY, endZ);

    movingToEnd = true;
}

void Enemy::animate(float time) {}

bool Enemy::checkCollision(){
    return collided;
}