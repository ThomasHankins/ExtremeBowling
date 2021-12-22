#include "floor.h"

#include <string>
#include <vector>

#define SPAWN_HEIGHT 10

vector<string> split(string str, string delimiter=" ")
{
    vector<string> words;

    size_t next = 0;
    size_t prev = 0;

    while ((next = str.find(delimiter, prev)) != string::npos)
    {
        words.push_back(str.substr(prev, next - prev));
        prev = next + 1;
    }
    words.push_back(str.substr(prev, string::npos));

    return words;
}

Tile::Tile(): Asset() {}

Tile::Tile(float x, float y, float z, float size, float friction, int tile_type, float r_a, float r_x, float r_y, float r_z):Asset(x, y, z)
{
    physics.setRotation(r_x, r_y, r_z, r_a);
    physics.setSurfaceFriction(friction);
    physics.addBoxCollider(size, 0.1, size, 0, 0, 0);
    physics.setId(tile_type);
    
    if (tile_type == CHECKPOINT)
        physics.addCallback(BALL, &hitBall, this);

    setGraphics();
}

void Tile::setGraphics()
{
    this->obj_scalar = 1;
    graphics = Graphics("floor");
    this->graphics.setTexture(WOOD_TEXTURE);
    switch (physics.getId())
    {
        case CHECKPOINT:
            // set graphics to be checkpoint tile 
            graphics.setMaterial(EMERALD);
            break;
        case FINISH:
            // set graphics to be checkpoint tile 
            this->graphics.setTexture(FINISH_TEXTURE);
            graphics.setMaterial(WHITE_RUBBER);
            break;
        default:
            // set graphics to be normal tile
            graphics.setMaterial(BRONZE);
            break;
    }
}

int Tile::hitBall(void* context, Vec3D deflection, void* obj)
{
    Tile* t = static_cast<Tile*>(context);
    // disable the checkpoint so that it doesnt keep getting reset
    // It also prevents someone from going backwards in the map and beign reset to an earlier checkpoint
    t->physics.setId(FLOOR);
    t->physics.removeCallback(BALL);
    t->setGraphics();

    return 0;
}

Floor::Floor()
{
    pos = Point3D();
    size = 1;
}

Floor::Floor(vector<string> csv, float tile_size, float friction, float x, float z)
{
    pos = Point3D(x, 0, z);
    size = tile_size;

    vector<string> str_row;
    vector<string> values;
    string tile;
    string c;
    vector<string> r;

    int row = 0;
    int col = 0;

    float t_x;
    float t_y;
    float t_z;

    int tile_type;
    for (string line : csv)
    {
        str_row = split(line, ",");
        col = 0;
        for (string tile : str_row)
        {
            t_x = x + size * col;
            t_y = 0;
            t_z = z + size * row;
            
            values = split(tile, "|");

            c = values.at(0);
            if (c.compare("0") != 0)
            {
                tile_type = FLOOR;
                if (c.compare("S") == 0)
                {
                    spawn = Point3D(t_x, SPAWN_HEIGHT, t_z);
                    tile_type = CHECKPOINT;
                }
                else if (c.compare("C") == 0)
                    tile_type = CHECKPOINT;
                else if (c.compare("F") == 0)
                    tile_type = FINISH;
                
                c = values.at(1);
                if (c.compare(" ") != 0)
                    t_y = stof(c);
                
                c = values.at(2);
                if (c.compare(" ") == 0)
                    floor_tiles.push_back(new Tile(t_x, t_y, t_z, size, friction, tile_type));
                else
                {
                    r = split(c, "*");
                    floor_tiles.push_back(new Tile(t_x, t_y, t_z, size, friction, tile_type, stof(r.at(0)), stof(r.at(1)), stof(r.at(2)), stof(r.at(3)) ));
                }
                    
            }

            col++;
        }
        row++;
    }
}

Floor Floor::fromJson(json floor_json)
{
    float default_friction = floor_json.find("DEFAULT_FRICTION").value();
    float tile_size = floor_json.find("TILE_SIZE").value();
    vector<string> csv = floor_json.find("CSV").value();

    return Floor(csv, tile_size, default_friction, 0, 0);
}

void Floor::getPointers(vector<PhysicsObject3D *> & p_tiles)
{

    for (Tile *t : floor_tiles)
        p_tiles.push_back(t->getPhysicsPointer());

}

float Floor::spawnX()
{
    return spawn.x;
}

float Floor::spawnY()
{
    return spawn.y;
}

float Floor::spawnZ()
{
    return spawn.z;
}

float Floor::getTileSize()
{
    return size;
}

void Floor::clearTiles()
{
    for(Tile* p_tile : floor_tiles)
    {
        delete p_tile;
    }
    floor_tiles.clear();
}

void Floor::displayFloor(bool hitbox)
{
    for (Tile* tile : floor_tiles)
    {
        tile->displayAsset(hitbox);
    }
}