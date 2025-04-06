#pragma once
#include "GlobalVar.h"
#include <fstream>
#include <vector>
#include <raylib.h>
#include <utility>
#include <string>
#include "LinkedList.h"
#include "button.h"
#include <string>
using namespace std;

class MinHeap;

struct HeapNode{
    int val;
    Vector2 pos;
    bool exist;
};

struct Button {
    Rectangle rect;
    std::string txt;
    Color color;
};

class ButtonManager{
    public:
        float roundness = 0.2f;
        float segments = 0;
        float blinkTime = 0.0f;
        char name[4] = "\0";
        int letterCount = 0;
        bool notFound = false;

        Button push = { {10, screenHeight - 60, 200, 50}, "PUSH", Fade(MAROON, 0.2f) };
        Button remove = { {20 + push.rect.width, screenHeight - 60, 200, 50}, "REMOVE", Fade(MAROON, 0.2f) };
        Button clear = { {30 + remove.rect.width * 2, screenHeight - 60, 200, 50}, "CLEAR", Fade(MAROON, 0.2f) };
        Button top = { {40 + clear.rect.width * 3, screenHeight - 60, 200, 50}, "TOP", Fade(MAROON, 0.2f) };
        Button size;
        Button random;

        void DrawButtons();
        void HandleButtons(MinHeap* mHeap);
        void DrawInputBox(int x, int y);
        void DrawBlinkingLine(int x, int y);
        void DrawBlinkingText( string txt, int x, int y );
        void DrawRandom(int x, int y);
};

extern std::vector<HeapNode> heapNode;

class IStateHeap{
public:
    ~IStateHeap(){}
    virtual void draw() = 0;
    virtual void update() = 0;
    ButtonManager buttons;
};


class MinHeap{
public:
    std::vector<int> tree;
    IStateHeap* mPush;
    IStateHeap* mRemove;
    IStateHeap* mTop;
    IStateHeap* mSize;
    IStateHeap* mClear;
    IStateHeap* mCurrent;
    IStateHeap* mWaiting;
    
    MinHeap();
    ~MinHeap();

    // Heap's utility functions
    int parent ( int i );
    int left ( int i );
    int right ( int i );
    void pop();
    void downHeap ( int idx );
    void upHeap ( int idx );
    int search ( int val );

    int size(); // Animation
    int top(); // Animation 
    void push ( int val ); // Animation 
    void remove ( int idx ); // Animation
    void fileInput ( std::ifstream& fin ); // Animation 
    void clear(); // Animation

    // State
    void setState(IStateHeap* state);
    IStateHeap* getPush();
    IStateHeap* getRemove();
    IStateHeap* getTop();
    IStateHeap* getSize();
    IStateHeap* getClear();

    void draw();
    void update();
};

class Push : public IStateHeap{
private:
    MinHeap *mHeap;
    int currentStep = 0;
    bool isAnimating = false;
public:
    Vector2 animatingPos;
    Vector2 targetPos;
    Vector2 animatingPos2;
    Vector2 targetPos2;
    int animatingIdx = -1;
    int parentIdx = -1;

    Push(MinHeap* heap);
    void draw() override;
    void update() override;

    void handleInsert(int val);
    void updateInsert();
    void handleBubbleUp();
    void updateBubbleUp();
};

class Remove : public IStateHeap{
private:
    MinHeap *mHeap;
    int currentStep = 0;
    bool isAnimating = false;
public:
    Vector2 animatingPos;
    Vector2 targetPos;
    Vector2 animatingPos2;
    Vector2 targetPos2;
    int animatingIdx = -1;
    int childIdx = -1;

    Remove(MinHeap* heap);
    void draw() override;
    void update() override;

    void handleRemove(int idx);
    void updateRemove();
    void handleBubbleDown();
    void updateBubbleDown();
};

class ClearH : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    ClearH(MinHeap* heap);
    void draw() override;
    void update() override;
};

class Top : public IStateHeap{
private:
    float blinkTime = 0.0f;
    MinHeap *mHeap;
public:
    Top(MinHeap* heap);
    void DrawBlinkingTop();
    void draw() override;
    void update() override;
};

// Drawing function
Vector2 calculateArrowPosition ( Vector2 &start, Vector2 &end, const float &radius );
Vector2 calculateNodePos ( Vector2 pos, int offsetY, int offsetX, int height);
void drawHeap(std::vector<int>& tree);
void DrawPartOfHeap ( MinHeap* mHeap, int animatingIdx, int parentIdx, bool isAnimating, int curentStep );

Vector2 Vector2Normalize(Vector2 v);
void swapHeapNode(HeapNode &a, HeapNode &b);
void recalculateNodePos ( MinHeap* mHeap );








