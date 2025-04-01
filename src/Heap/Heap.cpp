#include "Heap.h"

MinHeap::MinHeap(){
    tree = std::vector<int>();
    mPush = new Push(this);
    mRemove = new Remove(this);
    // mTop = new Top(this);
    // mSize = new Size(this);
    mClear = new ClearH(this);
    mCurrent = mPush;
}

MinHeap::~MinHeap(){
    if ( mPush ) delete mPush;
    if ( mRemove ) delete mRemove;
    if ( mTop ) delete mTop;
    if ( mSize ) delete mSize;
    if ( mClear ) delete mClear;
    if ( mCurrent ) delete mCurrent;
    if ( mWaiting ) delete mWaiting;

    mPush = nullptr;
    mRemove = nullptr;
    mTop = nullptr;
    mSize = nullptr;
    mClear = nullptr;
    mCurrent = nullptr;
    mWaiting = nullptr;

    tree.clear();
}

int MinHeap::size(){ return tree.size(); }
int MinHeap::parent ( int i ){ return ( i - 1 ) / 2; }
int MinHeap::left ( int i ){ return 2 * i + 1; }
int MinHeap::right ( int i ){ return 2 * i + 2; }
int MinHeap::top(){ return tree[0]; }
void MinHeap::pop(){ remove(0); }
void MinHeap::clear(){ tree.clear(); }

void MinHeap::push ( int val ){
    tree.push_back(val);
    upHeap(tree.size() - 1);
}

void MinHeap::remove ( int idx ){
    if ( tree.empty() ) return;
    std::swap(tree[idx], tree[tree.size() - 1]);
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

int MinHeap::search ( int val ){
    for ( int i = 0; i < size(); i++ )
        if ( tree[i] == val )
            return i;
    return -1;
}

void MinHeap::fileInput(std::ifstream& fin){
    int temp;
    while ( fin >> temp ){
        push(temp);
    }
}   

void MinHeap::setState(IStateHeap* state) { mCurrent = state; }
IStateHeap* MinHeap::getPush(){ return mPush; }
IStateHeap* MinHeap::getRemove(){ return mRemove; }
IStateHeap* MinHeap::getTop(){ return mTop; }
IStateHeap* MinHeap::getSize(){ return mSize; }
IStateHeap* MinHeap::getClear(){ return mClear; }

Vector2 calculateArrowPosition ( Vector2 &start, Vector2 &end, const float &radius ){
    Vector2 direction = Vector2Subtract(end, start);
    float length = Vector2Length(direction);
    Vector2 normalizedDirection = Vector2Scale(direction, radius / length);
    return Vector2Add(start, normalizedDirection);
}

Vector2 calculateNodePos ( Vector2 pos, int offsetY, int offsetX, int height){
    int horizontalSpacing = 60;
    int verticalSpacing = 60;
    int level = pos.y; // Current level of the node
    int nodesInLevel = pow(2, level); // Number of nodes in the current level
    int totalNodesInPrevLevels = pow(2, level) - 1; // Total nodes in all previous levels
    
    // Calculate horizontal spacing for the current level
    int levelSpacing = horizontalSpacing * pow(2, height - level - 1);
    
    // Calculate the x and y positions
    int pX = offsetX + (pos.x * levelSpacing) - ((nodesInLevel - 1) * levelSpacing / 2);
    int pY = offsetY + (level * verticalSpacing);
    
    return {pX, pY};
}

void drawHeap(std::vector<int>& tree) {
    if (tree.empty()) return;
    heapNode.clear();
    int nodeRadius = 20;
    int height = (int)log2(tree.size())+1;
    int offsetX = screenWidth / 2; 
    int offsetY = 100;
    for (int y = 0; y < height; y++) { // Iterate over levels
        for (int x = 0; x < std::pow(2, y); x++) { // Iterate over nodes in the level
            int temp = x + std::pow(2, y) - 1;
            if (temp >= tree.size()) break;
            Vector2 parentPos = calculateNodePos({x, y}, offsetY, offsetX, height);
            heapNode.push_back({tree[temp], parentPos, {x, y}});
            drawNode(parentPos, std::to_string(tree[temp]), nodeRadius); // Correct index calculation
            if ( y < height - 1 ){
                if ( 2 * temp + 1 < tree.size() ){
                    int leftChildX = 2 * x;
                    int leftChildY = y + 1;
                    Vector2 leftChildPos = calculateNodePos({leftChildX, leftChildY}, offsetY, offsetX, height);
                    Vector2 arrowStart = calculateArrowPosition(parentPos, leftChildPos, nodeRadius);
                    Vector2 arrowEnd = calculateArrowPosition(leftChildPos, parentPos, nodeRadius);
                    drawArrow(arrowStart, arrowEnd, BLACK);
                }
                if ( 2 * temp + 2 < tree.size() ){
                    int rightChildX = 2 * x + 1; // Horizontal position of right child
                    int rightChildY = y + 1; // Vertical position of right child
                    Vector2 rightChildPos = calculateNodePos({rightChildX, rightChildY}, offsetY, offsetX, height);
                    Vector2 arrowStart = calculateArrowPosition(parentPos, rightChildPos, nodeRadius);
                    Vector2 arrowEnd = calculateArrowPosition(rightChildPos, parentPos, nodeRadius);
                    drawArrow(arrowStart, arrowEnd, BLACK);
                }
            }
        }
    }
}

Vector2 Vector2Normalize(Vector2 v) {
    float length = sqrt(v.x * v.x + v.y * v.y);
    if (length == 0) return {0, 0};
    return {v.x / length, v.y / length};
}