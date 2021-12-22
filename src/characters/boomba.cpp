#include "boomba.h"
#define BOOMBA_RADIUS 2
Boomba::Boomba(float inX, float inY, float inZ, float endX, float endY, float endZ, int id, float spd) : Enemy(inX, inY, inZ, endX, endY, endZ, id)
{
    this->speed = spd;

    // initial rotation
    this->physics.setRotation(0, 1, 0, -90); 
    this->physics.addSphereCollider(BOOMBA_RADIUS * 2, 0, 0, 0);
    this->physics.setId(BOOMBA);
    this->physics.setLocalId(id);

    this->physics.addCallback(BALL, &hitBall, this);

    this->graphics = Graphics("ball", RUBY);
    this->graphics.setTexture(BOOMBA_TEXTURE);
    this->obj_scalar = BOOMBA_RADIUS / 5.0f;
}

void Boomba::animate(float time)
{
    if (collided)
    {
        // after being hit, set a deflection velocity
        if(deflect.length() > 0)
            physics.setVelocity(deflect.x, 0, deflect.z);
        // after the initial impact, decrease speed with friction
        else if (physics.getVel().length() > speed * 2)
            physics.setAccFriction(0.5);
        // when speed has been sufficiently decreased, set collided false and move normally
        else
            collided = false;

        deflect = Vec3D();
    }
    else
    {
        // choose target to move towards
        Point3D target = startPos;
        if(movingToEnd)
            target = endPos;
        
        // switch target after reaching it
        if (physics.getPos().distanceTo(target) < 0.5)
            movingToEnd = !movingToEnd;

        // set velocity dynamically based on current position and target
        Vec3D vel = Vec3D::createVector(physics.getPos(), target).normalize().multiply(speed);
        physics.setVelocity(vel.x, vel.y, vel.z);
    }

    // run physics
    vector<PhysicsObject3D*> empty;
    physics.updatePhysics(time, false, empty);

    // for rotation: if enemy is another bowling ball, use this. otherwise, use above section
    Vec3D rot_axis = Vec3D(0, 1, 0).crossProd(this->physics.getVel());
    physics.addRotation(rot_axis.x, 0, rot_axis.z, this->physics.getVel().length()* time * 0.02865f);
}

int Boomba::hitBall(void* context, Vec3D deflection, void* obj)
{
    Boomba* e = static_cast<Boomba*>(context);
    PhysicsObject3D* ball = static_cast<PhysicsObject3D*>(obj);
    
    // dont deflect if ball is in ghost mode
    if (ball->getInteraction() == Reaction::ghost)
        return 0;
    
    deflection.y = 0;
    deflection = deflection.normalize().multiply(10);

    e->collided = true;
    e->deflect = deflection;
    e->physics.addVelocity(-e->deflect.x/3.0f, 0, -e->deflect.z/3.0f);

    return 0;
}

vector<Boomba*> Boomba::fromJson(vector<json> jsonData, float tileSize)
{
    int counter = 0;
    vector<Boomba*> boombas;
    for (json entry : jsonData)
    {
        float sX = (float)entry.find("start col").value() * tileSize;
        float sZ = (float)entry.find("start row").value() * tileSize;
        float eX = (float)entry.find("end col").value() * tileSize;
        float eZ = (float)entry.find("end row").value() * tileSize;
        float spd = (float)entry.find("speed").value();
        boombas.push_back(new Boomba(sX, BOOMBA_RADIUS, sZ, eX, BOOMBA_RADIUS, eZ, counter, spd));
        counter += 1;
    }
    return boombas;
}