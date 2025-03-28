#pragma once 
#include<bits/stdc++.h>
#include<raylib.h>
using namespace std;

extern int screenWidth;
extern int screenHeight;
extern const int FPS;
extern int currentSlideID;

extern Vector2 mouse;
extern bool WindowClose;
extern Font customFont;

//--------------------------------
// Cấu trúc button
//--------------------------------
struct button {
    Rectangle rect;
    static const int sizeW = 120;
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

