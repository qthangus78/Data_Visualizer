#include "LinkedList.h"
#include "GlobalVar.h"
#include <raylib.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//--------------------------------
// ListNode
//--------------------------------
ListNode::ListNode(int x) : data(x), next(nullptr) {}

void insertAl(ListNode*& root, ListNode*& tail, int x) {
    if (!root) {
        root = new ListNode(x);
        tail = root;
        return;
    }
    tail->next = new ListNode(x); 
    tail = tail->next; 
}

bool delAl(ListNode*& root, ListNode*& tail, int x) {
    if (!root) return false;
    ListNode* cur = root;
    ListNode* prev = nullptr;
    while (cur) {
        if (cur->data == x) {
            if (!prev) {  
                root = cur->next;
                if (!root) tail = nullptr;
                delete cur;
                return true;
            } else {  
                prev->next = cur->next;
                if (!cur->next) tail = prev; 
                delete cur;
                return true;
            }
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

ListNode* findAl(ListNode*& root, int x) {
    ListNode* cur = root;
    while(cur) {
        if(cur->data == x) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void delMemAl(ListNode*& root) {
    while(root) {
        ListNode* tmp = root;
        root = root->next;
        delete tmp;
    }
}

void fileInputAl(ListNode*& root, ListNode*& tail, std::ifstream& fin){
    int numIns;
    while(fin >> numIns){
        insertAl(root,tail,numIns);
    }
}
//--------------------------------
// SSL
//--------------------------------
SSL::SSL()
 : root(nullptr), existVal(nullptr), mcurrent(nullptr), tail(nullptr)
{
    // Tạo các state
    mNotInMode = new notInMode(this);
    mInsert    = new Insert(this);
    mDelete    = new Delete(this);
    mFind      = new Find(this);
    mClear = new Clear(this);
    mcurrent   = mNotInMode;
}

SSL::~SSL(){
    // xóa state
    delete mNotInMode;
    delete mInsert;
    delete mDelete;
    delete mFind;
    delete mClear;
    // xóa list
    delMemAl(root);
}

void SSL::setState(IState* state) { mcurrent = state; }
IState* SSL::getnotInMode() { return mNotInMode; }
IState* SSL::getInsert()    { return mInsert; }
IState* SSL::getDel()       { return mDelete; }
IState* SSL::getFind()      { return mFind; }
IState* SSL::getClear()     {return mClear;}

ListNode* SSL::getRoot() { return root; }
ListNode* SSL::getTail() {return tail;}
ListNode* SSL::getprevTail() {return prevtail;}
void SSL::setprevTail(ListNode* APrev) {prevtail = APrev;}
void SSL::setRoot(ListNode* tmp) { root = tmp; }
void SSL::setExistVal(ListNode* tmp){ existVal = tmp; }
ListNode* SSL::getExistVal(){ return existVal; }
Vector2 SSL::getPosEnd() {return posEnd;}
void SSL::setPosEnd(Vector2 APos) {posEnd = APos;}

void SSL::insertList(int x){ insertAl(root,tail,x); }
bool SSL::delList(int x){ return delAl(root,tail,x); }
ListNode* SSL::findList(int x){ return findAl(root, x); }
void SSL::delAllList() {delMemAl(root); }
void SSL::fileInput(std::ifstream& fin) {fileInputAl(root,tail,fin);}

void SSL::draw(){
    if(mcurrent) mcurrent->draw();
}
void SSL::handle(){;
    if(mcurrent) mcurrent->handle();
}
//--------------------------------
// Hàm tiện ích
//--------------------------------
//Vẽ Node
void drawNode(Vector2 pos, const std::string& text, float radius){
    DrawCircleV(pos, radius, color::nodeNotInMode);
    Vector2 textSize = MeasureTextEx(customFont, text.c_str(), 23, 2);
    DrawTextEx(customFont, text.c_str(),{pos.x - textSize.x / 2, pos.y - textSize.y / 2}, 22, 2, BLACK);
}
//Vẽ mũi tên
Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
    return Vector2{ v1.x - v2.x, v1.y - v2.y };
}
float Vector2Length(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}
Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
    return Vector2{ v1.x + v2.x, v1.y + v2.y };
}
Vector2 Vector2Scale(Vector2 v, float scale) {
    return Vector2{ v.x * scale, v.y * scale };
}
void drawArrow(Vector2 start, Vector2 end, Color color) {
    Vector2 V = Vector2Subtract(end, start);
    float length = Vector2Length(V);
    if (length == 0) return;
    Vector2 V_unit = Vector2Scale(V, 1.0f / length);
    Vector2 V_perp = Vector2{ -V_unit.y, V_unit.x };
    float a = 10.0f;
    float b = 5.0f;  
    Vector2 temp1 = Vector2Scale(V_unit, -a);     
    Vector2 temp2 = Vector2Scale(V_perp, b);          
    Vector2 P1 = Vector2Add(end, Vector2Add(temp1, temp2));
    Vector2 P2 = Vector2Add(end, Vector2Subtract(temp1, temp2));
    DrawLineEx(start, end, EArrow.thickness, color);
    DrawLineEx(end, P1, EArrow.thickness, color);
    DrawLineEx(end, P2, EArrow.thickness, color);
}

void drawLinkedList(ListNode* root) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    const int rightMargin = 60;
    const int lineHeight = 100;
    int numLine = 0;
    while(root) {
        if(posStart.x + 2 * EArrow.length + 33 > GetScreenWidth() - rightMargin)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur  = { posStart.x + EArrow.length, posStart.y };
        if(cur.x > GetScreenWidth() - rightMargin) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + lineHeight * numLine };
        }
        if(root->next && cur.x > prev.x) {
            drawArrow(prev, cur, color::edgeNotInMode);
        }
        if(root->next && cur.x < prev.x){
            drawArrow2Node(prev,cur,color::edgeNotInMode);
        }
        std::string dataStr = std::to_string(root->data);
        drawNode(prev, dataStr, 33);
        if(!root->next) {
            if(cur.x < prev.x) {
                drawArrow(prev, cur, color::edgeNotInMode);
                drawNode(prev, dataStr, 33);
                DrawTextEx(customFont,"NULL", {cur.x - 10, cur.y + 13},25,2,BLUE);
            } else {
                drawArrow({ prev.x + 33, prev.y }, cur, color::edgeNotInMode);
                DrawTextEx(customFont,"NULL", {cur.x + 5, cur.y - 13}, 25, 2, BLUE);
            }
        }
        if(cur.x > posStart.x)
            posStart = { cur.x + 33, cur.y };
        else
            posStart = cur; 
        root = root->next;
    }
    for(const auto& u : shadedPos) {
        std::string textShaded = std::to_string(u.node->data);
        drawNode(u.pos, textShaded, 33);
    }
}

