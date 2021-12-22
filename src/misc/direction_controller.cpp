#include "direction_controller.h"

DirectionController::DirectionController()
{
    this->up = false;
    this->down = false;
    this->left = false;
    this->right = false;
    this->forward = Vec3D();
    this->rightward = Vec3D();
}

void DirectionController::toggleUp(bool state)
{
    this->up = state;
}

void DirectionController::toggleDown(bool state)
{
    this->down = state;
}

void DirectionController::toggleLeft(bool state)
{
    this->left = state;
}

void DirectionController::toggleRight(bool state)
{
    this->right = state;
}

void DirectionController::setForward(Vec3D dir)
{
    this->forward = dir.normalize();
}

void DirectionController::setRightward(Vec3D dir)
{
    this->rightward = dir.normalize();
}

Vec3D DirectionController::getDirection()
{
    Vec3D dir;

    if (this->up)
        dir = dir.addVec(this->forward);
    
    if (this->down)
        dir = dir.addVec(this->forward.multiply(-1));
    
    if (this->left)
        dir = dir.addVec(this->rightward.multiply(-1));
    
    if (this->right)
        dir = dir.addVec(this->rightward);
    
    return dir.normalize();
}
