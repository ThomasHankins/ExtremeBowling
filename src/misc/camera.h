#ifndef CAMERA_H
#define CAMERA_H

#include "mathlib_3D.h"

class Camera {
    private:
        float sensitivity;
        float distance;
        Point3D focus;
        Point3D pos;
        Rot3D rot;
        void updatePosition();
        Vec3D focusDirection();

    public:
        Camera();
        Camera(float startDistance);
        void orbit(int deltaX, int deltaY);
        void changeDistance(float direction);
        void changePosition(float x, float y, float z);
        Vec3D getForward();
        Vec3D getBackward();
        Vec3D getLeft();
        Vec3D getRight();

        float getX();
        float getY();
        float getZ();
};

#endif