#include "Heap.h"

bool valueFound = false;
bool isStepbystep = false;
bool isPaused = false;
float blinkTime = 0.0f;
float txtBlinkTime = 0.0f;
int offsetX = 912;
int offsetY = 200;
int nodeRadius = 20;
float duration = 1.0f;
PseudoCode yes;
SpeedButtonSpinner speedButton;
ButtonManager buttons;
AnnouncementBox pseudoCode;
std::vector<float> elapsedTime ( 31, 0.0f );
std::vector<HeapNode> targetHeapNode ( 31, {0, {0, 0}, false} );
std::vector<HeapNode> originHeapNode ( 31, {0, {0, 0}, false} );
std::vector<HeapNode> heapNode ( 31, {0, {0, 0}, false} );

MinHeap::MinHeap(){
    pseudoCode = AnnouncementBox({5, 60, 420, 320 }, "");
    speedButton.Init({500, 100});
    tree = std::vector<int> ( 0 );
    mPush = new Push(this);
    mRemove = new Remove(this);
    mTop = new Top(this);
    mClear = new ClearH(this);
    mInitialize = new Initialize(this);
    mWaiting = new Waiting(this);
    mSearch = new Search(this);
    mCurrent = mWaiting;
}

MinHeap::~MinHeap(){
    if ( mInitialize ) delete mInitialize;
    if ( mPush ) delete mPush;
    if ( mRemove ) delete mRemove;
    if ( mTop ) delete mTop;
    if ( mClear ) delete mClear;
    if ( mCurrent ) delete mCurrent;
    if ( mWaiting ) delete mWaiting;
    if ( mSearch ) delete mSearch;

    mInitialize = nullptr;
    mPush = nullptr;
    mRemove = nullptr;
    mTop = nullptr;
    mClear = nullptr;
    mCurrent = nullptr;
    mWaiting = nullptr;
    mSearch = nullptr;

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
    int idx = tree.size() - 1;
    if ( !heapNode[idx].exist ){
        float x = idx - ( 1 << (int)log2(idx + 1)) + 1;
        float y = (int)log2(idx + 1);
        int height = (int)log2(tree.size())+1;
        Vector2 pos = calculateNodePos ( {x, y}, height );
        heapNode[idx] = { val, pos, true };
    }
    upHeap(tree.size() - 1);
}

