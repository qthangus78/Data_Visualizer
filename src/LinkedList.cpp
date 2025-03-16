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
// Screen
//--------------------------------
Screen::Screen()
 : root(nullptr), existVal(nullptr), mcurrent(nullptr), tail(nullptr)
{
    // Tạo các state
    mNotInMode = new notInMode(this);
    mInsert    = new Insert(this);
    mDelete    = new Delete(this);
    mFind      = new Find(this);
    mClear = new Clear(this);
    mcurrent   = mNotInMode;
    moupos = GetMousePosition();
}

Screen::~Screen(){
    // xóa state
    delete mNotInMode;
    delete mInsert;
    delete mDelete;
    delete mFind;
    delete mClear;
    // xóa list
    delMemAl(root);
}

void Screen::setState(IState* state) { mcurrent = state; }
IState* Screen::getnotInMode() { return mNotInMode; }
IState* Screen::getInsert()    { return mInsert; }
IState* Screen::getDel()       { return mDelete; }
IState* Screen::getFind()      { return mFind; }
IState* Screen::getClear()     {return mClear;}

ListNode* Screen::getRoot() { return root; }
ListNode* Screen::getTail() {return tail;}
ListNode* Screen::getprevTail() {return prevtail;}
void Screen::setprevTail(ListNode* APrev) {prevtail = APrev;}
void Screen::setRoot(ListNode* tmp) { root = tmp; }
void Screen::setExistVal(ListNode* tmp){ existVal = tmp; }
ListNode* Screen::getExistVal(){ return existVal; }
Vector2 Screen::getPosEnd() {return posEnd;}
void Screen::setPosEnd(Vector2 APos) {posEnd = APos;}

void Screen::insertList(int x){ insertAl(root,tail,x); }
bool Screen::delList(int x){ return delAl(root,tail,x); }
ListNode* Screen::findList(int x){ return findAl(root, x); }
void Screen::delAllList() {delMemAl(root); }
void Screen::fileInput(std::ifstream& fin) {fileInputAl(root,tail,fin);}

void Screen::draw(){
    if(mcurrent) mcurrent->draw();
}
void Screen::handle(){
    moupos = GetMousePosition();
    if(mcurrent) mcurrent->handle();
}
//--------------------------------
// Hàm tiện ích
//--------------------------------
//Vẽ Node
void drawNode(Vector2 pos, const std::string& text, float radius){
    DrawCircleV(pos, radius, color::nodeNotInMode); 
    Font font = GetFontDefault();
    Vector2 textSize = MeasureTextEx(font, text.c_str(), 23, 2);
    DrawTextEx(font, text.c_str(),
                   {pos.x - textSize.x / 2, pos.y - textSize.y / 2},
                   22, 2, BLACK);
}
//Vẽ mũi tên
void drawArrow(Vector2 start, Vector2 end, Color edgeRender){
    DrawLineEx(start, end, EArrow.thickness, edgeRender);
    float angle = atan2f(end.y - start.y, end.x - start.x);
    Vector2 left = {
        end.x - EArrow.arrowsize * cosf(angle - PI/6),
        end.y - EArrow.arrowsize * sinf(angle - PI/6)
    };
    Vector2 right = {
        end.x - EArrow.arrowsize * cosf(angle + PI/6),
        end.y - EArrow.arrowsize * sinf(angle + PI/6)
    };
    DrawTriangle(end, left, right, edgeRender);
}

