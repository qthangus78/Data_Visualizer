#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include <string.h>


namespace settingScreen {
    void Display() {
        ClearBackground(theme.getTheme());
        display_title("Settings" , ScreenID::StartScreen);
        music.draw();  
        theme.draw();
        theme.update();
    }
}