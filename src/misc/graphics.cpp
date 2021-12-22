#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>

using namespace std;

GLuint Graphics::pinTexture = 0;
GLuint Graphics::sweeperTexture = 0;
GLuint Graphics::woodTexture = 0;
GLuint Graphics::finishTexture = 0;
GLuint Graphics::userTexture = 0;
GLuint Graphics::boombaTexture = 0;
GLuint Graphics::lifeTexture = 0;
GLuint Graphics::ghostTexture = 0;
GLuint Graphics::sizeTexture = 0;

Graphics::Graphics(){}

Graphics::Graphics(string filename, Material m){
    loadObj(filename, 
            this->vertices,
            this->uvs,
            this->normals);
    mat = m;
    this->texture = NO_TEXTURE;
}

bool Graphics::loadObj(string filename,
                vector <Vec3D> & out_vertices,
                vector <Vec3D> & out_uvs,
                vector <Vec3D> & out_normals)
{

    vector <unsigned int> vtxIndices, uvIndices, nIndices;
    vector <Vec3D> tempVertices; //Mesh
    vector <Vec3D> tempUV; // Textures 
    vector <Vec3D> tempNormals; // Normals

    // open the file
    filename = "src/objects/" + filename + ".obj";
    FILE * file = fopen(filename.c_str(), "r");

    
    if( file == NULL ){
        printf("Impossible to open file %s from\n", filename.c_str());
         return false;
    }

    // reading the file
    while(true) 
    {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);

        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) 
        {
            Vec3D vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            tempVertices.push_back(vertex);

        } else if (strcmp(lineHeader, "vn") == 0)
        {
            Vec3D normal;

            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);

        // faces
        } else if (strcmp(lineHeader, "f")== 0) 
        {
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vtxIndices.push_back(vertexIndex[0]);
            vtxIndices.push_back(vertexIndex[1]);
            vtxIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            nIndices.push_back(normalIndex[0]);
            nIndices.push_back(normalIndex[1]);
            nIndices.push_back(normalIndex[2]);
        } else if (strcmp( lineHeader, "vt" ) == 0)
        {
            Vec3D uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            tempUV.push_back(uv);
        }
    }

    for( unsigned int i=0; i<vtxIndices.size(); i++ ){
        unsigned int vertexIndex = vtxIndices[i] - 1;
        if (vertexIndex < 0)
            vertexIndex += vtxIndices.size() + 1;
        Vec3D vertex = tempVertices[ vertexIndex ];
        out_vertices.push_back(vertex);
    }

    for( unsigned int i=0; i<uvIndices.size(); i++ ){
        unsigned int uvIndex = uvIndices[i] - 1;
        if (uvIndex < 0)
            uvIndex += uvIndices.size() + 1;
        Vec3D uv = tempUV[ uvIndex ];
        out_uvs.push_back(uv);
    }

    for( unsigned int i=0; i<nIndices.size(); i++ ){
        unsigned int normalIndex = nIndices[i] - 1;
        if (normalIndex < 0)
            normalIndex += nIndices.size() + 1;
        Vec3D normal = tempNormals[ normalIndex ];
        out_normals.push_back(normal);
    }

    fclose(file);


    return true;
}

void Graphics::setMaterial(Material m)
{
    mat = m;
}

void Graphics::setTexture(int textureId)
{
    switch(textureId)
    {
        case PIN_TEXTURE:
            this->texture = pinTexture;
            break;
        case SWEEPER_TEXTURE:
            this->texture = sweeperTexture;
            break;
        case WOOD_TEXTURE:
            this->texture = woodTexture;
            break;
        case FINISH_TEXTURE:
            this->texture = finishTexture;
            break;
        case USER_TEXTURE:
            this->texture = userTexture;
            break;
        case BOOMBA_TEXTURE:
            this->texture = boombaTexture;
            break;
        case LIFE_TEXTURE:
            this->texture = lifeTexture;
            break;
        case GHOST_TEXTURE:
            this->texture = ghostTexture;
            break;
        case SIZE_TEXTURE:
            this->texture = sizeTexture;
            break;
        default:
            this->texture = 0;
            break;
    }
}

GLuint Graphics::loadPPM(string filename, bool repeat){
    int i;
    char b[100];
    char c;
    float s;
    int width, height, max, pixels;
    GLubyte* img;
    int red, green, blue;

    filename = "src/ppm/" + filename + ".ppm";
    FILE * file = fopen(filename.c_str(), "r");

    fscanf(file,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",filename.c_str());
        return false;
    }
    fscanf(file, "%c",&c);
    while(c == '#')
    {
        fscanf(file, "%[^\n] ", b);
        fscanf(file, "%c",&c);
    }
    ungetc(c,file);
    fscanf(file, "%d %d %d", &width, &height, &max);
    
    pixels = width*height;
    
    img = (GLubyte*)(malloc(3*sizeof(GLuint)*pixels));
    
    s=255.0/max;
    
    
    for(i=0;i<pixels;i++)
    {
        fscanf(file,"%d %d %d",&red, &green, &blue );
        img[3*pixels-3*i-3]=red*s;
        img[3*pixels-3*i-2]=green*s;
        img[3*pixels-3*i-1]=blue*s;
    }

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

    if (repeat)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    return textureID;

}

void Graphics::loadTextures()
{   
    pinTexture = loadPPM("pin");
    sweeperTexture = loadPPM("SweeperCoat");
    woodTexture = loadPPM("woodFloor");
    finishTexture = loadPPM("finish");
    userTexture = loadPPM("PlayerCoat");
    boombaTexture = loadPPM("EnemyCoat");
    lifeTexture = loadPPM("ExtraLifeCoat");
    ghostTexture = loadPPM("GhostCoat");
    sizeTexture = loadPPM("HalfSizeCoat");
}



void Graphics::displayAsset(Rot3D r)
{
    
    if(texture != NO_TEXTURE){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shine);

    glPushMatrix();
        glRotatef(r.getAngle(), r.getX(), r.getY(), r.getZ());
        glBegin(GL_TRIANGLES);
        // render each triangle
        for (long unsigned int i = 0; i < this->vertices.size() ; i++) {
            glNormal3f(this->normals.at(i).x, this->normals.at(i).y, this->normals.at(i).z);
            if(texture != NO_TEXTURE){
                glTexCoord2f(uvs.at(i).x,uvs.at(i).y);
            }
            glVertex3f(this->vertices.at(i).x, this->vertices.at(i).y, this->vertices.at(i).z);
        }
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}