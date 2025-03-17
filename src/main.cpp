#include "../include/startScreen.h"
#include "../include/menuScreen.h"
#include "../include/settingScreen.h"
#include "../include/SLL_display.h"
#include "../include/Heap_display.h"
#include "../include/AVLTree_display.h"
#include "../include/ShortestP_display.h"

const int screenWidth = 1000;
const int screenHeight = 700;
const int FPS = 60;
int currentSlideID = 0;
Vector2 mouse;
bool WindowClose = 0;
Font customFont;


int main() {       
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Data Visualizer");
    SetTargetFPS(FPS);

    customFont = LoadFont("../resources/fonts/Montserrat-Bold.ttf");
    SSL SSL;

    SLL_display::sslInstance = &SSL; 

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

        EndDrawing();
    }

    UnloadFont(customFont);
    CloseWindow();
    return 0;
}