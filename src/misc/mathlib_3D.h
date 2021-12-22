#ifndef MATHLIB_3D_H
#define MATHLIB_3D_H

#include <math.h> 

using namespace std;

class Dimension3 {
public:
    float x;
    float y;
    float z;
    Dimension3();
    Dimension3(float in_x, float in_y, float in_z);
};

class Point3D : public Dimension3 {
public:
    Point3D();
    Point3D(float in_x, float in_y, float in_z);


    float distanceTo(Point3D p);
    float fastDistanceTo(Point3D p);
    Point3D clone();
};

class Vec3D : public Dimension3{
public:
    Vec3D();
    Vec3D(float in_x, float in_y, float in_z);
    float length();
    float quickLength();
    Vec3D clone();
    Vec3D addVec(Vec3D p);
    Vec3D normalize();
    float dotProd(Vec3D v);
    Vec3D crossProd(Vec3D v);
    Vec3D project(Vec3D v);
    Vec3D multiply(float scalar);
    Point3D movePoint(Point3D source);

    static Vec3D createVector(Point3D p_1, Point3D p_2);
};

class Rot3D {
    public:
        float real;
        float x;
        float y;
        float z;

        Rot3D();
        Rot3D(float in_x, float in_y, float in_z, float in_a);

        float getAngle();
        float getX();
        float getY();
        float getZ();

        void rotate3D(Dimension3 *d);
        void addRotation(float in_x, float in_y, float in_z, float in_a, bool relative=false);

        static void rot3DAxisX(Dimension3 *d, float rad);
        static void rot3DAxisY(Dimension3 *d, float rad);
        static void rot3DAxisZ(Dimension3 *d, float rad);
    private:
        static void hamiltonProd(float a_r, float a_x, float a_y, float a_z, float b_r, float b_x, float b_y, float b_z, float * res);
};

#endif
