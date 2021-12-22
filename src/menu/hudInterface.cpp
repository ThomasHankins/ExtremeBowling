#include "hudInterface.h"

HudInterface::HudInterface(int windowX, int windowY) : Menu(windowX, windowY) {}

void HudInterface::display(int lives, int currentTime)
{
    start2D();

    glPushMatrix();

        glColor3f(0.95, 0.95, 0.95);
        glBegin(GL_QUADS);
            glVertex2f(windowWidth*0.80, windowHeight*0.90);
            glVertex2f(windowWidth, windowHeight*0.90);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f(windowWidth*0.80, windowHeight);
        glEnd();

    glPopMatrix();

    glPushMatrix();

        glColor3f(1.0, 0.0, 0.0);
        void* font = GLUT_BITMAP_9_BY_15;
        string livesText = "Lives left: " + std::to_string(lives);
        float livesTextWidth = 0;
        for (int i = 0; i < (int) livesText.size(); i++) {
            livesTextWidth += glutBitmapWidth(font, livesText[i]);
        }
        glRasterPos2f(windowWidth*0.90 - livesTextWidth / 2.0, windowHeight*0.935);
        for (char& c : livesText) {
            glutBitmapCharacter(font, c);
        }
        string currentTimeText = "Time: " + std::to_string(currentTime/1000) + " seconds";
        float currentTimeTextWidth = 0;
        for (int i = 0; i < (int) currentTimeText.size(); i++) {
            currentTimeTextWidth += glutBitmapWidth(font, currentTimeText[i]);
        }
        glRasterPos2f(windowWidth*0.90 - currentTimeTextWidth / 2.0, windowHeight*0.975);
        for (char& c : currentTimeText) {
            glutBitmapCharacter(font, c);
        }

    glPopMatrix();

    end2D();
}