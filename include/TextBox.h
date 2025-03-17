#pragma once 
#include "GlobalVar.h"

struct TextBox {
    Color rectColor = BLACK; /// need input
    Rectangle rect; /// need input
    Rectangle scaledRect; 
    float roundness = 0.5f;
    int numSegment = 10;

    Color textColor = WHITE; /// need input
    const char* text; /// need input
    float fontSize = 25; /// need input
    float current_fontSize;
    Vector2 textPos;

    float scale = 1.0f;
    float scaleRate = 1.0f;


    void ScaleProcess() {
        /// adjust :
        // scale
        // current_fontSize
        // textPos
        // scaledRect

        bool isHovered = CheckCollisionPointRec(mouse, rect);
        scale = isHovered ? scaleRate : 1.0f;

        current_fontSize = isHovered ? 1.0f * fontSize * scale : fontSize;
        int textWidth = MeasureText(text, fontSize);

        // adjust rect.width
        if (rect.width <= textWidth + 20) 
            rect.width = textWidth + 20;
        
        scaledRect = { 
            rect.x - (rect.width * (scale - 1.0f)) / 2, 
            rect.y - (rect.height * (scale - 1.0f)) / 2,
            rect.width * scale,
            rect.height * scale
        };

        
        textPos = {
            scaledRect.x + (scaledRect.width - textWidth) / 2,
            scaledRect.y + (scaledRect.height - fontSize) / 2
        };
    }

    void Draw_TextBox() {
        ScaleProcess();
        DrawRectangleRounded(scaledRect, roundness, numSegment, rectColor);
        DrawTextEx(customFont, text, {textPos.x , textPos.y}, current_fontSize, 1.5, textColor);
    }
};