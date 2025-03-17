#include "startScreen.h"
#include "menuScreen.h"
#include "settingScreen.h"
#include "SLL_display.h"
#include "Heap_display.h"
#include "AVLTree_display.h"
#include "ShortestP_display.h"

int main() {       
    InitWindow(screenWidth, screenHeight, "Data Visualizer");
    SetTargetFPS(FPS);

    customFont = LoadFont("../resources/fonts/Montserrat-Bold.ttf");

    while(!WindowShouldClose() && !WindowClose) {
        BeginDrawing();

        mouse = GetMousePosition();
  
        switch (currentSlideID)
        {
            case 0: 
                startScreen::Display();
                break;

            case 1: 
                startMenuScreen::Display();
                break;

            case 2: 
                settingScreen::Display();
                break;
            
            case 3:
                SLL_display::Display();
                break;
            
            case 4:
                Heap_display::Display();
                break;
            
            case 5:
                AVLTree_display::Display();
                break;

            case 6:
                ShortestP_display::Display();
                break;
        }
        

        // Texture2D hihi = LoadTexture("../resources/images/back-normal.png");
        // DrawTexture(hihi, 100, 100, WHITE);
        // DrawTextureEx(hihi, {200, 200}, 180, 2, WHITE);
        // void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
        // cout << hihi.width << ' ' << hihi.width << endl;

        EndDrawing();
    }

    UnloadFont(customFont);

    CloseWindow();
    return 0;
}