void drawLinkedList(ListNode* root) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    const int rightMargin = 50;
    const int lineHeight = 100;
    int numLine = 0;
    
    while(root) {
        // Nếu vị trí dự kiến cho node kế tiếp vượt quá giới hạn màn hình,
        if(posStart.x + 2 * EArrow.length + 35 > GetScreenWidth() - rightMargin)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur  = { posStart.x + EArrow.length, posStart.y };
        // Nếu cur vượt quá giới hạn màn hình, chuyển sang dòng mới
        if(cur.x > GetScreenWidth() - rightMargin) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + lineHeight * numLine };
        }
        if(root->next) {
            drawArrow(prev, cur, color::edgeNotInMode);
        }
        // Vẽ node với dữ liệu của nó
        std::string dataStr = std::to_string(root->data);
        drawNode(prev, dataStr, 35);
        // Nếu đây là node cuối, vẽ mũi tên nối tới "NULL"
        if(!root->next) {
            if(cur.x < prev.x) {
                drawArrow(prev, cur, color::edgeNotInMode);
                drawNode(prev, dataStr, 35);
                DrawText("NULL", cur.x - 10, cur.y + 13, 25, BLUE);
            } else {
                drawArrow({ prev.x + 35, prev.y }, cur, color::edgeNotInMode);
                DrawText("NULL", cur.x + 5, cur.y - 13, 25, BLUE);
            }
        }
        // Cập nhật vị trí cho node kế tiếp
        if(cur.x > posStart.x)
            posStart = { cur.x + 35, cur.y };
        else
            posStart = cur; 
        root = root->next;
    }
    // Vẽ lại các node lưu trong shadedPos
    for(const auto& u : shadedPos) {
        std::string textShaded = std::to_string(u.node->data);
        drawNode(u.pos, textShaded, 35);
    }
}

void drawPartofLinkedList(ListNode* root, ListNode* EndPart) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    const int rightMargin = 50;
    const int lineHeight = 100;
    int numLine = 0;
    
    while(root) {
        if(root == EndPart)
            return;
        if(posStart.x + 2 * EArrow.length + 35 > GetScreenWidth() - rightMargin)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur  = { posStart.x + EArrow.length, posStart.y };
        if(cur.x > GetScreenWidth() - rightMargin) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + lineHeight * numLine };
        }
        // Vẽ mũi tên nối các node nếu có node kế và không phải EndPart
        if(root->next && root->next != EndPart) {
            drawArrow(prev, cur, color::edgeRendered);
        }
        // Vẽ node (với viền được vẽ bằng DrawCircleV)
        DrawCircleV(prev, 39, color::nodeRendered);
        std::string dataStr = std::to_string(root->data);
        drawNode(prev, dataStr, 34);
        if(cur.x > posStart.x)
            posStart = { cur.x + 35, cur.y };
        else
            posStart = cur;
        root = root->next;
    }
    // Vẽ lại các node trong shadedPos
    for(const auto& u : shadedPos) {
        DrawCircleV(u.pos, 39, color::nodeRendered);
        std::string text = std::to_string(u.node->data);
        drawNode(u.pos, text, 34);
    }
}

