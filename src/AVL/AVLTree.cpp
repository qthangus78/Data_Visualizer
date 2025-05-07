#include <raylib.h>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include "AVLTree.h"
#include "GlobalVar.h"
//AVL Structure
int getHeight(Node* node) {return node ? node->height : 0;}
int getBalance(Node* node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}
Node* findParent(Node*& pRoot, Node*& y) {
    if (pRoot == y) return nullptr;
    Node* parent = nullptr;
    Node* curr   = pRoot;
    while (curr && curr != y) {
        parent = curr;
        if (y->data < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return parent;
}
Node* rightRotate(Node*& pRoot, Node*& y, std::vector<EdgeAVL>& edge) {
    Node* parent = findParent(pRoot, y);   
    Node* x      = y->left;
    Node* xRight = x->right;
    // 1) Cập nhật con trỏ của parent, hoặc cập nhật gốc nếu y là root
    if (parent) {
        if (parent->left == y)  parent->left  = x;
        else                    parent->right = x;
    } else {
        pRoot = x;
    }
    // 2) Thực hiện xoay
    x->right = y;
    y->left  = xRight;
    // 3) Cập nhật các EdgeAVL như trước…
    EdgeAVL& par = findEdge(parent,y,edge);
    par.child = x; par.color = BLACK;
    EdgeAVL& xy = findEdge(y, x, edge);
    xy.par   = x;  xy.child = y;  xy.color = BLACK;
    if (xRight) {
        EdgeAVL& yr = findEdge(x, xRight, edge);
        yr.par   = y;  yr.child = xRight;  yr.color = BLACK;
    }
    // 4) Cập nhật height
    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
    return x;
}
Node* leftRotate(Node*& pRoot, Node*& y, std::vector<EdgeAVL>& edge) {
    Node* parent = findParent(pRoot, y);
    Node* x      = y->right;
    Node* xLeft  = x->left;
    if (parent) {
        if (parent->left == y)  parent->left  = x;
        else                    parent->right = x;
    } else {
        pRoot = x;
    }
    x->left  = y;
    y->right = xLeft;
    EdgeAVL& par = findEdge(parent,y,edge);
    par.child = x; par.color = BLACK;
    EdgeAVL& xy = findEdge(y, x, edge);
    xy.par   = x;  xy.child = y;  xy.color = BLACK;
    if (xLeft) {
        EdgeAVL& yl = findEdge(x, xLeft, edge);
        yl.par   = y;  yl.child = xLeft;  yl.color = BLACK;
    }
    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
    return x;
}
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* xRight = x->right;
    x->right = y;
    y->left = xRight;
    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
    return x;
}
Node* leftRotate(Node* y) {
    Node* x = y->right;
    Node* xLeft = x->left;
    x->left = y;
    y->right = xLeft;
    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
    return x;
}
Node* insertNode(Node* pRoot, int x) {
    if (!pRoot)
        return new Node(x, posRoot, NodeSizeAVL, FontNodeAVL);
    if (x < pRoot->data)
        pRoot->left = insertNode(pRoot->left, x);
    else if (x > pRoot->data)
        pRoot->right = insertNode(pRoot->right, x);
    else
        return pRoot; 
    pRoot->height = 1 + std::max(getHeight(pRoot->left), getHeight(pRoot->right));
    int balance = getBalance(pRoot);
    if (balance > 1 && x < pRoot->left->data)
        return rightRotate(pRoot);
    if (balance < -1 && x > pRoot->right->data)
        return leftRotate(pRoot);
    if (balance > 1 && x > pRoot->left->data) {
        pRoot->left = leftRotate(pRoot->left);
        return rightRotate(pRoot);
    }
    if (balance < -1 && x < pRoot->right->data) {
        pRoot->right = rightRotate(pRoot->right);
        return leftRotate(pRoot);
    }
    return pRoot;
}
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left) current = current->left;
    return current;
}
Node* deleteNode(Node* root, int key) {
    if (!root) return nullptr;
    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }
    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
