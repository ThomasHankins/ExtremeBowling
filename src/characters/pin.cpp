#include "pin.h"

Pin::Pin(float inX, float inY, float inZ, int local_id) : Asset(inX, inY, inZ)
{
    this->graphics = Graphics("pin", WHITE_RUBBER);
    this->graphics.setTexture(PIN_TEXTURE);

    this->physics.addBoxCollider(4, 11, 4, 0, 2, 0);
    this->physics.setInteraction(Reaction::ghost);
    this->physics.setId(PIN);
    this->physics.setLocalId(local_id);

    this->physics.addCallback(BALL, &hitBall, this);
}

bool Pin::checkCollision(){
    return collided;
}

int Pin::hitBall(void* context, Vec3D deflection, void* obj)
{
    Pin* pin = static_cast<Pin*>(context);
    
    pin->collided = true;

    return 0;
}

vector<Pin*> Pin::fromJson(vector<json> jsonData, float tileSize)
{
    int counter = 0;
    vector<Pin*> pins;
    for (json entry : jsonData)
    {
        float sX = (float)entry.find("col").value() * tileSize;
        float sZ = (float)entry.find("row").value() * tileSize;
        pins.push_back(new Pin(sX, 3.75, sZ, counter));
        counter += 1;
    }
    return pins;
}