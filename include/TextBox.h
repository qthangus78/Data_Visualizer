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

    void ScaleProcess();
    void Draw_TextBox();
};