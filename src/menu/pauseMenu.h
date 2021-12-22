#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "menu.h"

class PauseMenu : public Menu {
    public:
        PauseMenu(int windowX, int windowY);
        void display();
};

#endif