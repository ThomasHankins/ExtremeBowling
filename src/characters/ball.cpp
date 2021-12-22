# include "ball.h"

Ball::Ball(float x, float y, float z, float radius): Asset(x, y, z)
{
    this->physics.setInteraction(Reaction::kinetic);
    this->physics.setSurfaceFriction(1);
    this->physics.setId(BALL);

    this->graphics = Graphics("ball", BLUE);
    this->graphics.setTexture(USER_TEXTURE);
    this->radius = radius;
    this->lives = 3;
    this->powerUpType = NO_POWERUP;
    this->blend = false;
    this->finishedStatus = false;
    this->onGround = false;

    this->lastCheckpoint = Point3D(x,y,z);
}

void Ball::init()
{
    this->physics.addSphereCollider(this->radius * 2, 0, 0, 0);
    
    this->physics.addCallback(CHECKPOINT, &hitCheckpoint, this);
    this->physics.addCallback(FINISH, &hitFinish, this);
    this->physics.addCallback(FLOOR, &hitFloor, this);
}

void Ball::runPhysics(float time, vector<PhysicsObject3D*> &world_objs)
{
    this->onGround -= time;
    this->physics.updatePhysics(time, true, world_objs);

    // rotate ball
    Vec3D rot_axis = Vec3D(0, 1, 0).crossProd(this->physics.getVel());
    this->physics.addRotation(rot_axis.x, 0, rot_axis.z, this->physics.getVel().length() * time * 0.18f / (M_PI * radius));
}

void Ball::activatePowerUp(PowerUp powerup)
{
    clearPowerUp();
    if(powerup.powerUpType() == ADD_LIVES){
        this->lives += 1;
        this->powerUpType = ADD_LIVES;

    }else if(powerup.powerUpType() == HALF_SIZE){
        this->powerUpType = HALF_SIZE;
        this->radius /= 2.0f;
        this->physics.setPosition(physics.getPos().x,physics.getPos().y, physics.getPos().z);
        this->physics.addSphereCollider(this->radius * 2, 0, 0, 0);
        this->obj_scalar /= 2.0f;

    }else if(powerup.powerUpType() == GHOST_MODE){
        this->powerUpType = GHOST_MODE;
        this->physics.setInteraction(Reaction::ghost);
        this->blend = true;
    }
}

void Ball::clearPowerUp(){

    if(this->powerUpType == HALF_SIZE){
        this->radius *= 2;
        this->physics.setPosition(physics.getPos().x,physics.getPos().y + this->radius/2, physics.getPos().z);
        this->physics.addSphereCollider(this->radius * 2, 0, 0, 0);
        this->obj_scalar *=2;
    }
    else if (this->powerUpType == GHOST_MODE)
    {
        this->physics.setInteraction(Reaction::kinetic);
        this->blend = false;
    }
    this->powerUpType = NO_POWERUP;
}


void Ball::jump(){
    if (this->onGround > 0)
    {
        this->physics.addVelocity(0,20,0);
        this->onGround = 0;
    }
}

bool Ball::respawn(){
    this->physics.setPosition(lastCheckpoint.x,lastCheckpoint.y,lastCheckpoint.z);
    this->physics.setVelocity(0,0,0);
    this->lives--;

    if(lives <= 0){
        return false;
    } else{
        return true;
    }

}

bool Ball::finished()
{
    return finishedStatus;
}


int Ball::getLives(){
    return this->lives;
}

int Ball::hitCheckpoint(void* context, Vec3D deflection, void* obj)
{
    Ball* b = static_cast<Ball*>(context);
    PhysicsObject3D* other = static_cast<PhysicsObject3D*>(obj);
    // set ball's last checkpoint to obj position
    b->lastCheckpoint.x = other->getX();
    b->lastCheckpoint.y = other->getY() + b->radius;
    b->lastCheckpoint.z = other->getZ();
    b->onGround = 125;
    return 1;
}

int Ball::hitFinish(void* context, Vec3D deflection, void* obj)
{
    Ball* b = static_cast<Ball*>(context);
    b->finishedStatus = true;
    b->onGround = 125;
    return 1;
}

int Ball::hitFloor(void* context, Vec3D deflection, void* obj)
{
    Ball* b = static_cast<Ball*>(context);
    b->onGround = 125;
    return 1;
}