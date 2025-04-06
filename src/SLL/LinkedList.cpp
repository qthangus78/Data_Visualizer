#include "LinkedList.h"
#include "GlobalVar.h"
#include <raylib.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//Code Box
std::vector<button> code;
void initCodeButton(){
    code.resize(7);
    code[0] = {{850,400,500,36},{240,240,240,230},"",};
    for(int i=1;i<code.size(); i++){
        code[i] = {{code[i-1].rect.x,code[i-1].rect.y+code[i-1].rect.height,code[i-1].rect.width,code[i-1].rect.height},{240,240,240,230},""};
    }
}
//--------------------------------
// ListNode
//--------------------------------
ListNode::ListNode(int x) : data(x), next(nullptr) {}
//Add cuối
void insertTailAl(ListNode*& root, ListNode*& tail, int x) {
    if (!root) {
        root = new ListNode(x);
        tail = root;
        return;
    }
    tail->next = new ListNode(x); 
    tail = tail->next; 
}
//Add đầu
void insertHeadAl(ListNode*& root, ListNode*& tail, int x){
    if (!root) {
        root = new ListNode(x);
        tail = root;
        return;
    }
    ListNode* cur = new ListNode(x);
    cur->next = root;
    root = cur;
}
//Add Indx
void insertIdxAl(ListNode*& root, ListNode*& tail, int idx, int val) {
    if (!root) {  
        return;
    }
    if (idx == 0) {  
        ListNode* newNode = new ListNode(val);
        newNode->next = root;
        root = newNode;
        return;
    }
    ListNode* prev = nullptr;
    ListNode* cur = root;
    int num = 0;
    while (cur && num < idx) {  
        prev = cur;
        cur = cur->next;
        num++;
    }
    if (!prev) {  
        return;
    }
    ListNode* newNode = new ListNode(val);
    if (!cur) { 
        prev->next = newNode;
        tail = newNode;
    } else {  
        prev->next = newNode;
        newNode->next = cur;
    }
}
//Delete Val
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
//Delete Head
void delHeadAl(ListNode*& root, ListNode*& tail){
    if (!root) return;
    ListNode* temp = root;
    root = root->next;
    delete temp;
    if (!root) tail = nullptr;
}
//Delete Tail
void delTailAl(ListNode*& root, ListNode*& tail){
    if (!root) return;
    if (root == tail) {
        delete root;
        root = nullptr;
        tail = nullptr;
        return;
    }
    ListNode* prev = nullptr;
    ListNode* curr = root;
    while (curr != tail) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = nullptr;
    delete tail;
    tail = prev;
}
//Del Indx
void delIdxAl(ListNode*& root, ListNode*& tail, int idx){
    int num = 0;
    ListNode* prev = nullptr;
    ListNode* cur = root;
    while(num < idx){
        prev = cur;
        cur = cur->next;
        num++;
    }
    prev->next = cur->next;
    delete cur;
}
//Find Val
ListNode* findAl(ListNode*& root, int x) {
    ListNode* cur = root;
    while(cur) {
        if(cur->data == x) return cur;
        cur = cur->next;
    }
    return nullptr;
}
//Delete ALl
void delMemAl(ListNode*& root) {
    while(root) {
        ListNode* tmp = root;
        root = root->next;
        delete tmp;
    }
}
//Handle File
void fileInputAl(ListNode*& root, ListNode*& tail, std::ifstream& fin){
    int numIns;
    while(fin >> numIns){
        insertTailAl(root,tail,numIns);
    }
}
//--------------------------------
// SSL
//--------------------------------
SSL::SSL()
 : root(nullptr), existVal(nullptr), mcurrent(nullptr), tail(nullptr), num(0)
{
    mNotInMode = new notInMode(this);
    mCreate    = new Create(this);
    mInsert    = new Insert(this);
    mDelete    = new Delete(this);
    mFind      = new Find(this);
    mClear = new Clear(this);
    mcurrent   = mNotInMode;
}

SSL::~SSL(){
    delete mNotInMode;
    delete mCreate;
    delete mInsert;
    delete mDelete;
    delete mFind;
    delete mClear;
    delMemAl(root);
}

