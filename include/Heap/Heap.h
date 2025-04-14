#pragma once
#include <fstream>
#include <vector>
#include <raylib.h>
#include <utility>
#include <string>
#include "GlobalVar.h"
#include "LinkedList.h"
#include "button.h"

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
        float fontSize = 30.0f;
        char name[4] = "\0";
        int letterCount = 0;
        bool notFound = false;
        Button push = { {10, 385, 200, 50}, "PUSH", BLUE };
        Button remove = { {10, 385 + 60, 200, 50}, "REMOVE", BLUE };
        Button clear = { {10, 385 + 60*2, 200, 50}, "CLEAR", BLUE };
        Button top = { {10, 385 + 60*3, 200, 50}, "TOP", BLUE };
        Button initialize = { {10, 385 + 60*4, 200, 50}, "INITIALIZE", BLUE };
        Rectangle loadFile = { 220, 385 + 60*5, 200, 50 };
        Button random;
        // Button size;

        void DrawButton(Button &button);
        void DrawButtons();
        void HandleButtonsClick(MinHeap* mHeap);
        void HandleButtonsHover();
        void DrawInputBox(float x, float y);
        void DrawBlinkingLine(float x, float y);
        void DrawBlinkingText( string txt, float x, float y );
        void DrawRandom(float x, float y);
        void DrawLoadFile();
        void DrawBackground();
};

extern bool isPaused;
extern float blinkTime;
extern std::vector<float> elapsedTime;
extern std::vector<HeapNode> targetHeapNode;
extern std::vector<HeapNode> originHeapNode;
extern std::vector<HeapNode> heapNode;
extern int offsetX;
extern int offsetY;
extern int nodeRadius;
extern float duration;

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
    IStateHeap* mInitialize;
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
    IStateHeap* getState();
    IStateHeap* getPush();
    IStateHeap* getRemove();
    IStateHeap* getTop();
    IStateHeap* getSize();
    IStateHeap* getClear();
    IStateHeap* getWaiting();
    IStateHeap* getInitialize();

    void draw();
    void update();
};

class PushState{
public:
    std::vector<int> tree;
    std::vector<HeapNode> heapNode;
    std::vector<HeapNode> originHeapNode;
    std::vector<HeapNode> targetHeapNode;
    int animatingIdx;
    int parentIdx;
    Vector2 animatingPos;
    Vector2 targetPos;
    Vector2 originPos;
    Vector2 animatingPos2;
    Vector2 targetPos2;
    Vector2 originPos2;
    int currentStep;
    bool isAnimating;

    PushState();
    ~PushState();
};

class Push : public IStateHeap{
public:
    MinHeap *mHeap;
    int currentStep = -1;
    bool isAnimating = false;
    float blinkTime1 = 0.0f;
    float blinkTime2 = 0.0f;

    std::stack<PushState> undoStack;
    void saveState();
    void handleUndo();

    Vector2 animatingPos;
    Vector2 targetPos;
    Vector2 originPos;

    Vector2 animatingPos2;
    Vector2 targetPos2;
    Vector2 originPos2;

    int animatingIdx = -1;
    int parentIdx = -1;

    Push(MinHeap* heap);
    void draw() override;
    void update() override;

    void handleInsert(int val);
    void updateInsert();
    void updateTreeStructure();
    void handleBubbleUp();
    void drawBlinkingNode();
    void updateBubbleUp();
};

class Remove : public IStateHeap{
private:
    MinHeap *mHeap;
    int currentStep = -1;
    bool isAnimating = false;
public:
    Vector2 animatingPos;
    Vector2 targetPos;
    Vector2 originPos;

    Vector2 animatingPos2;
    Vector2 originPos2;
    Vector2 targetPos2;
    int tempIdx = -1;
    int animatingIdx = -1;
    int childIdx = -1;

    Remove(MinHeap* heap);
    void draw() override;
    void update() override;

    void updateTreeStructure();
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
    MinHeap *mHeap;
public:
    Top(MinHeap* heap);
    void draw() override;
    void update() override;
};

class Initialize : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    Initialize(MinHeap* heap);
    void handleInputFile();
    void draw() override;
    void update() override;
};

class Waiting : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    Waiting(MinHeap* heap);
    void draw() override;
    void update() override;
};

// Drawing function
Vector2 calculateArrowPosition ( Vector2 &start, Vector2 &end, const float &radius );
Vector2 calculateNodePos ( Vector2 pos, int height);
void drawHeap(std::vector<int>& tree);
void DrawPartOfHeap ( MinHeap* mHeap, int animatingIdx, int parentIdx, bool isAnimating, int curentStep );

void swapHeapNode(HeapNode &a, HeapNode &b);
void recalculateAllNodePos ( MinHeap* mHeap );
void updateNodePos ( Vector2 &animatingPos, Vector2 targetPos, Vector2 originPos, float duration, bool &isAnimating, int i = 0 );
void DrawBlinkingNode(Vector2 pos, int val, float &blinkTime);