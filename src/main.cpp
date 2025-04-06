#include "startScreen.h"
#include "menuScreen.h"
#include "settingScreen.h"
#include "SLL_display.h"
#include "Heap_display.h"
#include "AVLTree_display.h"
#include "Graph_Display.h"
#include "Trie_display.h"

int main() {       
    srand(time(0));
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    InitWindow((int)screenWidth, (int)screenHeight, "Data Visualizer");
    SetTargetFPS(FPS);

    LoadFontResource();
    LoadButtonsResources();

    SSL SSL;
    SLL_display::sslInstance = &SSL; 


    Trie Trie;
    Trie_Display::TrieInstance = &Trie;
    Trie.Visualize(Trie.root, screenWidth / 10, screenHeight / 2, 150, 0);

    MinHeap minHeap;
    Heap_display::heapInstance = &minHeap;
    
    while(!WindowShouldClose() && !WindowClose) {
        BeginDrawing();
        ClearBackground({192, 245, 242, 100});

        mouse = GetMousePosition();

        switch (ScreenID(currentScreenID)) {
            case ScreenID::StartScreen: 
                startScreen::Display();
                break;
            case ScreenID::StartMenuScreen: 
                startMenuScreen::Display();
                break;
            case ScreenID::SettingScreen: 
                settingScreen::Display();
                break;
            case ScreenID::SLLScreen: 
                SLL_display::Display();
                break;
            case ScreenID::HeapScreen: 
                Heap_display::Display();
                break;

            case ScreenID::AVLTreeScreen: 
                AVLTree_display::Display();
                //Trie_Display::Display();

                break;
            case ScreenID::GraphScreen: 
                Graph_display::Display();
                break;
        }

        EndDrawing();
    }

    UnloadFontResource();
    CloseWindow();
    return 0;
}