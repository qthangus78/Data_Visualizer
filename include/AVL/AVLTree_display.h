#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "AVLTree.h"

namespace AVLTree_display {
    AVL_tree Hung;

    // const int arr[] = {10, 20, 5, 50, 100, 22, 12, 25, 11, 23, 59, 15, 105};
    // int cnt = 0;

    string textIn;

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("AVL Tree" , ScreenID::StartMenuScreen);

        // DrawRectangle(100, 200, 100, 100 , BLUE);
        
        int key = GetKeyPressed();
        
        if (key == KEY_ENTER && textIn.size() > 0) {
            int x = 0;
            for (char c : textIn)
                x = x * 10 + (c - '0');
            Hung.Insert(x);
            textIn.clear();
        }

        if (key >= '0' && key <= '9') {
            textIn += char(key);
        }

        if (key == KEY_BACKSPACE && !textIn.empty())
            textIn.pop_back();

        DrawText(textIn.c_str(), 100, 200, 20, BLACK);
        DrawNode(Hung.root, screenWidth / 2 - 20, 100);         
    }   
}   