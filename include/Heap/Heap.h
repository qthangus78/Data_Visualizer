#pragma once
#include <fstream>
#include <vector>
#include <raylib.h>
#include <utility>
#include <string>
#include <stack>
#include "GlobalVar.h"
#include "LinkedList.h"
#include "button.h"
#include "AnnouncementBox.h"
#include <string>
using namespace std;

class MinHeap;

struct PseudoCode {
    char titleC[16] = "CLEAR OPERATION";
    std::vector<char*> contentC = { "A.clear" };

    char titleP[15] = "PUSH OPERATION";
    std::vector<char*> contentP = {
        "i = A.heap_size",
        "A[i] = value",
        "while ( i > 0 )",
        "   p = PARENT(i)", 
        "   if A[p] > A[i]", 
        "      swap ( A[i], A[p] )", 
        "      i = p" ,
        "   else", 
        "      break"
    };

    char titleR[17] = "REMOVE OPERATION";
    std::vector<char*> contentR = {
        "swap ( A[i], A[A.heap_size - 1] )",
        "while ( smallest < A.heap_size )",
        "   l = LEFT(i), r = RIGHT(i), smallest = i",
        "   if l < A.heap_size and A[l] < A[smallest]",
        "      smallest = l",
        "   if r < A.heap_size and A[r] < A[smallest]",
        "      smallest = r",
        "   if smallest != i",
        "      swap ( A[i], A[smallest] )",
        "      i = smallest",
        "   else",
        "      break"
    };

    char titleT[14] = "TOP OPERATION";
    std::vector<char*> contentT = { "return A[0]" };

    char titleI[21] = "INITIALIZE OPERATION";
    std::vector<char*> contentI = {
        "A.heap_size = A.length",
        "for i = floor(A.length / 2) downto 1",
        "   MinHeapify( A, i )",
    };

    char titleS[17] = "SEARCH OPERATION";
    std::vector<char*> contentS = {
        "for i = 0 up to A.heap_size - 1",
        "   if A[i] == value",
        "      return i",
        "return -1"
    };

};

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
        Vector2 pos;
        std::string txt;
        float fontSize = 30.0f;
        char name[4] = "\0";
        int letterCount = 0;
        bool notFound = false;
        Button push = { {10, 385, 200, 50}, "PUSH", BLUE };
        Button remove = { {10, 385 + 60, 200, 50}, "REMOVE", BLUE };
        Button clear = { {10, 385 + 60*2, 200, 50}, "CLEAR", BLUE };
        Button top = { {10, 385 + 60*3, 200, 50}, "TOP", BLUE };
        Button initialize = { {10, 385 + 60*4, 200, 50}, "INITIALIZE", BLUE };
        Button search = { {10, 385 + 60*5, 200, 50}, "SEARCH", BLUE };
        Button Stepbystep = { {10, 5, 200, 50 }, "Step by step", BLUE };
        Rectangle loadFile = { 220, 385 + 60*5, 200, 50 };
        Button random;

        void Update();
        void DrawButtons();

        void setInputBox ( float x, float y );
        void setTxt ( std::string a );

        void DrawButton(Button &button);
        void HandleButtonsClick(MinHeap* mHeap);
        void HandleButtonsHover();
        void DrawInputBox();
        void DrawBlinkingText();
        void DrawLoadFile();
        void DrawBackground();
};

extern bool valueFound;
extern bool isStepbystep;
extern bool isPaused;
extern float blinkTime;
extern float txtBlinkTime;
extern int offsetX;
extern int offsetY;
extern int nodeRadius;
extern float duration;
extern PseudoCode yes;
extern AnnouncementBox pseudoCode;
extern SpeedButtonSpinner speedButton;
extern ButtonManager buttons;
extern std::vector<float> elapsedTime;
extern std::vector<HeapNode> targetHeapNode;
extern std::vector<HeapNode> originHeapNode;
extern std::vector<HeapNode> heapNode;

class IStateHeap{
public:
    ~IStateHeap(){}
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void reset() = 0;
};

class MinHeap{
public:
    std::vector<int> tree;
    IStateHeap* mPush;
    IStateHeap* mRemove;
    IStateHeap* mTop;
    IStateHeap* mClear;
    IStateHeap* mCurrent;
    IStateHeap* mInitialize;
    IStateHeap* mSearch;
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

    int size(); 
    int top(); // Animation 
    void push ( int val ); // Animation 
    void remove ( int idx ); // Animation
    void clear(); // Animation

    // State
    void setState(IStateHeap* state);
    IStateHeap* getState();
    IStateHeap* getPush();
    IStateHeap* getRemove();
    IStateHeap* getTop();
    IStateHeap* getClear();
    IStateHeap* getWaiting();
    IStateHeap* getInitialize();
    IStateHeap* getSearch();

    void draw();
    void update();
    void reset();
};

class State{
public:
    int blinkingStep;
    int beginLine;
    int endLine;
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

    State();
    ~State();
};

class Push : public IStateHeap{
public:
    int val = -1;
    int beginLine = -1;
    int endLine = -1;
    MinHeap *mHeap;
    int currentStep = -1;
    bool isAnimating = false;
    float blinkTime1 = 0.0f;
    float blinkTime2 = 0.0f;

    bool isUndoing = false;
    bool isRedoing = false;
    std::stack<State> undoStack;
    std::stack<State> redoStack;
    void getState(State &state);
    void saveState();
    void handleUndo();
    void handleRedo();

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
    void reset() override;

    void handleInsert(int val);
    void updateTreeStructure();
    void updateInsert();
    void handleBubbleUp();
    void drawBlinkingNode(float duration);
    void updateBubbleUp();
};

class Remove : public IStateHeap{
private:
    int val;
    int beginLine = -1;
    int endLine = -1;
    MinHeap *mHeap;
    int currentStep = -1;
    bool isAnimating = false;
    float blinkTime1 = 0.0f;
    float blinkTime2 = 0.0f;
    int blinkingStep = -1;
public:
    bool isUndoing = false;
    bool isRedoing = false;
    std::stack<State> undoStack;
    std::stack<State> redoStack;
    void saveState();
    void handleUndo();
    void handleRedo();
    void getState(State &state);

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
    void reset() override;

    void handleRemove(int idx);
    void updateRemove();
    void updateTreeStructure();
    void handleBubbleDown();
    void drawBlinkingNode(float duration);
    void updateBubbleDown();
};

class ClearH : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    ClearH(MinHeap* heap);
    void draw() override;
    void update() override;
    void reset() override;
};

class Top : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    Top(MinHeap* heap);
    void draw() override;
    void update() override;
    void reset() override;
};

class Initialize : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    Initialize(MinHeap* heap);
    void handleInputFile();
    void draw() override;
    void update() override;
    void reset() override;
};

class Search : public IStateHeap{
public:
    MinHeap *mHeap;
    int currentStep = -1;
    int animatingIdx = -1;
    int targetIdx = -1;
    float blinkTime = 0.0f;
    int beginLine = -1;
    int endLine = -1;

    bool isUndoing = false;
    bool isRedoing = false;
    std::stack<State> undoStack;
    std::stack<State> redoStack;
    void saveState();
    void handleUndo();
    void handleRedo();
    void getState(State &state);
public:
    Search(MinHeap* heap);
    void draw() override;
    void update() override;
    void reset() override;
};

class Waiting : public IStateHeap{
private:
    MinHeap *mHeap;
public:
    Waiting(MinHeap* heap);
    void draw() override;
    void update() override;
    void reset() override;
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
void DrawNode(Vector2 pos, const std::string& text);
void clearState(std::stack<State> &a, std::stack<State> &b);