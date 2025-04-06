#pragma once 
#include <bits/stdc++.h>
#include <raylib.h>
#include "button.h"
// using namespace std;

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
extern Font customFont;

extern myTexture BackButton; 
extern myTexture PlayButton;
extern myTexture PauseButton;
extern myTexture ReplayButton;
extern myTexture UndoButton;
extern myTexture RedoButton;
extern ScreenID currentScreenID;
extern Texture2D customTexture;
extern Texture2D dice;


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
//--------------------------------
// ListNode
//--------------------------------
struct ListNode {
    int data;
    ListNode* next;
    ListNode(int x);
};

struct ShadedData {
    Vector2 pos;
    ListNode* node;
};

void LoadFontResource();
void LoadButtonsResources();

void UnloadFontResource();


