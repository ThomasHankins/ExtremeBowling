#include "startMenu.h"

#include <iostream>
#include <fstream>
#include "../third_party/json.hpp"

using namespace std;
using json = nlohmann::json;

StartMenu::StartMenu(int windowX, int windowY) : Menu(windowX, windowY)
{
    level1 = Button(windowWidth*0.40, windowWidth*0.60, windowHeight*0.34, windowHeight*0.30, "Level 1");
    level2 = Button (windowWidth*0.40, windowWidth*0.60, windowHeight*0.47, windowHeight*0.43, "Level 2");
    level3 = Button(windowWidth*0.40, windowWidth*0.60, windowHeight*0.60, windowHeight*0.56, "Level 3");

    instructions = Button(windowWidth*0.40, windowWidth*0.60, windowHeight*0.77, windowHeight*0.73, "Instructions");
    quit = Button(windowWidth*0.45, windowWidth*0.55, windowHeight*0.83, windowHeight*0.79, "Quit");
}

void StartMenu::display()
{
    ifstream f1("src/levels/map1.json");
    if (f1.is_open())
    {
        json level1_data;
        f1 >> level1_data;
        f1.close();
        level1HighScore = level1_data.find("HighScore").value();
    }
    
    ifstream f2("src/levels/map2.json");
    if (f2.is_open())
    {
        json level2_data;
        f2 >> level2_data;
        f2.close();
        level2HighScore = level2_data.find("HighScore").value();
    }

    ifstream f3("src/levels/map3.json");
    if (f3.is_open())
    {
        json level3_data;
        f3 >> level3_data;
        f3.close();
        level3HighScore = level3_data.find("HighScore").value();
    }

    start2D();

    drawBackground();
        
    glPushMatrix();

        glColor3f(1.0, 0.0, 0.0);
        void* font = GLUT_BITMAP_9_BY_15;

        // Game title
        string title = "WELCOME TO EXTREME BOWLING";
        centerText(title, font, windowHeight*0.20);

        // Underline the title
        string underlineTitle = "__________________________";
        centerText(underlineTitle, font, windowHeight*0.205);

        // High scores
        centerText("High Score: " + to_string(level1HighScore), font, windowHeight*0.37);
        centerText("High Score: " + to_string(level2HighScore), font, windowHeight*0.50);
        centerText("High Score: " + to_string(level3HighScore), font, windowHeight*0.63);
        
        // Buttons
        level1.display();
        level2.display();
        level3.display();
        instructions.display();
        quit.display();


    glPopMatrix();

    end2D();
}

bool StartMenu::level1Clicked(int x, int y)
{
    return level1.clicked(x, y);
}

bool StartMenu::level2Clicked(int x, int y)
{
    return level2.clicked(x, y);
}

bool StartMenu::level3Clicked(int x, int y)
{
    return level3.clicked(x, y);
}

bool StartMenu::instructionsClicked(int x, int y)
{
    return instructions.clicked(x, y);
}

bool StartMenu::quitClicked(int x, int y)
{
    return quit.clicked(x, y);
}