void SSL::setState(IState* state) { mcurrent = state; }
IState* SSL::getnotInMode() { return mNotInMode; }
IState* SSL::getCreate()    {return mCreate;}
IState* SSL::getInsert()    { return mInsert; }
IState* SSL::getDel()       { return mDelete; }
IState* SSL::getFind()      { return mFind; }
IState* SSL::getClear()     {return mClear;}
IState::ToggleSwitch SSL::getToggle() {return toggle;}

ListNode* SSL::getRoot() { return root; }
void SSL::setRoot(ListNode* cur) {root = cur;}
ListNode* SSL::getTail() {return tail;}
ListNode* SSL::getprevTail() {return prevtail;}
void SSL::setprevTail(ListNode* APrev) {prevtail = APrev;}
void SSL::setExistVal(ListNode* tmp){ existVal = tmp;}
ListNode* SSL::getExistVal(){ return existVal;}
Vector2 SSL::getPosEnd() {return posEnd;}
void SSL::setPosEnd(Vector2 APos) {posEnd = APos;}
int SSL::getNumElement(){return num;}
void SSL::setNumElement(int nums) {num = nums;}
std::stack <SSL::command>& SSL::getCommandUndo() {return undo;}
std::stack <SSL::command>& SSL::getCommnadRedo() {return redo;}
void SSL::pushStack(std::stack<command>& st, command prev) {st.push(prev);}
void SSL::popStack(std::stack<command>& st) {st.pop();}
void SSL::clearStackUndo() {
    while(!undo.empty()) undo.pop();
}
void SSL::clearStackRedo(){
    while(!redo.empty()) redo.pop();
}
bool SSL::getPause() {return IsPaused;}

