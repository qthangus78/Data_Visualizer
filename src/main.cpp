#include "startScreen.h"
#include "menuScreen.h"
#include "settingScreen.h"
#include "SLL_display.h"
#include "Heap_display.h"
#include "AVLTree_display.h"
#include "ShortestP_display.h"

int main() {       
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Data Visualizer");
    SetTargetFPS(FPS);

    customFont = LoadFont("../resources/fonts/Montserrat-Bold.ttf");
    SSL SSL;
    SLL_display::sslInstance = &SSL; 

    MinHeap minHeap;
    Heap_display::heapInstance = &minHeap;
    minHeap.tree = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    
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