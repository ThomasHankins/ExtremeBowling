#include "camera.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>

Camera::Camera(){
    this->sensitivity = 180.0f / 400.0f;
    this->distance = 50;

    this->focus = Point3D();
    this->pos = Point3D();

    this->rot = Rot3D();

    updatePosition();
}

Camera::Camera(float startDistance){
    this->sensitivity = 180.0f / 400.0f;   // degrees to pixel ratio (how many pixels do you want to move to rotate 180)
    this->distance = startDistance;

    this->focus = Point3D();
    this->pos = Point3D();

    this->rot = Rot3D();
    this->rot.addRotation(1, 0, 0, -30);
    this->rot.addRotation(0, 1, 0, 180);
    updatePosition();
}


void Camera::updatePosition(){
    this->pos = Point3D(0, 0, this->distance);
    this->rot.rotate3D(&this->pos);
    this->pos = Vec3D(this->focus.x, this->focus.y, this->focus.z).movePoint(this->pos);
}

Vec3D Camera::focusDirection()
{
    return Vec3D::createVector(pos, focus).normalize();
}

void Camera::changeDistance(float direction){
    this->distance+=direction;
    this->distance = max(this->distance, 1.0f);
    updatePosition();
}

void Camera::orbit(int deltaX, int deltaY)
{
    if (deltaY != 0)
    {
        deltaY = -deltaY;
        Vec3D axis = focusDirection().crossProd(Vec3D(0, -1, 0));
        Point3D testPos = Point3D(0,0,1);
        rot.rotate3D(&testPos);
        float currentAngle = asin(testPos.y) * 180.0f / M_PI;
        if (currentAngle + deltaY * this->sensitivity > 85)         // vertical upward limit
            rot.addRotation(axis.x, axis.y, axis.z, 85 - currentAngle);
        else if (currentAngle + deltaY * this->sensitivity < 2)     // vertical downward limit
            rot.addRotation(axis.x, axis.y, axis.z, 2 - currentAngle);
        else
            rot.addRotation(axis.x, axis.y, axis.z, deltaY * this->sensitivity);
    }

    if (deltaX != 0)
        rot.addRotation(0, 1, 0, deltaX * this->sensitivity);
    
    updatePosition();
}

void Camera::changePosition(float x, float y, float z){
    this->focus = Point3D(x, y, z);
    this->updatePosition();
}

Vec3D Camera::getForward()
{
    return Vec3D(this->focus.x - this->pos.x, 0, this->focus.z - this->pos.z).normalize();
}

Vec3D Camera::getBackward()
{
    return Vec3D(this->pos.x - this->focus.x, 0, this->pos.z - this->focus.z).normalize();
}

Vec3D Camera::getLeft()
{
    return Vec3D(0,1,0).crossProd(getForward());
}

Vec3D Camera::getRight()
{
    return Vec3D(0,1,0).crossProd(getBackward());
}

float Camera::getX(){
    return this->pos.x;
}

float Camera::getY(){
    return this->pos.y;
}

float Camera::getZ(){
    return this->pos.z;
}
