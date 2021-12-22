#include "sweeper.h"

Sweeper::Sweeper(float inX, float inY, float inZ, float endX, float endY, float endZ, int id) : Enemy(inX, inY, inZ, endX, endY, endZ, id)
{
    speed = 5;

    this->graphics = Graphics("sweeper", WHITE_RUBBER); 
    this->graphics.setTexture(SWEEPER_TEXTURE);

    this->physics.addBoxCollider(20, 8, 4, 0, 0, 2);
    this->physics.setId(SWEEPER);
    this->physics.setLocalId(id);

    this->physics.addCallback(BALL, &hitBall, this);

    Vec3D dir = Vec3D::createVector(startPos, endPos);
    this->physics.setRotation(0, 1, 0, atan2(dir.x, dir.z) * 180.0f / M_PI + 180.0f);
    this->obj_scalar = 6;
}

void Sweeper::animate(float time) 
{
    Point3D target = startPos;
    if(movingToEnd)
        target = endPos;
    
    if (physics.getPos().distanceTo(target) < 0.1)
        movingToEnd = !movingToEnd;

    Vec3D vel = Vec3D::createVector(physics.getPos(), target).normalize().multiply(speed);
    physics.setVelocity(vel.x, 0, vel.z);

    vector<PhysicsObject3D*> empty;
    physics.updatePhysics(time, false, empty);
}

int Sweeper::hitBall(void* context, Vec3D deflection, void* obj)
{
    Sweeper* e = static_cast<Sweeper*>(context);
    
    e->collided = true;
    
    deflection.y = 0;
    deflection = deflection.normalize().multiply(10);

    e->physics.addVelocity(-deflection.x, 0, -deflection.z);

    return 0;
}

vector<Sweeper*> Sweeper::fromJson(vector<json> jsonData, float tileSize)
{
    int counter = 0;
    vector<Sweeper*> sweepers;
    for (json entry : jsonData)
    {
        float sX = (float)entry.find("start col").value() * tileSize;
        float sZ = (float)entry.find("start row").value() * tileSize;
        float eX = (float)entry.find("end col").value() * tileSize;
        float eZ = (float)entry.find("end row").value() * tileSize;
        sweepers.push_back(new Sweeper(sX, tileSize, sZ, eX, tileSize, eZ, counter));
        counter += 1;
    }
    return sweepers;
}