#ifndef DIRECTION_CONTROLLER_H
#define DIRECTION_CONTROLLER_H

#include "mathlib_3D.h"

using namespace std;

class DirectionController {
    private:
        bool up;
        bool down;
        bool left;
        bool right;
        Vec3D forward;
        Vec3D rightward;
    public:
        DirectionController();
        void toggleUp(bool state);
        void toggleDown(bool state);
        void toggleLeft(bool state);
        void toggleRight(bool state);
        void setForward(Vec3D dir);
        void setRightward(Vec3D dir);
        Vec3D getDirection();
};
#endif