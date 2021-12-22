#ifndef HUDINTERFACE
#define HUDINTERFACE

#include "menu.h"

class HudInterface : public Menu {
    public:
        HudInterface(int windowX, int windowY);
        void display(int lives, int currentTime);
};


#endif