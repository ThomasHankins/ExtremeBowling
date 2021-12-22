#include "instructionsMenu.h"

InstructionsMenu::InstructionsMenu(int windowX, int windowY) : Menu(windowX, windowY)
{
    back = Button(windowWidth*0.45, windowWidth*0.55, windowHeight*0.83, windowHeight*0.79, "Back");
}

void InstructionsMenu::display()
{
    start2D();

    drawBackground();

    glPushMatrix();

        glColor3f(1.0, 0.0, 0.0);
        void* font = GLUT_BITMAP_9_BY_15;

        // Instructions title
        string title = "INSTRUCTIONS";
        centerText(title, font, windowHeight*0.20);

        // Underline the title
        string underlineTitle = "____________";
        centerText(underlineTitle, font, windowHeight*0.205);

        /* Game instructions */
        string goal1 = "The goal of the game is to get to the end of the map";
        centerText(goal1, font, windowHeight*0.30);

        string goal2 = "(indicated by a checkered tile) as quickly as possible,";
        centerText(goal2, font, windowHeight*0.33);

        string goal3 = "while collecting as many pins as possible and avoiding";
        centerText(goal3, font, windowHeight*0.36);

        string goal4 = "being hit by the sweepers and boombas. The green tiles";
        centerText(goal4, font, windowHeight*0.39);

        string goal5 = "indicate checkpoints, and there are different power ups";
        centerText(goal5, font, windowHeight*0.42);

        string goal6 = "available, including a life power up, a decrease size";
        centerText(goal6, font, windowHeight*0.45);

        string goal7 = "power up and a ghost power up.";
        centerText(goal7, font, windowHeight*0.48);


        string commands = "Commands:";
        centerText(commands, font, windowHeight*0.57);

        // Space to pause
        string space = "Press space to pause the game.";
        centerText(space, font, windowHeight*0.60);

        // Move mouse to move camera 
        string camera = "Look around by moving the mouse.";
        centerText(camera, font, windowHeight*0.63);

        // Arrow keys to move
        string arrowKeys = "Use arrow keys or WASD to move the ball.";
        centerText(arrowKeys, font, windowHeight*0.66);

        // Left click to jump
        string leftClick = "Left click to make the ball jump.";
        centerText(leftClick, font, windowHeight*0.69);

        back.display();

    glPopMatrix();

    end2D();
}

bool InstructionsMenu::backClicked(int x, int y)
{
    return back.clicked(x, y);
}