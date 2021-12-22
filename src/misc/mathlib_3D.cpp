#include "mathlib_3D.h"

Dimension3::Dimension3()
{
    x = 0;
    y = 0;
    z = 0;
}

Dimension3::Dimension3(float in_x, float in_y, float in_z)
{
    x = in_x;
    y = in_y;
    z = in_z;
}

Point3D::Point3D()
{
}

Point3D::Point3D(float in_x, float in_y, float in_z):Dimension3(in_x, in_y, in_z)
{
}

float Point3D::distanceTo(Point3D p)
{
    return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z));
}

float Point3D::fastDistanceTo(Point3D p)
{
    return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z);
}

Point3D Point3D::clone()
{
    return Point3D(x, y, z);
}

Vec3D::Vec3D()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3D::Vec3D(float in_x, float in_y, float in_z)
{
    x = in_x;
    y = in_y;
    z = in_z;
}

float Vec3D::length()
{
    return sqrt(x * x + y * y + z * z);
}

float Vec3D::quickLength()
{
    return x * x + y * y + z * z;
}

Vec3D Vec3D::clone()
{
    return Vec3D(x, y, z);
}

Vec3D Vec3D::addVec(Vec3D p)
{
    return Vec3D(x + p.x, y + p.y, z + p.z);
}

Vec3D Vec3D::normalize()
{
    float len = length();
    if (len == 0)
        return Vec3D();
    return Vec3D(x / len, y / len, z / len);
}

float Vec3D::dotProd(Vec3D v)
{
    return x * v.x + y * v.y + z * v.z;
}

Vec3D Vec3D::crossProd(Vec3D v)
{
    float c_x = y * v.z - z * v.y;
    float c_y = z * v.x - x * v.z;
    float c_z = x * v.y - y * v.x;

    return Vec3D(c_x, c_y, c_z);
}

// projects vector v onto this vector
Vec3D Vec3D::project(Vec3D v)
{
    return multiply(dotProd(v) / dotProd(*this));
}

Vec3D Vec3D::multiply(float scalar)
{
    return Vec3D(x * scalar, y * scalar, z * scalar);
}

Vec3D Vec3D::createVector(Point3D p_1, Point3D p_2)
{
    return Vec3D(p_2.x - p_1.x, p_2.y - p_1.y, p_2.z - p_1.z);
}

Point3D Vec3D::movePoint(Point3D source)
{
    return Point3D(source.x + x, source.y + y, source.z + z);
}

/* Rot3D
 * Stores rotation angles for x, y, z
 * Rotations are done in the order: x, y, z
 * Each rotation is done a its relative axis.
 * ie. done in the same way as
*/
Rot3D::Rot3D()
{
    real = 1;
    x = 0;
    y = 0;
    z = 0;
}

Rot3D::Rot3D(float in_x, float in_y, float in_z, float in_a)
{
    float angle = in_a * M_PI / 360.0f; // convert to rad and divide by 2
    Vec3D axis = Vec3D(in_x, in_y, in_z).normalize();
    real = cos(angle);
    x = sin(angle) * axis.x;
    y = sin(angle) * axis.y;
    z = sin(angle) * axis.z;
}

float Rot3D::getAngle()
{
    return acos(real) * 360.0f / M_PI;
}

float Rot3D::getX()
{
    return x;
}

float Rot3D::getY()
{
    return y;
}

float Rot3D::getZ()
{
    return z;
}

void Rot3D::rotate3D(Dimension3 *d)
{
    float result[4];

    hamiltonProd(real, x, y, z, 0.0f, d->x, d->y, d->z, result);
    hamiltonProd(result[0], result[1], result[2], result[3], real, -x, -y, -z, result);

    d->x = result[1];
    d->y = result[2];
    d->z = result[3];
}

void Rot3D::addRotation(float in_x, float in_y, float in_z, float in_a, bool relative)
{
    float angle = in_a * M_PI / 360.0f; // convert to rad and divide by 2
    Vec3D axis = Vec3D(in_x, in_y, in_z).normalize();
    float a_r = cos(angle);
    float a_x = sin(angle) * axis.x;
    float a_y = sin(angle) * axis.y;
    float a_z = sin(angle) * axis.z;

    float result[4];
    
    // rotate relative to world orientation or relative to object's current rotation
    if (relative)
        hamiltonProd(real, x, y, z, a_r, a_x, a_y, a_z, result);
    else
        hamiltonProd(a_r, a_x, a_y, a_z, real, x, y, z, result);

    real = result[0];
    x = result[1];
    y = result[2];
    z = result[3];
}

void Rot3D::rot3DAxisX(Dimension3 *d, float rad)
{
    float y = d->y;
    float z = d->z;
    d->y = cos(rad) * y - sin(rad) * z;
    d->z = sin(rad) * y + cos(rad) * z;
}
void Rot3D::rot3DAxisY(Dimension3 *d, float rad)
{
    float x = d->x;
    float z = d->z;
    d->x = cos(rad) * x + sin(rad) * z;
    d->z = -sin(rad) * x + cos(rad) * z;
}
void Rot3D::rot3DAxisZ(Dimension3 *d, float rad)
{
    float x = d->x;
    float y = d->y;
    d->x = cos(rad) * x - sin(rad) * y;
    d->y = sin(rad) * x + cos(rad) * y;
}

void Rot3D::hamiltonProd(float a_r, float a_x, float a_y, float a_z, float b_r, float b_x, float b_y, float b_z, float * res)
{
    res[0] = a_r * b_r - a_x * b_x - a_y * b_y - a_z * b_z;
    res[1] = a_r * b_x + a_x * b_r + a_y * b_z - a_z * b_y;
    res[2] = a_r * b_y - a_x * b_z + a_y * b_r + a_z * b_x;
    res[3] = a_r * b_z + a_x * b_y - a_y * b_x + a_z * b_r;
}