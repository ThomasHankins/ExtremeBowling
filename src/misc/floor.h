#ifndef FLOOR_H
#define FLOOR_H

#include "physics_3D.h"
#include "../characters/asset.h"
#include "../characters/character_ids.h"

#include "../third_party/json.hpp"

using namespace std;
using json = nlohmann::json;

class Tile : public Asset {
    public:
        Tile();
        Tile(float x, float y, float z, float size, float friction, int tile_type, float r_a = 0, float r_x = 0, float r_y = 0, float r_z = 0);
        void setGraphics();
        static int hitBall(void* context, Vec3D deflection, void* obj);
};

class Floor {
    private:
        vector<Tile*> floor_tiles;
        float size;
        Point3D pos;
        Point3D spawn;
    public:
        Floor();
        Floor(vector<string> csv, float tile_size, float friction, float x, float z);
        static Floor fromJson(json floor_json);
        void getPointers(vector<PhysicsObject3D *> & p_tiles);
        float spawnX();
        float spawnY();
        float spawnZ();
        float getTileSize();
        void displayFloor(bool hitbox);
        void clearTiles();
};

#endif