void drawPartofLinkedListNotColor(ListNode* root, ListNode* EndPart, Screen* s) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    const int rightMargin = 50;
    const int lineHeight = 100;
    int numLine = 0;
    while (root) {
        if (root == EndPart)
            return;
        // Nếu vị trí dự kiến cho node kế tiếp vượt quá giới hạn màn hình,
        if (posStart.x + 2 * EArrow.length + 35 > GetScreenWidth() - rightMargin)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur = { posStart.x + EArrow.length, posStart.y };
        if (cur.x > GetScreenWidth() - 50) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + lineHeight * numLine };
        }
        // Vẽ mũi tên nếu có node kế tiếp
        if (root->next && root->next!=EndPart) {
            drawArrow(prev, cur, color::edgeNotInMode);
        }
        // Vẽ node với dữ liệu của nó
        std::string str = std::to_string(root->data);
        drawNode(prev, str, 35);
        if (root->next == EndPart) {
            s->setPosEnd(posStart);
            s->setprevTail(root);
        }
        if (cur.x > posStart.x)
            posStart = { cur.x + 35, cur.y };
        else 
            posStart = cur;
        // Chuyển sang node kế tiếp
        root = root->next;
    }
    // Sau khi duyệt xong danh sách, vẽ lại các node lưu trong shadedPos
    for (const auto &u : shadedPos) {
        std::string textShaded = std::to_string(u.node->data);
        drawNode(u.pos, textShaded, 35);
    }
}
// Vẽ 3 nút Insert, Delete, Find
void drawButtons(){
    // Nút Insert
    DrawRectangleRounded(buttonVar::buttonIns.rect, 0.3f, 30, buttonVar::buttonIns.buCol);
    DrawText(buttonVar::buttonIns.text.c_str(),
             buttonVar::buttonIns.rect.x+15,
             buttonVar::buttonIns.rect.y+7, 22, WHITE);
    // Nút Delete
    DrawRectangleRounded(buttonVar::buttonDel.rect, 0.3f, 30, buttonVar::buttonDel.buCol);
    DrawText(buttonVar::buttonDel.text.c_str(),
             buttonVar::buttonDel.rect.x+15,
             buttonVar::buttonDel.rect.y+7, 22, WHITE);
    // Nút Find
    DrawRectangleRounded(buttonVar::buttonF.rect, 0.3f, 30, buttonVar::buttonF.buCol);
    DrawText(buttonVar::buttonF.text.c_str(),
             buttonVar::buttonF.rect.x+15,
             buttonVar::buttonF.rect.y+7, 22, WHITE);
    //Nút Clear
    DrawRectangleRounded(buttonVar::buttonClear.rect, 0.3f, 30, buttonVar::buttonClear.buCol);
    DrawText(buttonVar::buttonClear.text.c_str(),
            buttonVar::buttonClear.rect.x+17,
            buttonVar::buttonClear.rect.y+7, 22, WHITE);

}
// Xử lý hover 3 nút
void handleButtonsHover(){
    Vector2 mousePos = GetMousePosition();
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonIns.rect))
        buttonVar::buttonIns.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mousePos, buttonVar::buttonDel.rect))
        buttonVar::buttonDel.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mousePos, buttonVar::buttonF.rect))
        buttonVar::buttonF.buCol       = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mousePos, buttonVar::buttonClear.rect))
        buttonVar::buttonClear.buCol   = color::buttonColorHovered;
    else {
        buttonVar::buttonIns.buCol   = color::buttonColor;
        buttonVar::buttonDel.buCol   = color::buttonColor;
        buttonVar::buttonF.buCol     = color::buttonColor;
        buttonVar::buttonClear.buCol = color::buttonColor;
    }
}
// Xử lý click 3 nút
void handleButtonsClick(Screen* screen){
    Vector2 mousePos = screen->getMouse();
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        screen->setState(screen->getInsert());
        screen->setExistVal(screen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        screen->setState(screen->getDel());
        screen->setExistVal(screen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        screen->setState(screen->getFind());
        screen->setExistVal(screen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        screen->setState(screen->getClear());
        screen->setExistVal(screen->getRoot());
    }
}
// Vẽ blinking lines trong input
void drawBlinkingLines(const std::string& text, Rectangle rect, int& frameCounter){
    frameCounter++;
    if(frameCounter < 30) {
        // Tính x hiển thị con trỏ
        Font font = GetFontDefault();
        int width = MeasureTextEx(font, text.c_str(), rect.height-10, 1).x;
        float cursorX = rect.x + width + 7;
        DrawLine(cursorX, rect.y + 2, cursorX, rect.y + rect.height - 2, BLACK);
    }
    if(frameCounter >= 60) frameCounter = 0;
}
void drawTextIn(const std::string& text, Rectangle rect, int& frameCounter){
    Font font = GetFontDefault();
    float fontsize = button::sizeH - 10;
    float spacing = 1.0f;
    Vector2 pos = { rect.x + 5, rect.y + 5 };
    std::string str = text;
    Vector2 widthText = MeasureTextEx(font, str.c_str(), fontsize, spacing);
    // Cắt ký tự từ đầu nếu chuỗi quá dài
    while (widthText.x > rect.width-5 && !str.empty()) {
    str.erase(str.begin());  
    widthText = MeasureTextEx(font, str.c_str(), fontsize, spacing);
    }
    DrawTextEx(font, str.c_str(), pos, fontsize, spacing, BLACK);
    drawBlinkingLines(str, rect, frameCounter);
}