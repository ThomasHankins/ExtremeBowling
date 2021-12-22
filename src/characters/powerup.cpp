#include "powerup.h"

PowerUp::PowerUp(float inX, float inY, float inZ, int type, int local_id) : Asset(inX, inY, inZ)
{
    this->physics.addCubeCollider(4, 0, 0, 0);
    this->physics.setInteraction(Reaction::ghost);
    this->physics.setId(type);
    this->physics.setLocalId(local_id);
    this->physics.addCallback(BALL, &hitBall, this);

    this->physics.setVelocity(0, 2, 0);
    this->startHeight = inY - 0.5;
    this->endHeight = inY + 3.5;

    this->collided = false;

    setGraphics();
}

void PowerUp::defaultAnimation(float time_ms)
{
    this->physics.addRotation(0, 1, 0, time_ms * 0.06);

    if (this->physics.getY() > this->endHeight)
        this->physics.setVelocity(0, -2, 0);
    else if (this->physics.getY() < this->startHeight)
        this->physics.setVelocity(0, 2, 0);

    vector<PhysicsObject3D*> empty;
    this->physics.updatePhysics(time_ms, false, empty);
}

int PowerUp::powerUpType()
{
    return this->physics.getId();
}

bool PowerUp::checkCollision(){
    return collided;
}

void PowerUp::setGraphics()
{
    this->obj_scalar = 1;
    this->graphics = Graphics("powerup");
    graphics.setMaterial(WHITE_RUBBER);
    switch (physics.getId())
    {
        case ADD_LIVES:
            graphics.setTexture(LIFE_TEXTURE);
            break;
        case HALF_SIZE:
            graphics.setTexture(SIZE_TEXTURE);
            break;
        case GHOST_MODE:
            graphics.setTexture(GHOST_TEXTURE);
            break;
    }
}

int PowerUp::hitBall(void* context, Vec3D deflection, void* obj)
{
    PowerUp* power = static_cast<PowerUp*>(context);
    
    power->collided = true;

    return 0;
}

vector<PowerUp*> PowerUp::fromJson(vector<json> jsonData, float tileSize)
{
    int counter = 0;
    vector<PowerUp*> powerups;
    for (json entry : jsonData)
    {
        float sX = (float)entry.find("col").value() * tileSize;
        float sZ = (float)entry.find("row").value() * tileSize;
        int pwType = entry.find("type").value();
        powerups.push_back(new PowerUp(sX, tileSize, sZ, pwType, counter));
        counter += 1;
    }
    return powerups;
}