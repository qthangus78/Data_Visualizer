#include "Heap.h"

// void drawButtons(){
//     // Push button
//     DrawRectangleRounded(buttonVar::buttonIns.rect, 0.3f, 30, buttonVar::buttonIns.buCol);
//     DrawTextEx(customFont,buttonVar::buttonIns.text.c_str(),{buttonVar::buttonIns.rect.x+15,buttonVar::buttonIns.rect.y+7}, 22, 2, WHITE);
// }

void MinHeap::update(){
    
}

void MinHeap::draw(){
    if (tree.empty()) return;

    int nodeRadius = 20;
    int verticalSpacing = 50;
    int horizontalSpacing = 40;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    
}