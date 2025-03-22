#pragma once
#include "GlobalVar.h"
#include "TextBox.h"


namespace settingScreen {
    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Settings" , 0);
    }
}