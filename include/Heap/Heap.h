#pragma once
#include "GlobalVar.h"
#include <fstream>
#include <vector>
#include <raylib.h>
#include <utility>
#include "LinkedList.h"

struct HeapNode{

};

class MinHeap{
public:
    std::vector<int> tree;
    IState* mPush;
    IState* mRemove;
    IState* mTop;
    IState* mSize;
    IState* mClear;
    IState* mCurrent;
    IState* mWaiting;
    
    MinHeap();
    ~MinHeap();

    // Hàm tiện ích cho heap 
    int parent ( int i );
    int left ( int i );
    int right ( int i );
    void pop();
    void downHeap ( int idx );
    void upHeap ( int idx );

    int size(); // Animation
    int top(); // Animation 
    void push ( int val ); // Animation 
    void remove ( int idx ); // Animation
    void fileInput ( std::ifstream& fin ); // Animation 
    void clear(); // Animation

    // State
    void setState(IState* state);
    IState* getPush();
    IState* getRemove();
    IState* getTop();
    IState* getSize();
    IState* getClear();

    void draw();
    void update();
};


class Push : public IState{

};

// Drawing function
void DrawNode ( Vector2 pos, const std::string& txt, float radius );

void DrawArrow( Vector2 start, Vector2 end, float thickness, Color color );

void DrawHeapHelper ( int &idx, int &x, int &y, MinHeap& heap, int &nodeRadius, int &verticalSpacing, int &horizontalSpacing );