void MinHeap::remove ( int idx ){
    if ( tree.empty() ) return;
    int lastIdx = tree.size() - 1;
    std::swap(tree[idx], tree[lastIdx]);
    swapHeapNode(heapNode[idx], heapNode[lastIdx]);
    tree.pop_back();
    heapNode[lastIdx] = {0, {0, 0}, false};
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
            swapHeapNode(heapNode[idx], heapNode[pos]);
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
            swapHeapNode(heapNode[idx], heapNode[p]);
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

void MinHeap::setState(IStateHeap* state) { 
    if ( mCurrent )
        mCurrent->reset();
    mCurrent = state; 
}
IStateHeap* MinHeap::getState() { return mCurrent; }
IStateHeap* MinHeap::getPush(){ return mPush; }
IStateHeap* MinHeap::getRemove(){ return mRemove; }
IStateHeap* MinHeap::getTop(){ return mTop; }
IStateHeap* MinHeap::getClear(){ return mClear; }
IStateHeap* MinHeap::getInitialize(){ return mInitialize; }
IStateHeap* MinHeap::getSearch(){ return mSearch; }
IStateHeap* MinHeap::getWaiting(){ return mWaiting; }

Vector2 calculateArrowPosition ( Vector2 &start, Vector2 &end, const float &radius ){
    Vector2 direction = Vector2Subtract(end, start);
    float length = Vector2Length(direction);
    Vector2 normalizedDirection = Vector2Scale(direction, radius / length);
    return Vector2Add(start, normalizedDirection);
}

Vector2 calculateNodePos ( Vector2 pos, int height){
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
    
    return {float(pX), float(pY)};
}

void drawHeap(std::vector<int>& tree) {
    if (tree.empty()) return;

    float height = (int)log2(tree.size())+1;

    for (float y = 0; y < height; y++) { // Iterate over levels
        for (float x = 0; x < std::pow(2, y); x++) { // Iterate over nodes in the level
            int temp = x + std::pow(2, y) - 1;
            if (temp >= tree.size()) break;

            Vector2 parentPos = calculateNodePos({x, y}, height);

            DrawNode(parentPos, std::to_string(tree[temp]));
            
            if ( y < height - 1 ){
                if ( 2 * temp + 1 < tree.size() ){
                    float leftChildX = 2.0f * x;
                    float leftChildY = y + 1.0;
                    Vector2 leftChildPos = calculateNodePos({leftChildX, leftChildY}, height);
                    Vector2 arrowStart = calculateArrowPosition(parentPos, leftChildPos, nodeRadius);
                    Vector2 arrowEnd = calculateArrowPosition(leftChildPos, parentPos, nodeRadius);
                    drawArrow(arrowStart, arrowEnd, BLACK);
                }
                if ( 2 * temp + 2 < tree.size() ){
                    float rightChildX = 2.0f * x + 1; // Horizontal position of right child
                    float rightChildY = y + 1.0f; // Vertical position of right child
                    Vector2 rightChildPos = calculateNodePos({rightChildX, rightChildY}, height);
                    Vector2 arrowStart = calculateArrowPosition(parentPos, rightChildPos, nodeRadius);
                    Vector2 arrowEnd = calculateArrowPosition(rightChildPos, parentPos, nodeRadius);
                    drawArrow(arrowStart, arrowEnd, BLACK);
                }
            }
        }
    }
}

void swapHeapNode(HeapNode &a, HeapNode &b){
    std::swap(a.val, b.val);
    std::swap(a.exist, b.exist);
}

void recalculateAllNodePos ( MinHeap* mHeap ){
    float height = (int)log2(mHeap->size()) + 1;
    for (int i = 0; i < mHeap->size(); i++) {
        float x = i - (1 << (int)log2(i + 1)) + 1;
        float y = (int)log2(i + 1);
        heapNode[i].pos = calculateNodePos({x, y}, height);
    }
}

void updateNodePos ( Vector2 &animatingPos, Vector2 targetPos, Vector2 originPos, float duration, bool &isAnimating, int i ){
    float dt = GetFrameTime();
    elapsedTime[i] += dt;
    float t = elapsedTime[i] / duration;
    if ( t > 1.0f ){
        animatingPos = targetPos;
        t = 1.0f;
        elapsedTime[i] = 0.0f;
        isAnimating = false;
    }
    animatingPos = { 
        originPos.x + (targetPos.x - originPos.x) * t, 
        originPos.y + (targetPos.y - originPos.y) * t 
    };
}

void DrawPartOfHeap ( MinHeap* mHeap, int animatingIdx, int parentIdx, bool isAnimating, int currentStep ){
    for ( int i = 0; i < mHeap->size(); i++ ){
        int height = (int)log2(mHeap->size()) + 1;
        int y = (int)log2(i + 1);

        if ( isAnimating ){
            if ( currentStep == 0 || currentStep == 1 )
                if ( i != animatingIdx )
                    DrawNode(heapNode[i].pos, to_string(heapNode[i].val));
            if ( currentStep == 2 )
                DrawNode(heapNode[i].pos, to_string(heapNode[i].val));
            if ( currentStep == 4 )
                if ( i != animatingIdx && i != parentIdx )
                    DrawNode(heapNode[i].pos, to_string(heapNode[i].val));
        }
        else
            DrawNode(heapNode[i].pos, to_string(heapNode[i].val));

        if ( y < height - 1 ){
            int leftChild = 2*i + 1;
            if ( leftChild < mHeap->size() ){
                Vector2 arrowStart = calculateArrowPosition(heapNode[i].pos, heapNode[leftChild].pos, nodeRadius);
                Vector2 arrowEnd = calculateArrowPosition(heapNode[leftChild].pos, heapNode[i].pos, nodeRadius);
                drawArrow(arrowStart, arrowEnd, BLACK);
            }
            int rightChild = 2*i + 2;
            if ( rightChild < mHeap->size() ){
                Vector2 arrowStart = calculateArrowPosition(heapNode[i].pos, heapNode[rightChild].pos, nodeRadius);
                Vector2 arrowEnd = calculateArrowPosition(heapNode[rightChild].pos, heapNode[i].pos, nodeRadius);
                drawArrow(arrowStart, arrowEnd, BLACK);
            }
        }
    }
}

void DrawBlinkingNode(Vector2 pos, int val, float &blinkTime){
    blinkTime += GetFrameTime();
    if ((int)(blinkTime * 2) % 2 == 0) { // Blink every 0.5 seconds
        DrawCircle( pos.x, pos.y, nodeRadius, YELLOW);
        string value = to_string(val);
        Vector2 textSize = MeasureTextEx(customFont, value.c_str(), 23, 2);
        DrawTextEx(customFont, value.c_str() ,{pos.x - textSize.x / 2, pos.y - textSize.y / 2}, 22, 2, BLACK);
    }
}

void DrawNode(Vector2 pos, const std::string& text){
    DrawCircleV( pos, 20, BLUE );
    Vector2 textSize = MeasureTextEx(SSLFont, text.c_str(), FontNode+1, 2);
    DrawTextEx(SSLFont, text.c_str(),{pos.x - textSize.x / 2, pos.y - textSize.y / 2}, FontNode, 2, BLACK);
}

void clearState(std::stack<State> &a, std::stack<State> &b){
    while ( !a.empty() ) a.pop();
    while ( !b.empty() ) b.pop();
}