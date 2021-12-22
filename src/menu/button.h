#ifndef BUTTON_H
#define BUTTON_H

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

#include <string>
using namespace std;

class Button {
    public:
        Button();
        Button(float leftX, float rightX, float botY, float topY, string text);
        void display();
        bool clicked(int x, int y);
    private:
        float leftX;
        float rightX;
        float botY;
        float topY;
        string text;
        void centerText(string text, void* font);
};


#endif