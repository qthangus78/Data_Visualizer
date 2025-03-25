#include "Heap.h"

MinHeap::MinHeap(){
    tree = std::vector<int>();
    // mPush = new Push(this);
    // mRemove = new Remove(this);
    // mTop = new Top(this);
    // mSize = new Size(this);
    // mClear = new Clear(this);
    mCurrent = mWaiting;
}

MinHeap::~MinHeap(){
    delete mPush;
    delete mRemove;
    delete mTop;
    delete mSize;
    delete mClear;
    delete mCurrent;
    delete mWaiting;

    tree.clear();
}

int MinHeap::size(){
    return tree.size();
}

int MinHeap::parent ( int i ){
    return ( i - 1 ) / 2;
}

int MinHeap::left ( int i ){
    return 2 * i + 1;
}

int MinHeap::right ( int i ){
    return 2 * i + 2;
}

int MinHeap::top(){
    return tree[0];
}

void MinHeap::pop(){
    remove(0);
}

void MinHeap::push ( int val ){
    tree.push_back(val);
    upHeap(tree.size() - 1);
}

void MinHeap::remove ( int idx ){
    tree[idx] = tree[tree.size() - 1];
    tree.pop_back();
    downHeap(idx);
}

void MinHeap::downHeap ( int idx ){
    int n = size();
    int l = left(idx);
    int r = right(idx);
    int pos = idx;
    while ( pos < n ){
        if ( l < n && tree[l] < tree[pos] ){
            pos = l;
        }
        if ( r < n && tree[r] < tree[pos] ){
            pos = r;
        }
        if ( pos != idx ){
            std::swap(tree[idx], tree[pos]);
            idx = pos;
        }
        else break;
    }
}

void MinHeap::upHeap ( int idx ){
    while ( idx > 0 ){
        int p = parent(idx);
        if ( tree[p] > tree[idx] ){
            std::swap(tree[p], tree[idx]);
            idx = p;
        }
        else break;
    }
}

void MinHeap::fileInput(std::ifstream& fin){
    int temp;
    while ( fin >> temp ){
        push(temp);
    }
}   

void MinHeap::clear(){
    tree.clear();
}

void MinHeap::setState(IState* state) { mCurrent = state; }
IState* MinHeap::getPush(){ return mPush; }
IState* MinHeap::getRemove(){ return mRemove; }
IState* MinHeap::getTop(){ return mTop; }
IState* MinHeap::getSize(){ return mSize; }
IState* MinHeap::getClear(){ return mClear; }

void DrawNode( Vector2 pos, const std::string& txt, float radius ){
    DrawCircle( pos.x, pos.y, radius, BLUE );
    DrawText ( txt.c_str(), pos.x - 10, pos.y - 10, 20, WHITE );
}

void DrawArrow(Vector2 start, Vector2 end, float thickness, Color color) {
    DrawLineEx(start, end, thickness, color);
    float angle = atan2(end.y - start.y, end.x - start.x);
    float arrowSize = 10.0f;
    Vector2 arrowPoint1 = { end.x - arrowSize * cos(angle - PI / 6), end.y - arrowSize * sin(angle - PI / 6) };
    Vector2 arrowPoint2 = { end.x - arrowSize * cos(angle + PI / 6), end.y - arrowSize * sin(angle + PI / 6) };
    DrawTriangle(end, arrowPoint1, arrowPoint2, color);
}

