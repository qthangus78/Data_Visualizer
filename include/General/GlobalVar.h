#pragma once 
#include <bits/stdc++.h>
#include <raylib.h>
#include "button.h"
using namespace std;

extern const float screenWidth;
extern const float screenHeight;
extern const Vector2 GraphDisplayScreenSize;
extern Rectangle GraphDisplayScreen;

extern const int FPS;

extern Vector2 mouse;
extern bool WindowClose;

extern Font customFont;
extern Font SSLFont;
extern Font textCodeFont;
extern myTexture BackButton; 
extern myTexture PlayButton;
extern myTexture PauseButton;
extern myTexture ReplayButton;
extern myTexture UndoButton;
extern myTexture RedoButton;
extern myTexture addButton;
extern myTexture minusButton;
extern ScreenID currentScreenID;
extern Texture2D customTexture;
extern Texture2D dice;

struct SpeedButtonSpinner {
    float value = 1.0f;
    const float minValue = 0.5f;
    const float maxValue = 4.0f;
    const float step = 0.5f;
    Vector2 position;
    void Init(Vector2 pos) {
        position = pos;
        Vector2 leftCenter = {position.x - 44, position.y-7};
        Vector2 rightCenter = {position.x + 44, position.y-7};
        addButton.SetPosition(rightCenter.x,rightCenter.y);
        minusButton.SetPosition(leftCenter.x,leftCenter.y);
    }
    void Update() {
            if(minusButton.isPressed()) value = std::max(minValue, value - step);
            if(addButton.isPressed()) value = std::min(maxValue, value + step);
    }
    void Draw() const {
        addButton.Drawtexture();
        minusButton.Drawtexture();
        char text[10];
        snprintf(text, sizeof(text), "x%.1f", value);
        Vector2 textSize = MeasureTextEx(customFont, text, 24, 1);
        Vector2 textPos = { position.x + 20 - textSize.x / 2, position.y + 15 - textSize.y / 2 };
        DrawTextEx(customFont, text, textPos, 24, 1, BLACK);

    }
    float GetValue() const {
        return value;
    }
};


//--------------------------------
// Cấu trúc button
//--------------------------------
struct button {
    Rectangle rect;
    static const int sizeW = 100;
    static const int sizeH = 40;
    Color buCol;
    std::string text;
};
//--------------------------------
// Namespace màu
//--------------------------------
namespace color {
    extern Color buttonColor;
    extern Color buttonColorHovered;
    extern Color buttonFile;
    extern Color buttonFileHovered;
    extern Color nodeRendered;
    extern Color nodeNotInMode;
    extern Color edgeRendered;
    extern Color edgeNotInMode;
}
//--------------------------------
// Namespace chứa các nút
//--------------------------------
namespace buttonVar {
    extern button buttonIns;
    extern button buttonDel;
    extern button buttonF;
    extern button buttonClear;
    extern button buttonGo;
    extern button buttonCreate;
}
//--------------------------------
// Cấu trúc Arrow
//--------------------------------
struct Edge {
    float length;     
    float thickness;
    float arrowsize;
};
extern Edge EArrow;
extern Vector2 startLinkedListPos;

void LoadFontResource();
void LoadButtonsResources();

void UnloadFontResource();


