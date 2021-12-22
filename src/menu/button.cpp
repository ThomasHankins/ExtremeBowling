#include "button.h"

Button::Button() {}

Button::Button(float leftX, float rightX, float botY, float topY, string text)
{
    this->leftX = leftX;
    this->rightX = rightX;
    this->botY = botY;
    this->topY = topY;
    this->text = text;
}

void Button::display()
{
    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        void* font = GLUT_BITMAP_9_BY_15;
        // draw button
        glBegin(GL_QUADS);
            glVertex2f(leftX, botY);
            glVertex2f(rightX, botY);
            glVertex2f(rightX, topY);
            glVertex2f(leftX, topY);
        glEnd();

        glColor3f(1.0, 1.0, 1.0);
        centerText(text, font);
    glPopMatrix();
}

// Will render and center text
void Button::centerText(string text, void* font)
{
    float textWidth = 0;
    for (int i = 0; i < (int) text.size(); i++) {
        textWidth += glutBitmapWidth(font, text[i]);
    }
    // If font is ever changed this will needed to be changed as well, or reshape
    glRasterPos2f((this->leftX + this->rightX) / 2.0 - textWidth / 2.0, (this->botY + this->topY) / 2.0 + 5.0);
    for (char& c : text) {
        glutBitmapCharacter(font, c);
    }
}

bool Button::clicked(int x, int y)
{
    // based on glut coordinates (y = 0 starts at top of screen)
    return (leftX <= x && x <= rightX && botY >= y && y >= topY);
}