#ifndef GRAPHICS_H
#define GRAPHICS_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <vector>
#include <string>
#include "mathlib_3D.h"

using namespace std;

#define FLOOR_M {{1.000000f, 1.000000f, 1.000000f,0.2f}, {0.882919f, 0.424586f, 0.199295f, 0.2f}, {0.500000f, 0.500000f, 0.500000f, 0.2f}, 1.0f}
#define BOWLING_BALL {{0.753695f, 0.753695f, 0.753695f,0.2f}, {0.0f, 0.0f, 0.0f, 0.2f}, {0.500000f, 0.500000f, 0.500000f, 0.2f}, 3.0f}
#define BRASS {{0.329412f, 0.223529f, 0.027451f,0.2f}, {0.780392f, 0.568627f, 0.113725f, 0.2f}, {0.992157f, 0.941176f, 0.807843f, 0.2f}, 27.8974f}
#define BRONZE {{0.2125f, 0.1275f, 0.054f, 0.2f}, {0.714f, 0.4284f, 0.18144f, 0.2f}, {0.393548f, 0.271906f, 0.166721f, 0.2f}, 25.6f}
#define SILVER {{0.23125f, 0.23125f, 0.23125f, 0.2f}, {0.2775f, 0.2775f, 0.2775f, 0.2f}, {0.773911f, 0.773911f, 0.773911f, 0.2f}, 89.6f}
#define GOLD {{0.24725f, 0.2245f, 0.0645f, 0.2f}, {0.34615f, 0.3143f, 0.0903f, 0.2f}, {0.797357f, 0.723991f, 0.208006f, 0.2f}, 83.2f}
#define OBSIDIAN {{0.05375f, 0.05f, 0.06625f, 0.82f}, {0.18275f, 0.17f, 0.22525f, 0.82f}, {0.332741f, 0.328634f, 0.346435f, 0.82f}, 38.4f}
#define RUBY {{0.1745f, 0.01175f, 0.01175f, 0.55f}, {0.61424f, 0.04136f, 0.04136f, 0.55f}, {0.727811f, 0.626959f, 0.626959f, 0.55f}, 76.8f}
#define BLUE {{0.24175f, 0.24175f, 0.5745f, 0.55f}, {0.29136f, 0.29136f, 0.81424f, 0.55f}, {0.726959f, 0.726959f, 0.827811f, 0.55f}, 86.8f}
#define EMERALD {{0.0215f, 0.1745f, 0.0215f, 0.55f}, {0.07568f, 0.61424f, 0.07568f, 0.55f}, {0.633f, 0.727811f, 0.633f, 0.55f}, 76.8f}
#define WHITE_RUBBER {{0.45f, 0.45f, 0.45f, 1.0f}, {0.75f, 0.75f, 0.75f, 1.0f}, {0.75f, 0.75f, 0.75f, 1.0f}, 10.0f}

#define NO_TEXTURE 0
#define PIN_TEXTURE 1
#define SWEEPER_TEXTURE 2
#define WOOD_TEXTURE 3
#define FINISH_TEXTURE 4
#define USER_TEXTURE 5
#define BOOMBA_TEXTURE 6
#define LIFE_TEXTURE 7
#define GHOST_TEXTURE 8
#define SIZE_TEXTURE 9

struct Material {
    float amb[4];
    float diff[4];
    float spec[4];
    float shine;
};

class Graphics {

    private:
        static GLuint pinTexture, sweeperTexture, woodTexture, finishTexture, userTexture, boombaTexture, lifeTexture, ghostTexture, sizeTexture;
        static GLuint loadPPM(string filename, bool repeat=false);

    public:
        Graphics();
        Graphics(string filename, Material m=GOLD);
        bool loadObj(string filename,
            vector <Vec3D> & out_vertices,
            vector <Vec3D> & out_uvs,
            vector <Vec3D> & out_normals 
        );
        void setMaterial(Material m);
        void setTexture(int textureId);
        
        GLuint texture;
        Material mat;
        vector <Vec3D> vertices; 
        vector <Vec3D> uvs; //texture mapping
        vector <Vec3D> normals;

        
        void displayAsset(Rot3D r);
        
        
        static void loadTextures();


};



#endif