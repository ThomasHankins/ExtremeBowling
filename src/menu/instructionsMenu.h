#ifndef INSTRUCTIONSMENU_H
#define INSTRUCTIONSMENU_H

#include "menu.h"

class InstructionsMenu : public Menu {
    public:
        InstructionsMenu(int windowX, int windowY);
        void display();
        bool backClicked(int x, int y);
    private: 
        Button back;
};

#endif