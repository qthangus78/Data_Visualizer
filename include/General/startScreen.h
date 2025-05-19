#pragma once
#include "GlobalVar.h"
#include "TextBox.h"

namespace startScreen {
    void Display() {
        ClearBackground(theme.getTheme());

        TextBox Start;
        TextBox Settings;
        TextBox Exit;

        Start.scaleRate = Settings.scaleRate = Exit.scaleRate = 1.2f;

        // Color 
        Start.rectColor = Settings.rectColor = Exit.rectColor = {52, 86, 221, 255}; 

        // assign Text
        Start.text = "Start";
        Settings.text = "Settings";
        Exit.text = "Exit";

        Start.rect = {1.0f * screenWidth / 2 - 100 - 250, 500, 200, 100};
        Settings.rect = {1.0f * screenWidth / 2 - 100, 500, 200, 100};
        // Exit.rect = {1.0f * screenWidth / 2 - 100, 1.0f * screenHeight / 4 * 3.0f, 200, 100};
        Exit.rect = {1.0f * screenWidth / 2 - 100 + 250, 500, 200, 100};

        Start.Draw_TextBox();
        Settings.Draw_TextBox();
        Exit.Draw_TextBox();
        DrawTextureEx(logoDS, {1.0f * screenWidth / 2 - logoDS.width * 0.8f/ 2, 100}, 0.0f, 0.8f, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Start.rect)) {
            currentScreenID = ScreenID::StartMenuScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Settings.rect)) {
            currentScreenID = ScreenID::SettingScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Exit.rect)) {
            WindowClose = 1;
        }

    }
};
