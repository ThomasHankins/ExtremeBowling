#include "button.h"
#include "menu.h"

Menu::Menu(int windowX, int windowY) 
{
    this->windowWidth = windowX;
    this->windowHeight = windowY;
}

void Menu::display() {}

void Menu::display(int lives, int currentTime) {} // for hudInterface

void Menu::start2D()
{
    // we will be switching to 2d projection here
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // save projection matrix state
        glLoadIdentity(); // reset projection matrix
        gluOrtho2D(0, windowWidth, windowHeight, 0); // set 2d projection matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); // save modelview matrix state
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glLoadIdentity(); // reset modelview matrix state
            glDisable(GL_LIGHTING);
}


void Menu::drawBackground()
{
    // draw white background
    glPushMatrix();
        glColor3f(0.95, 0.95, 0.95);
        glBegin(GL_QUADS);
            glVertex2f(windowWidth*0.15, windowHeight*0.15);
            glVertex2f(windowWidth*0.85, windowHeight*0.15);
            glVertex2f(windowWidth*0.85, windowHeight*0.85);
            glVertex2f(windowWidth*0.15, windowHeight*0.85);
        glEnd();
    glPopMatrix();
}

void Menu::end2D()
{
            // switch back to 3d projection
            glMatrixMode(GL_PROJECTION);
        glPopMatrix(); // retrieve projection matrix state
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // retrieve modelview matrix state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
}

// Will render and center text
void Menu::centerText(string text, void* font, float y)
{
    float textWidth = 0;
    for (int i = 0; i < (int) text.size(); i++) {
        textWidth += glutBitmapWidth(font, text[i]);
    }
    glRasterPos2f(windowWidth / 2.0 - textWidth / 2.0, y);
    for (char& c : text) {
        glutBitmapCharacter(font, c);
    }
}