void drawPartofLinkedList(ListNode* root, ListNode* EndPart) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    const int rightMargin = 60;
    const int lineHeight = 100;
    int numLine = 0;
    while(root) {
        if(root == EndPart)
            return;
        if(posStart.x + 2 * EArrow.length + 33 > GetScreenWidth() - rightMargin)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur  = { posStart.x + EArrow.length, posStart.y };
        if(cur.x > GetScreenWidth() - rightMargin) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + lineHeight * numLine };
        }
        if(root->next && cur.x > prev.x && root->next!=EndPart) {
            drawArrow(prev, cur, color::edgeRendered);
        }
        if(root->next && cur.x < prev.x && root->next!=EndPart){
            drawArrow2Node(prev,cur,color::edgeRendered);
        }
        DrawCircleV(prev, 34, color::nodeRendered);
        std::string dataStr = std::to_string(root->data);
        drawNode(prev, dataStr, 30);
        if(cur.x > posStart.x)
            posStart = { cur.x + 33, cur.y };
        else
            posStart = cur;
        root = root->next;
    }
    // Vẽ lại các node trong shadedPos
    for(const auto& u : shadedPos) {
        DrawCircleV(u.pos, 34, color::nodeRendered);
        std::string text = std::to_string(u.node->data);
        drawNode(u.pos, text, 30);
    }
}
void drawPartofLinkedListNotColor(ListNode* root, ListNode* EndPart, SSL* s) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    const int rightMargin = 60;
    const int lineHeight = 100;
    int numLine = 0;
    while (root) {
        if (root == EndPart)
            return;
        if (posStart.x + 2 * EArrow.length + 33 > GetScreenWidth() - rightMargin)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur = { posStart.x + EArrow.length, posStart.y };
        if (cur.x > GetScreenWidth() - rightMargin) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + lineHeight * numLine };
        }
        if(root->next && cur.x > prev.x && root->next!=EndPart) {
            drawArrow(prev, cur, color::edgeNotInMode);
        }
        if(root->next && cur.x < prev.x && root->next!=EndPart){
            drawArrow2Node(prev,cur,color::edgeNotInMode);
        }
        std::string str = std::to_string(root->data);
        drawNode(prev, str, 33);
        if (root->next == EndPart) {
            s->setPosEnd(posStart);
            s->setprevTail(root);
        }
        if (cur.x > posStart.x)
            posStart = { cur.x + 33, cur.y };
        else 
            posStart = cur;
        root = root->next;
    }
    // Sau khi duyệt xong danh sách, vẽ lại các node lưu trong shadedPos
    for (const auto &u : shadedPos) {
        std::string textShaded = std::to_string(u.node->data);
        drawNode(u.pos, textShaded, 33);
    }
}
// Vẽ 3 nút Insert, Delete, Find
void drawButtons(){
    // Nút Insert
    DrawRectangleRounded(buttonVar::buttonIns.rect, 0.3f, 30, buttonVar::buttonIns.buCol);
    DrawTextEx(customFont,buttonVar::buttonIns.text.c_str(),{buttonVar::buttonIns.rect.x+15,buttonVar::buttonIns.rect.y+7}, 22, 2, WHITE);
    // Nút Delete
    DrawRectangleRounded(buttonVar::buttonDel.rect, 0.3f, 30, buttonVar::buttonDel.buCol);
    DrawTextEx(customFont,buttonVar::buttonDel.text.c_str(), {buttonVar::buttonDel.rect.x+15, buttonVar::buttonDel.rect.y+7}, 22, 2, WHITE);
    // Nút Find
    DrawRectangleRounded(buttonVar::buttonF.rect, 0.3f, 30, buttonVar::buttonF.buCol);
    DrawTextEx(customFont,buttonVar::buttonF.text.c_str(),{buttonVar::buttonF.rect.x+15,buttonVar::buttonF.rect.y+7}, 22, 2, WHITE);
    //Nút Clear
    DrawRectangleRounded(buttonVar::buttonClear.rect, 0.3f, 30, buttonVar::buttonClear.buCol);
    DrawTextEx(customFont,buttonVar::buttonClear.text.c_str(), {buttonVar::buttonClear.rect.x+17,buttonVar::buttonClear.rect.y+7}, 22, 2, WHITE);
}
// Xử lý hover 3 nút
void handleButtonsHover(){
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect))
        buttonVar::buttonIns.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect))
        buttonVar::buttonDel.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect))
        buttonVar::buttonF.buCol       = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect))
        buttonVar::buttonClear.buCol   = color::buttonColorHovered;
    else {
        buttonVar::buttonIns.buCol   = color::buttonColor;
        buttonVar::buttonDel.buCol   = color::buttonColor;
        buttonVar::buttonF.buCol     = color::buttonColor;
        buttonVar::buttonClear.buCol = color::buttonColor;
    }
}
// Xử lý click 3 nút
void handleButtonsClick(SSL* SSL){
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getInsert());
        SSL->setExistVal(SSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getDel());
        SSL->setExistVal(SSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getFind());
        SSL->setExistVal(SSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getClear());
        SSL->setExistVal(SSL->getRoot());
    }
}
// Vẽ blinking lines trong input
void drawBlinkingLines(const std::string& text, Rectangle rect, int& frameCounter){
    frameCounter++;
    if(frameCounter < 30) {
        // Tính x hiển thị con trỏ
        int width = MeasureTextEx(customFont, text.c_str(), rect.height-10, 1).x;
        float cursorX = rect.x + width + 7;
        DrawLine(cursorX, rect.y + 2, cursorX, rect.y + rect.height - 2, BLACK);
    }
    if(frameCounter >= 60) frameCounter = 0;
}
void drawTextIn(const std::string& text, Rectangle rect, int& frameCounter){
    float fontsize = button::sizeH - 10;
    float spacing = 1.0f;
    Vector2 pos = { rect.x + 5, rect.y + 5 };
    std::string str = text;
    Vector2 widthText = MeasureTextEx(customFont, str.c_str(), fontsize, spacing);
    // Cắt ký tự từ đầu nếu chuỗi quá dài
    while (widthText.x > rect.width-5 && !str.empty()) {
    str.erase(str.begin());  
    widthText = MeasureTextEx(customFont, str.c_str(), fontsize, spacing);
    }
    DrawTextEx(customFont, str.c_str(), pos, fontsize, spacing, BLACK);
    drawBlinkingLines(str, rect, frameCounter);
}

void drawArrow2Node(Vector2 start, Vector2 end, Color edgeRender) {
    Vector2 direct = Vector2Subtract(end, start);
    float length = Vector2Length(direct);
    if (length == 0) return;
    Vector2 endPos = {
        end.x - (33 * direct.x / length),
        end.y - (33 * direct.y / length)
    };
    drawArrow(start, endPos, edgeRender);
}