void clearAVL(Node*& pRoot) {
    if (pRoot == nullptr) {
        return;
    }
    clearAVL(pRoot->left);
    clearAVL(pRoot->right);
    delete pRoot;
    pRoot = nullptr; 
}
void FileInput(std::ifstream& fin, Node*& pRoot) {
    int t;
    while (fin >> t) {
        pRoot = insertNode(pRoot, t);
    }
}
//AVL 
AVL::AVL() : pRoot(nullptr) {
    mNotInMode = new notInModeAVL(this);
    mCreate = new CreateAVL(this);
    mInsert = new InsertAVL(this);
    mDelete = new DeleteAVL(this);
    mFind = new FindAVL(this);
    mClear = new ClearAVL(this);
    mCurrent = mNotInMode;    
}
AVL::~AVL() {
    delete mNotInMode;
    delete mCreate;
    delete mInsert;
    delete mDelete;
    delete mFind;
    delete mClear;
    clearAVL(pRoot);
    mCurrent = nullptr;   
}
Node*& AVL::getRoot() {return pRoot;}
IStateAVL* AVL::getCurrent() {return mCurrent;}
IStateAVL* AVL::getNotInMode() {return mNotInMode;}
IStateAVL* AVL::getCreate() {return mCreate;}
IStateAVL* AVL::getInsert() {return mInsert;}
IStateAVL* AVL::getDelete() {return mDelete;}
IStateAVL* AVL::getFind() {return mFind;}
IStateAVL* AVL::getClear() {return mClear;}
void AVL::setState(IStateAVL* state) {mCurrent = state;}
std::vector<EdgeAVL>& AVL::getEdge() {return edge;}
void AVL::pushEdge(EdgeAVL newEdge) {edge.push_back(newEdge);}
std::vector<Node*> AVL::BFSOrder(){
    std::vector<Node*> order;
    std::queue<Node*> q;
    q.push(pRoot);
    while(!q.empty()){
        auto node = q.front();
        q.pop();
        order.push_back(node);
        if(node->left) q.push(node->left);
        if(node->right) q.push(node->right);
    }  
    return order;
}
void AVL::draw() {mCurrent->draw();}
void AVL::handle() {mCurrent->handle();}
//Reuseable utility
//DrawNode
void DrawNodeAVL(Node* node){
    if(!node) return;
    DrawCircleV(node->pos,node->radius,node->color);
    std::string text = std::to_string(node->data);
    Vector2 textSize = MeasureTextEx(SSLFont, text.c_str(), node->font+1, 2);
    DrawTextEx(SSLFont, text.c_str(),{node->pos.x-textSize.x/2, node->pos.y-textSize.y/2},node->font, 2, node->textColor);
}
//Draw Edge
void DrawEdge(EdgeAVL edge){
    Vector2 start = edge.par->pos;
    Vector2 end = edge.child->pos;
    DrawLineEx(start,end,2.0f,edge.color);
}
//Draw animation down tree
void DrawEdgeColor(Vector2 start, Vector2 end, float& progress, Color color) {
    Vector2 dir = Vector2SubtractAVL(end, start);
    float totalLength = Vector2LengthAVL(dir);
    if (totalLength <= NodeSizeAVL) return; 
    Vector2 unitDir = Vector2ScaleAVL(dir, 1.0f / totalLength);
    Vector2 target = Vector2SubtractAVL(end, Vector2ScaleAVL(unitDir, NodeSizeAVL));
    // Nội suy vị trí hiện tại theo progress
    if (progress >= 1.0f) {
        DrawLineEx(start, target, 3.0f, color);
        return;
    }
    Vector2 current = lerpAVL(start, target, progress);
    DrawLineEx(start, current, 3.0f, color);
    progress += 2*deltaTimeAVL;
}
//Find Edge
EdgeAVL& findEdge(Node* par, Node* child, std::vector<EdgeAVL>& edge) {
    for (auto& e : edge) {
        if (e.par == par && e.child == child) 
            return e;
    }
    static EdgeAVL dummy{ BLACK, nullptr, nullptr };
    return dummy;
}
//Draw the whole AVL Tree
void DrawAVLTree(Node* pRoot, std::vector<EdgeAVL>& edge){
    if(!pRoot) return;
    if(!edge.empty()) {
        for(auto x : edge) DrawEdge(x);
    }
    std::queue<Node*> q;
    q.push(pRoot);
    while(!q.empty()){
        auto u = q.front();
        q.pop();
        DrawNodeAVL(u);
        if(u->left) q.push(u->left);
        if(u->right) q.push(u->right);
    }
}
//Draw Button
void DrawButtonAVL(Rectangle rect, const std::string &text, Color &color, Font font, float fontsize) {
    Color drawColor = color;
    DrawRectangleRounded(rect, 0.3f, 12, drawColor);
    DrawRectangleRoundedLinesEx(rect, 0.3f, 12, 2.0f, BLACK);
    int fontSize = fontsize;
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 2);
    Vector2 textPos = { rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2 };
    DrawTextEx(font, text.c_str(), textPos, fontSize, 2, BLACK);
}
void drawButtonAVL(){
    DrawButtonAVL(buttonVar::buttonCreate.rect, buttonVar::buttonCreate.text, buttonVar::buttonCreate.buCol, SSLFont,22);
    DrawButtonAVL(buttonVar::buttonIns.rect, buttonVar::buttonIns.text, buttonVar::buttonIns.buCol, SSLFont,22);
    DrawButtonAVL(buttonVar::buttonDel.rect, buttonVar::buttonDel.text, buttonVar::buttonDel.buCol, SSLFont,22);
    DrawButtonAVL(buttonVar::buttonF.rect, buttonVar::buttonF.text, buttonVar::buttonF.buCol, SSLFont,22);
    DrawButtonAVL(buttonVar::buttonClear.rect, buttonVar::buttonClear.text, buttonVar::buttonClear.buCol, SSLFont,22);
}
//Handle Button
void handleButtonsHoverAVL(){
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect))
        buttonVar::buttonIns.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect))
        buttonVar::buttonCreate.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect))
        buttonVar::buttonDel.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect))
        buttonVar::buttonF.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect))
        buttonVar::buttonClear.buCol     = color::buttonColorHovered;
    else {
        buttonVar::buttonCreate.buCol    = color::buttonColor;
        buttonVar::buttonIns.buCol    = color::buttonColor;
        buttonVar::buttonDel.buCol = color::buttonColor;
        buttonVar::buttonF.buCol = color::buttonColor;
        buttonVar::buttonClear.buCol = color::buttonColor;
    }
}
void handleButtonsClickAVL(AVL* AVL){
    if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        AVL->setState((AVL->getCurrent() == AVL->getCreate()) ? AVL->getNotInMode() : AVL->getCreate());
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        AVL->setState((AVL->getCurrent() == AVL->getInsert()) ? AVL->getNotInMode() : AVL->getInsert());
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        AVL->setState((AVL->getCurrent() == AVL->getDelete()) ? AVL->getNotInMode() : AVL->getDelete());
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        AVL->setState((AVL->getCurrent() == AVL->getFind()) ? AVL->getNotInMode() : AVL->getFind());
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        AVL->setState((AVL->getCurrent() == AVL->getClear()) ? AVL->getNotInMode() : AVL->getClear());
}
void drawBlinkingLinesAVL(const std::string& text, Rectangle rect, int& frameCounter){
    frameCounter++;
    if(frameCounter < 30) {
        int width = MeasureTextEx(SSLFont, text.c_str(), rect.height-10, 1).x;
        float cursorX = rect.x + width + 7;
        DrawLine(cursorX, rect.y + 2, cursorX, rect.y + rect.height - 2, BLACK);
    }
    if(frameCounter >= 60) frameCounter = 0;
}
void drawTextInAVL(const std::string& text, Rectangle rect, int& frameCounter){
    float fontsize = button::sizeH - 10;
    float spacing = 1.0f;
    Vector2 pos = { rect.x + 5, rect.y + 5 };
    std::string str = text;
    Vector2 widthText = MeasureTextEx(SSLFont, str.c_str(), fontsize, spacing);
    while (widthText.x > rect.width-5 && !str.empty()) {
    str.erase(str.begin());  
    widthText = MeasureTextEx(SSLFont, str.c_str(), fontsize, spacing);
    }
    DrawTextEx(SSLFont, str.c_str(), pos, fontsize, spacing, BLACK);
    drawBlinkingLinesAVL(str, rect, frameCounter);
}
//Tính toán lại khoảng cách cho layout
std::vector<Vector2> calposAVLTree(Node*& pRoot){
    if(!pRoot) return {};
    std::vector<Vector2> pos;
    std::queue<std::tuple<Node*,int,Vector2>> q;
    q.push({ pRoot, 1, posRoot });
    int maxH = getHeight(pRoot);
    while(!q.empty()){
        auto [curr, level, currPos] = q.front();
        q.pop();
        pos.push_back(currPos);
        int rem    = maxH - level - 1;
        int offset = (rem < 0) ? 1 : pow(2.0f,rem);
        if(curr->left){
            Vector2 leftPos  = { currPos.x - offset*dis, currPos.y + dis };
            q.push({ curr->left,  level+1, leftPos  });
        }
        if(curr->right){
            Vector2 rightPos = { currPos.x + offset*dis, currPos.y + dis };
            q.push({ curr->right, level+1, rightPos });
        }
    }
    return pos;
}
void handlePos(AVL*& AVL, std::vector<Vector2>& oldPos, std::vector<Vector2>& newPos){
    std::vector<Node*> node = AVL->BFSOrder();
    oldPos.clear(); newPos.clear();
    newPos = calposAVLTree(AVL->getRoot());
    for(auto& u : node) oldPos.push_back(u->pos);
}
//Animation
float smoothstepAVL(float t){
    if(t >= 1) return 1;
    if(t<=0) return 0;
    return t*t*(3-2*t);
}
Vector2 lerpAVL(Vector2 start, Vector2 end, float progress){
    float t = smoothstepAVL(progress);
    return {(start.x+t*(end.x-start.x)),(start.y+t*(end.y-start.y))};
}
float lerpAVL(float start, float end, float progress){
    float t = smoothstepAVL(progress);
    return start+t*(end-start);
}
Color lerpAVL(Color start, Color end, float progress) {
    float t = smoothstepAVL(progress);
    unsigned char r = static_cast<unsigned char>(start.r + t * (end.r - start.r));
    unsigned char g = static_cast<unsigned char>(start.g + t * (end.g - start.g));
    unsigned char b = static_cast<unsigned char>(start.b + t * (end.b - start.b));
    unsigned char a = static_cast<unsigned char>(start.a + t * (end.a - start.a));
    return {r, g, b, a};
}
void newNodeAVL(Node*& node, float& progress){
    progress += deltaTimeAVL;
    node->radius = lerpAVL(0,NodeSizeAVL,progress);
    node->font = lerpAVL(0,FontNodeAVL,progress);
}
//Handle distance
Vector2 Vector2SubtractAVL(Vector2 v1, Vector2 v2) {
    return (Vector2){ v1.x - v2.x, v1.y - v2.y };
}
float Vector2LengthAVL(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}
Vector2 Vector2AddAVL(Vector2 v1, Vector2 v2) {
    return (Vector2){ v1.x + v2.x, v1.y + v2.y };
}
Vector2 Vector2ScaleAVL(Vector2 v, float scale) {
    return (Vector2){ v.x * scale, v.y * scale };
}