void SSL::insertHeadList(int x){ insertHeadAl(root,tail,x); }
void SSL::insertTailList(int x){ insertTailAl(root,tail,x); }
void SSL::insertIdxList(int idx,int x) {insertIdxAl(root,tail,idx,x);}
bool SSL::delList(int x){ return delAl(root,tail,x); }
void SSL::delHeadList() {delHeadAl(root,tail);}
void SSL::delTailList() {delTailAl(root,tail);}
void SSL::delIdxList(int idx) {delIdxAl(root,tail,idx);}
ListNode* SSL::findList(int x){ return findAl(root, x); }
void SSL::delAllList() {delMemAl(root); }
void SSL::fileInput(std::ifstream& fin) {fileInputAl(root,tail,fin);}
void SSL::handleUndo(){
    if(undo.empty()) return;
    command top = undo.top();
    undo.pop();
    redo.push(top);
    switch (top.modeInsertType)
    {
    case IState::InsertType::Head:
        delHeadList();
        num--;
        break;
    case IState::InsertType::Tail:
        delTailList();
        num--;
        break;
    case IState::InsertType::Idx:
        switch (top.curInsertType)
        {
        case IState::InsertType::Head:
            delHeadList();
            num--;
            break;
        case IState::InsertType::Tail:
            delTailList();
            num--;
            break;
        case IState::InsertType::Idx:
            delIdxList(top.idx);
            num--;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    switch (top.modeDeleteType)
    {
    case IState::DeleteType::Head:
        insertHeadList(top.val);
        num++;
        break;
    case IState::DeleteType::Tail:
        insertTailList(top.val);
        num++;
        break;
    case IState::DeleteType::Val:
        switch(top.curDeleteType)
        {
        case IState::DeleteType::Head:
            insertHeadList(top.val);
            num++;
            break;
        case IState::DeleteType::Tail:
            insertTailList(top.val);
            num++;
            break;
        case IState::DeleteType::Val:
            insertIdxList(top.idx,top.val);
            num++;
            break;
        default:
            break;
        }
    default:
        break;
    }
}
void SSL::handleRedo(){
    if(redo.empty()) return;
    command top = redo.top();
    redo.pop();
    undo.push(top);
    switch (top.modeInsertType)
    {
    case IState::InsertType::Head:
        insertHeadList(top.val);
        num++;
        break;   
    case IState::InsertType::Tail:
        insertTailList(top.val);
        num++;
        break;
    case IState::InsertType::Idx:
        switch (top.curInsertType)
        {
        case IState::InsertType::Head:
            insertHeadList(top.val);
            num++;
            break;
        case IState::InsertType::Tail:
            insertTailList(top.val);
            num++;
            break;
        case IState::InsertType::Idx:
            insertIdxList(top.idx,top.val);
            num++;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    switch (top.modeDeleteType)
    {
    case IState::DeleteType::Head:
        delHeadList();
        num--;
        break;
    case IState::DeleteType::Tail:
        delTailList();
        num--;
        break;
    case IState::DeleteType::Val:
        switch(top.curDeleteType)
        {
        case IState::DeleteType::Head:
            delHeadList();
            num--;
            break;
        case IState::DeleteType::Tail:
            delTailList();
            num--;
            break;
        case IState::DeleteType::Val:
            delIdxList(top.idx);
            num--;
            break;
        default:
            break;
        }
    default:
        break;
    }
}

void SSL::draw(){
    UndoButton.Drawtexture();
    RedoButton.Drawtexture();
    if(!IsPaused) PauseButton.Drawtexture();
    else PlayButton.Drawtexture();
    toggle.Draw();
    if(mcurrent) mcurrent->draw();
}
void SSL::handle(){
    initCodeButton();
    UndoButton.SetPosition(460,650);
    RedoButton.SetPosition(670,650);
    PlayButton.SetPosition(560,635);
    PauseButton.SetPosition(560,635);
    toggle.Update(GetMousePosition());
    if(PlayButton.isPressed()) {IsPaused = !IsPaused;}
    if(mcurrent) mcurrent->handle();
}
//--------------------------------
// Hàm tiện ích
//--------------------------------
//Vẽ Node
void drawNode(Vector2 pos, const std::string& text, float radius, Color col){
    DrawCircleV(pos, radius, col);
    DrawCircleV(pos, radius-4, WHITE);
    Vector2 textSize = MeasureTextEx(SSLFont, text.c_str(), FontNode+1, 2);
    DrawTextEx(SSLFont, text.c_str(),{pos.x - textSize.x / 2, pos.y - textSize.y / 2}, FontNode, 2, BLACK);
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
void drawLinkedList(ListNode* root, Vector2 startpos, SSL *s) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startpos;
    int numLine = 0;
    if(s->getRoot()){
    drawTextUp("head",20,startLinkedListPos);
    }
    while(root) {
        if(posStart.x + 3* EArrow.length + 3*NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur  = { posStart.x + EArrow.length, posStart.y };
        if(cur.x + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + ROW_OFFSET * numLine };
        }
        if(root->next && cur.x > prev.x) {
            drawArrow(prev, cur, color::edgeNotInMode);
        }
        if(root->next && cur.x < prev.x){
            drawArrow2Node(prev,cur,color::edgeNotInMode);
        }
        std::string dataStr = std::to_string(root->data);
        drawNode(prev, dataStr, NODE_SIZE,color::nodeNotInMode);
        if(!root->next) {
            if(cur.x < prev.x) {
                drawArrow(prev, cur, color::edgeNotInMode);
                drawNode(prev, dataStr, NODE_SIZE,color::nodeNotInMode);
                DrawTextEx(SSLFont,"NULL", {cur.x - 10, cur.y + 13},25,2,BLUE);
            } else {
                drawArrow({ prev.x + NODE_SIZE, prev.y }, cur, color::edgeNotInMode);
                DrawTextEx(SSLFont,"NULL", {cur.x + 5, cur.y - 13}, 25, 2, BLUE);
            }
        }
        if(cur.x > posStart.x)
            posStart = { cur.x + NODE_SIZE, cur.y };
        else
            posStart = cur; 
        root = root->next;
    }
    for(const auto& u : shadedPos) {
        std::string textShaded = std::to_string(u.node->data);
        drawNode(u.pos, textShaded, NODE_SIZE,color::nodeNotInMode);
    }
}
void drawPartofLinkedList(ListNode* root, ListNode* EndPart, SSL* s) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    if(s->getRoot()){
        drawTextUp("head",20,startLinkedListPos);
    }
    int numLine = 0;
    while(root) {
        if(root == EndPart)
            return;
        if(root->next == EndPart){
            drawTextDown("pointer",20,posStart);
        }
        if(posStart.x + 3*EArrow.length + 3*NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur  = { posStart.x + EArrow.length, posStart.y };
        if(cur.x + NODE_SIZE> GetScreenWidth() - SCREEN_MARGIN) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + ROW_OFFSET * numLine };
        }
        if(root->next && cur.x > prev.x && root->next!=EndPart) {
            drawArrow(prev, cur, color::edgeRendered);
        }
        if(root->next && cur.x < prev.x && root->next!=EndPart){
            drawArrow2Node(prev,cur,color::edgeRendered);
        }
        std::string dataStr = std::to_string(root->data);
        drawNode(prev, dataStr, NODE_SIZE, color::nodeRendered);
        if(cur.x > posStart.x)
            posStart = { cur.x + NODE_SIZE, cur.y };
        else
            posStart = cur;
        root = root->next;
    }
    for(const auto& u : shadedPos) {
        std::string text = std::to_string(u.node->data);
        drawNode(u.pos, text, NODE_SIZE, color::nodeRendered);
    }
}
void drawPartofLinkedListNotColor(ListNode* root, ListNode* EndPart, SSL* s) {
    std::vector<ShadedData> shadedPos;
    Vector2 posStart = startLinkedListPos;
    int numLine = 0;
    while (root) {
        if (root == EndPart)
            return;
        if (posStart.x + 3*EArrow.length + 2*NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
            shadedPos.push_back({ posStart, root });
        Vector2 prev = posStart;
        Vector2 cur = { posStart.x + EArrow.length, posStart.y };
        if (cur.x + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN) {
            numLine++;
            cur = { startLinkedListPos.x, startLinkedListPos.y + ROW_OFFSET * numLine };
        }
        if(root->next && cur.x > prev.x && root->next!=EndPart) {
            drawArrow(prev, cur, color::edgeNotInMode);
        }
        if(root->next && cur.x < prev.x && root->next!=EndPart){
            drawArrow2Node(prev,cur,color::edgeNotInMode);
        }
        std::string str = std::to_string(root->data);
        drawNode(prev, str, NODE_SIZE, color::nodeNotInMode);
        if (root->next == EndPart) {
            s->setPosEnd(posStart);
            s->setprevTail(root);
        }
        if (cur.x > posStart.x)
            posStart = { cur.x + NODE_SIZE, cur.y };
        else 
            posStart = cur;
        root = root->next;
    }
    for (const auto &u : shadedPos) {
        std::string textShaded = std::to_string(u.node->data);
        drawNode(u.pos, textShaded, NODE_SIZE,color::nodeNotInMode);
    }
}
// Vẽ 3 nút Create, Insert, Delete, Find
void DrawButton(Rectangle rect, const std::string &text, Color &color, Font font, float fontsize) {
    Color drawColor = color;
    DrawRectangleRounded(rect, 0.3f, 12, drawColor);
    DrawRectangleRoundedLinesEx(rect, 0.3f, 12, 2.0f, BLACK);
    int fontSize = fontsize;
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 2);
    Vector2 textPos = { rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2 };
    DrawTextEx(font, text.c_str(), textPos, fontSize, 2, BLACK);
}
void drawButtons() {
    DrawButton(buttonVar::buttonCreate.rect, buttonVar::buttonCreate.text, buttonVar::buttonCreate.buCol, SSLFont,22);
    DrawButton(buttonVar::buttonIns.rect, buttonVar::buttonIns.text, buttonVar::buttonIns.buCol, SSLFont,22);
    DrawButton(buttonVar::buttonDel.rect, buttonVar::buttonDel.text, buttonVar::buttonDel.buCol, SSLFont,22);
    DrawButton(buttonVar::buttonF.rect, buttonVar::buttonF.text, buttonVar::buttonF.buCol, SSLFont,22);
    DrawButton(buttonVar::buttonClear.rect, buttonVar::buttonClear.text, buttonVar::buttonClear.buCol, SSLFont,22);
}
// Xử lý hover 4 nút
void handleButtonsHover(){
    if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect))
        buttonVar::buttonCreate.buCol   = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect))
        buttonVar::buttonIns.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect))
        buttonVar::buttonDel.buCol     = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect))
        buttonVar::buttonF.buCol       = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect))
        buttonVar::buttonClear.buCol   = color::buttonColorHovered;
    else if(CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect)){
        buttonVar::buttonGo.buCol = color::buttonFileHovered;
    }
    else {
        buttonVar::buttonCreate.buCol = color::buttonColor;
        buttonVar::buttonIns.buCol    = color::buttonColor;
        buttonVar::buttonDel.buCol    = color::buttonColor;
        buttonVar::buttonF.buCol      = color::buttonColor;
        buttonVar::buttonClear.buCol  = color::buttonColor;
        buttonVar::buttonGo.buCol = color::buttonFile;
    }
}
// Xử lý click 3 nút
void handleButtonsClick(SSL* SSL){
    if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getCreate());
        SSL->setExistVal(SSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getInsert());
        SSL->setExistVal(SSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getDel());
        SSL->setExistVal(SSL->getRoot());
        buttonVar::buttonGo    = {{buttonVar::buttonDel.rect.x+250, buttonVar::buttonDel.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getFind());
        SSL->setExistVal(SSL->getRoot());
        buttonVar::buttonGo    = {{buttonVar::buttonF.rect.x+250, buttonVar::buttonF.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        SSL->setState(SSL->getClear());
        SSL->setExistVal(SSL->getRoot());
        SSL->clearStackUndo();
        SSL->clearStackRedo();
    }
}
// Vẽ blinking lines trong input
void drawBlinkingLines(const std::string& text, Rectangle rect, int& frameCounter){
    frameCounter++;
    if(frameCounter < 30) {
        int width = MeasureTextEx(SSLFont, text.c_str(), rect.height-10, 1).x;
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
    Vector2 widthText = MeasureTextEx(SSLFont, str.c_str(), fontsize, spacing);
    while (widthText.x > rect.width-5 && !str.empty()) {
    str.erase(str.begin());  
    widthText = MeasureTextEx(SSLFont, str.c_str(), fontsize, spacing);
    }
    DrawTextEx(SSLFont, str.c_str(), pos, fontsize, spacing, BLACK);
    drawBlinkingLines(str, rect, frameCounter);
}
void drawTextInwithoutLine(const std::string& text, Rectangle rect){
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
}
void drawArrow2Node(Vector2 start, Vector2 end, Color edgeRender) {
    Vector2 direct = Vector2Subtract(end, start);
    float length = Vector2Length(direct);
    if (length == 0) return;
    Vector2 endPos = {
        end.x - (NODE_SIZE * direct.x / length),
        end.y - (NODE_SIZE * direct.y / length)
    };
    drawArrow(start, endPos, edgeRender);
}
float smoothstep(float t){
    if(t >= 1) return 1;
    if(t<=0) return 0;
    return t*t*(3-2*t);
}
Vector2 lerp(Vector2 start, Vector2 end, float progress){
    float t = smoothstep(progress);
    return {(start.x+t*(end.x-start.x)),(start.y+t*(end.y-start.y))};
}
float lerp(float start, float end, float progress){
    float t = smoothstep(progress);
    return start+t*(end-start);
}
void handlePos(ListNode* s, Vector2 startPos, std::vector<ShadedData>& pos){
    const float ARROW_LENGTH = EArrow.length;
    ListNode* cur = s;
    pos.push_back({ startPos, cur });
    cur = cur->next;
    while (cur) {
        const Vector2& lastPos = pos.back().pos;
        if (lastPos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
            pos.push_back({ {startLinkedListPos.x, lastPos.y + ROW_OFFSET}, cur });
        else
            pos.push_back({ {lastPos.x + ARROW_LENGTH + NODE_SIZE, lastPos.y}, cur });
        cur = cur->next;
    }
}
void drawPos(std::vector<ShadedData> pos, float NodeRadiusRender, float FontSize){
    for (int i = 0; i < pos.size(); i++) {
        DrawCircleV(pos[i].pos, NodeRadiusRender, color::nodeNotInMode);
        if (NodeRadiusRender >= 4) 
            DrawCircleV(pos[i].pos, NodeRadiusRender - 4, WHITE);
        std::string nodeData = std::to_string(pos[i].node->data);
        Vector2 textSize = MeasureTextEx(SSLFont, nodeData.c_str(), FontSize, 2);
        DrawTextEx(SSLFont, nodeData.c_str(),{ pos[i].pos.x - textSize.x / 2, pos[i].pos.y - textSize.y / 2 },FontSize, 2, BLACK);
    }
}
void amplifyNode(float& NodeRadiusRender, float& FontSize, Vector2 pos, int nums, float& progressNode, SSL* s){
    if(!s->getPause())progressNode += GetFrameTime();
    NodeRadiusRender = lerp(0, 33, progressNode);
    FontSize = lerp(0, FontNode, progressNode); 
    float fontText = lerp(0,22,progressNode);
    DrawCircleV(pos, NodeRadiusRender, color::nodeRendered);
    if(NodeRadiusRender>=4) DrawCircleV(pos, NodeRadiusRender-4, WHITE);
    std::string nodeData = std::to_string(nums);
    Vector2 textSize = MeasureTextEx(SSLFont, nodeData.c_str(), FontSize, 2);
    DrawTextEx(SSLFont, nodeData.c_str(),{ pos.x - textSize.x / 2, pos.y - textSize.y / 2 },FontSize, 2, BLACK);
    drawTextUp("vtx",fontText,pos);
}
void removeNode(float& NodeRadiusRender, float& FontSize, std::string str, Vector2 pos, float& progressNode, SSL* s){
    if(!s->getPause())progressNode+=GetFrameTime();
    NodeRadiusRender = lerp(NODE_SIZE-4,0,progressNode);
    FontSize = lerp(FontNode,0,progressNode);
    float fontText = lerp(22,0,progressNode);
    drawTextDown("pointer",fontText,pos);
    DrawCircleV(pos, NodeRadiusRender + 4, color::nodeRendered);
    DrawCircleV(pos, NodeRadiusRender, WHITE);
    Vector2 textSize = MeasureTextEx(SSLFont, str.c_str(), FontSize, 2);
    DrawTextEx(SSLFont, str.c_str(), { pos.x - textSize.x / 2, pos.y - textSize.y / 2 }, FontSize, 2, BLACK);
}
void movesPos(std::vector<ShadedData>& movePos,float progressNode){
    for(int i=0; i<movePos.size(); i++){
        if(movePos[i].pos.x + EArrow.length + NODE_SIZE > GetScreenWidth()-SCREEN_MARGIN){
            movePos[i].pos = lerp(movePos[i].pos,{startLinkedListPos.x,movePos[i].pos.y+ROW_OFFSET},progressNode);
        }
        else{
            movePos[i].pos = lerp(movePos[i].pos,{movePos[i].pos.x+EArrow.length+NODE_SIZE,movePos[i].pos.y},progressNode);
        }
    }
    for(int i=0; i<movePos.size()-1; i++){
        drawArrow2Node(movePos[i].pos,movePos[i+1].pos,color::edgeNotInMode);
    }
    for(auto u : movePos){
        drawNode(u.pos, std::to_string(u.node->data),NODE_SIZE,color::nodeNotInMode);
    }
}
void drawTextUp(std::string h, float fontText, Vector2 pos){
    Vector2 hwidth = MeasureTextEx(SSLFont,h.c_str(),fontText,2);
    Vector2 hDraw = {pos.x-hwidth.x/2,pos.y-NODE_SIZE-hwidth.y};
    DrawTextEx(SSLFont,h.c_str(),hDraw,fontText,2,RED);
}
void drawTextDown(std::string pointer, float fontText, Vector2 pos){
    Vector2 tmpwidth = MeasureTextEx(SSLFont,pointer.c_str(),fontText,2);
    Vector2 tmpDraw = {pos.x-tmpwidth.x/2,pos.y+NODE_SIZE};
    DrawTextEx(SSLFont,pointer.c_str(),tmpDraw,fontText,2,RED);
}
void nodeNext(ListNode*& cur, Vector2& pos, int& framecntDel){
    const int ARROW_LENGTH = EArrow.length;
    cur = cur->next;
    framecntDel = 0;
    if (pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
        pos = { startLinkedListPos.x, pos.y + ROW_OFFSET };
    else
        pos.x += ARROW_LENGTH + NODE_SIZE;
}
void drawTextCode(int curline, int curlinetmp) {
    DrawRectangleRec(CodeBox, {240,240,240,230});
    for (int i = 0; i < code.size(); i++) {
        const auto& btn = code[i];
        Color fillColor = (i == curline || i == curlinetmp) ? GRAY : btn.buCol;
        DrawRectangleRec(btn.rect, fillColor);
        DrawRectangleLinesEx(btn.rect, 0.5f, BLACK);
        Vector2 textPos = {btn.rect.x + 5.0f, btn.rect.y + (btn.rect.height - 21) / 2};
        DrawTextEx(textCodeFont, btn.text.c_str(), textPos, 21, 1, BLACK);
    }
}