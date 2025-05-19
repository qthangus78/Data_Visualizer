#pragma once 
#include "GlobalVar.h"

struct TextBox {
    Color rectColor = BLACK; /// need input
    Rectangle rect = {}; /// need input
    Rectangle scaledRect = {};
    float roundness = 0.5f;
    int numSegment = 10;

    Color textColor = WHITE; /// need input
    const char* text = "\0"; /// need input
    float fontSize = 25; /// need input
    float current_fontSize = 0;
    Vector2 textPos = {};

    float scale = 1.0f;
    float scaleRate = 1.0f;

    void ScaleProcess();
    void MakeHover();
    void Draw_TextBox(bool makeHover = 0);
    void Draw_TextBox2();
    void Draw_BlinkingLine();
};