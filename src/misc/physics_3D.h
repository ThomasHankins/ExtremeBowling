#ifndef PHYSICS_3D_H
#define PHYSICS_3D_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <vector>

#include "mathlib_3D.h"

#define GRAVITY (-18.6f)
#define MAX_SPD 1000000
#define ZERO 0.0000001

using namespace std;

enum class Shape {
    box, 
    sphere,
    none
};

enum class Reaction {
    kinetic = 0, 
    fixed,
    ghost
};

struct Collision
{
    int id;
    Vec3D deflection;
};

struct Callback
{
    int(*f)(void*, Vec3D, void*);
    void* context;
    Callback(int(*in_f)(void*, Vec3D, void*), void* in_context) : f(in_f), context(in_context) {}
    int runFunction(Vec3D v, void* obj) { return f(context, v, obj); }
};

class Collider3D {
public:
    Shape collider_type;
    Point3D *p_pos;
    Rot3D *p_rot;
    float size_x;
    float size_y;
    float size_z;
    Vec3D offset;
    Point3D old_pos;
    Collider3D();
    Collider3D(Shape col, Point3D *p_position, Rot3D *p_rotation, float s_x, float s_y, float s_z, float off_x, float off_y, float off_z);
    Point3D calculatePos();
    Vec3D collide(Collider3D col);
private:
    static Vec3D collisionPointSphere(Point3D p, Point3D sph_p, float rad);
    static Vec3D collisionCornerSphere(Point3D corner, Vec3D edge_x, Vec3D edge_y, Vec3D edge_z, Point3D sph_p, float rad);
    static Vec3D collisionBoxBox(Collider3D box_1, Collider3D box_2);
    static Vec3D collisionBoxSphere(Collider3D box, Collider3D sph);
    static Vec3D collisionSphereSphere(Collider3D sph_1, Collider3D sph_2);
};

class PhysicsObject3D{
private:
    Point3D pos;
    Vec3D vel;
    Vec3D acc;
    Rot3D rot;
    float rot_vel;
    Reaction interaction;

    float surface_friction;
    float acc_friction;
    
    int id;
    int local_id;
    
    vector<Collision> collided;       // when an object collides with another object, they can add some value to collided to know what they collided with 
    map<int, Callback> callbacks;
public:
    Collider3D collider;
    PhysicsObject3D();
    PhysicsObject3D(float p_x, float p_y, float p_z, int i = 1, float f = 0);

    // getters
    float getX();
    float getY();
    float getZ();
    Point3D getPos();
    Vec3D getVel();
    Rot3D getRot();
    Reaction getInteraction();
    bool isMoveable();
    float getSurfaceFriction();
    int getId();
    int getLocalId();
    vector<Collision> getCollided();

    // setters
    void setId(int object_id);
    void setLocalId(int object_id);
    void setPosition(float x, float y, float z);
    void setVelocity(Vec3D v);
    void setVelocity(float x, float y, float z);
    void setRotation(float x, float y, float z, float a);
    void addRotation(float x, float y, float z, float a);
    void addRelativeRotation(float x, float y, float z, float a);

    void setRotVel(float r_v);

    void setInteraction(Reaction r);
    void setSurfaceFriction(float f);
    void setAccFriction(float f);

    void addBoxCollider(float s_x, float s_y, float s_z, float off_x, float off_y, float off_z);
    void addCubeCollider(float size, float off_x, float off_y, float off_z);
    void addSphereCollider(float size, float off_x, float off_y, float off_z);

    void addCallback(int i, int(*f)(void*, Vec3D, void*), void* context);
    void removeCallback(int i);

    // modifiers
    void addVelocity(float x, float y, float z);
    void addAcceleration(float x, float y, float z);
    void updatePhysics(float time, bool gravity, vector<PhysicsObject3D *> &objs);
    
    void reflect(Vec3D ref_normal, float scale);
    
    void collision(PhysicsObject3D *other_obj);
    int runCallback(int id, Vec3D deflection, void* obj);
};





#endif