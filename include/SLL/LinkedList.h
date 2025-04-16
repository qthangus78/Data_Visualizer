#pragma once
#include <string>
#include <fstream>
#include <raylib.h>
#include <vector>
#include <stack>
#include "GlobalVar.h"

const int NODE_SIZE = 33;
const int ROW_OFFSET = 100;
const int SCREEN_MARGIN = 50;
const int FontNode = 22;
const float speedNode = 70;
extern std::vector<button> code;
const Rectangle CodeBox = {960,430,400,252};
extern const float deltaTime;
void initCodeButton();
struct ListNode {
    int data;
    ListNode* next;
    ListNode(int x);
};

struct ShadedData {
    Vector2 pos;
    ListNode* node;
};
//--------------------------------
// Các hàm thao tác LinkedList
//--------------------------------
void insertTailAl(ListNode*& root, ListNode*& tail, int x);
void insertHeadAl(ListNode*& root, ListNode*& tail, int x);
void insertIdxAl(ListNode*& root, ListNode*& tail, int idx, int val);
bool delAl(ListNode*& root, ListNode*& tail,int x);
void delIdxAl(ListNode*& root, ListNode*& tail, int idx);
void delHeadAl(ListNode*& root, ListNode*& tail);
void delTailAl(ListNode*& root, ListNode*& tail);
ListNode* findAl(ListNode*& root, int x);
void fileInputAl(ListNode*& root, ListNode*&tail, std::ifstream& fin);
void delMemAl(ListNode*& root);
//--------------------------------
// Forward Declarations
//--------------------------------
class IState;
class SSL;

//--------------------------------
// Interface State
//--------------------------------
class IState {
public:
    enum class InsertType {Head,Tail,Idx,None};
    enum class DeleteType{None,Head,Tail,Val};
    enum class CreateType{None,File,Random};
    struct ToggleSwitch {
        Rectangle bounds;
        bool isStepByStep;
        float toggleX;
        float toggleRadius;
        float labelAlpha;
        float labelAlphaReverse;
        ToggleSwitch() {
            bounds = {40,20,140,40};
            isStepByStep = true;
            toggleRadius = 40 * 0.8f;
            toggleX = bounds.x + 5;
            labelAlpha = 1.0f;
            labelAlphaReverse = 0.0f;
        }
        void Update(Vector2 mouse) {
            if (CheckCollisionPointRec(mouse, bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                isStepByStep = !isStepByStep;
            }
            float targetX = isStepByStep ? bounds.x + 5 : bounds.x + bounds.width - toggleRadius - 5;
            toggleX += (targetX - toggleX) * 0.15f;
            float targetAlpha = isStepByStep ? 1.0f : 0.0f;
            float targetAlphaReverse = isStepByStep ? 0.0f : 1.0f;
            labelAlpha += (targetAlpha - labelAlpha) * 0.1f;
            labelAlphaReverse += (targetAlphaReverse - labelAlphaReverse) * 0.1f;
        }
    
        void Draw() {
            DrawRectangleRoundedLinesEx(bounds, 0.5f, 20, 4, DARKGRAY);
            DrawCircleV({ toggleX + toggleRadius / 2, bounds.y + bounds.height / 2 }, toggleRadius / 2 + 3.0f, DARKBLUE);
            DrawCircleV({ toggleX + toggleRadius / 2, bounds.y + bounds.height / 2 }, toggleRadius / 2, WHITE);
            DrawTextEx(SSLFont, "Step by step", {bounds.x + 40, bounds.y + 10}, 17, 1, Fade(BLACK, labelAlpha));
            DrawTextEx(SSLFont, "Run at once", {bounds.x + bounds.width - 135, bounds.y + 15}, 17, 1, Fade(BLACK, labelAlphaReverse));
        }
    };
    virtual ~IState() {}
    virtual void draw() = 0;
    virtual void handle() = 0;
};

//--------------------------------
// SSL
//--------------------------------
class SSL {
private:
    ListNode* root;   
    ListNode* tail;
    ListNode* prevtail;
    ListNode* existVal; 
    Vector2 posEnd;
    IState* mcurrent;
    IState* mNotInMode;
    IState* mCreate;
    IState* mInsert;
    IState* mDelete;
    IState* mFind;
    IState* mClear;
    int num;
    IState::ToggleSwitch toggle; 
    SpeedButtonSpinner speed;
    bool IsPaused = false;
    float fraction = 1.0f;
public:
    SSL();
    ~SSL();
    struct command{
        IState::InsertType modeInsertType;
        IState::InsertType curInsertType;
        IState::DeleteType modeDeleteType;
        IState::DeleteType curDeleteType;
        int idx, val;
    };
    // getter/setter
    ListNode* getRoot();
    void setRoot(ListNode* cur);
    ListNode* getTail();
    ListNode* getprevTail();
    void setprevTail(ListNode* APrev);
    void setExistVal(ListNode* tmp);
    ListNode* getExistVal();
    Vector2 getPosEnd();
    void setPosEnd(Vector2 APos);
    int getNumElement();
    void setNumElement(int nums);
    std::stack <command>& getCommandUndo();
    std::stack <command>& getCommnadRedo();
    command getTop();
    void pushStack(std::stack<command>& st,command cur);
    void popStack(std::stack<command>& st);
    bool getPause ();
    void setPause(bool Pause);
    float getFraction();
    void setFraction(float fraction);
    void clearStackUndo();
    void clearStackRedo();
    // hàm thao tác LinkedList
    void insertHeadList(int x);
    void insertTailList(int x);
    void insertIdxList(int idx, int val);
    bool delList(int x);
    void delHeadList();
    void delTailList();
    void delIdxList(int idx);
    ListNode* findList(int x);
    void delAllList();
    void fileInput(std::ifstream& fin);
    // chuyển state
    void setState(IState* state);
    IState* getnotInMode();
    IState* getCreate();
    IState* getInsert();
    IState* getDel();
    IState* getFind();
    IState* getClear(); 
    IState::ToggleSwitch getToggle();
    void setToggle(IState::ToggleSwitch toggle);
    void handleUndo();
    void handleRedo();
    // main loop
    void draw();
    void handle();
private:
    std::stack <command> undo;
    std::stack <command> redo;
};
//--------------------------------
// Các lớp state
//--------------------------------
class notInMode : public IState {
private:
    SSL* mSSL;
public:
    notInMode(SSL* s) : mSSL(s) {}
    void draw() override;
    void handle() override;
};
//-----------------------------
class Create : public IState{
private: 
    SSL* mSSL;
    button fileInput, random;
    std::vector<ShadedData> pos;
    bool randomProcess, fileProcess;
    float progress, progressGo;
    CreateType prev = CreateType::None, cur = CreateType::None;
    bool inProcess = false;
    public:
    Create(SSL *s);
    CreateType DetectCurrentMode();
    void handleModeTransitTion(CreateType newType);
    void updateCommonAnimation();
    void handleFileMode();
    void handleRandomMode();
    void drawInputFile();
    void drawInitialize();
    void handleInit();
    void handleInputFile();
    void randomAnimation();
    void fileAnimation();
    void draw() override;
    void handle() override;
};
//--------------------------------
class Insert : public IState {
private:
    SSL* mSSL;
    InsertType prev = InsertType::None,cur = InsertType::None, modeCur = InsertType::None;
    int frameCounter, framecntInsert;
    std::string textIn, textInIndex;
    button InsertHead, InsertTail, InsertIndex, Index, Value;
    bool InsertTailProcess, InsertHeadProcess, InsertIdxProcess, inProcess;
    ListNode* currentAnimatingNode;
    Vector2 posTail, pos, prevpos;
    float progressNode, progressAppear, progressArrow;
    float Font;
    Rectangle inputRect,idxRect,valRect;
    bool valInsert, idxInsert;
    int nums, idx = 0, curindex=0, curline=-1, curlinetmp=-1;
    bool overValue = false, inIndex = true;
    stack<pair<ListNode*,Vector2>> st;
    std::vector<std::string> codeHead = {
        "Vertex vtx = new Vertex(v)",
        "vtx.next = head",
        "head = vtx"
    };
    std::vector<std::string> codeTail = {
        "if(!head.next) head.next = vtx",
        "Vertex vtx = new Vertex(v)",
        "pointer = head", 
        "while (pointer.next != null)",
        "   pointer = pointer.next",
        "pointer.next = vtx"
    };
    std::vector<std::string> codeIdx = {
        "pointer = head",
        "for (k = 0; k < i-1; k++)",
        "   pointer = pointer.next",
        "aft = pointer.next",
        "Vertex vtx = new Vertex(v)",
        "vtx.next = aft",
        "pointer.next = vtx"
    };
public:
    Insert(SSL* s);
    void drawHeadInsert();
    void drawTailInsert();
    void drawIdxInsert();
    InsertType DetectCurrentMode();
    void handleModeTransitTion(InsertType newType);
    void updateCommonAnimation();
    void handleNotMode();
    void handleHeadMode();
    void handleTailMode();
    void handleIdxMode();
    void handleHeadCode();
    void handleTailCode();
    void handleIdxCode();
    void handleCodeLine();
    void insertHeadAnimation(int x);
    void insertTailAnimation(ListNode*& tmp,Vector2& posTail);
    void insertIdxAnimation(ListNode*& tmp);
    void handleUndo();
    void handleRedo();
    void ResetInsertState();
    void draw() override;
    void handle() override;
};
//--------------------------------
class Delete : public IState {
private:
    DeleteType prev = DeleteType::None, cur = DeleteType::None, modeCur = DeleteType::None;
    SSL* mSSL;
    int frameCounter, framecntDel;
    std::string textIn;
    std::string tmpText;
    bool existVal, DeleteValProcess, DeleteHeadProcess, DeleteTailProcess, inProcess;
    ListNode* currentAnimatingNode;
    ShadedData shadedPos;
    button DeleteHead, DeleteTail, DeleteVal;
    float progressNode, progressArrow, progressAppear,ArrowLengthRender;
    Rectangle inputRect;
    int curline = -1, curlinetmp = -1;
    Vector2 prevpos = {0,0};
    int idx = 0;
    stack<pair<ListNode*,Vector2>> st;
    std::vector<std::string> delHeadCode = {
        "if empty, do nothing",
        "if head.next = null",
        "delete head",
        "pointer = head",
        "head = head.next",
        "delete pointer"
    };
    std::vector<std::string>delTailCode ={
        "if empty, do nothing",
        "Vertex pointer = head",
        "while (pointer.next != null)",
        "  pointer = pointer.next",
        "delete pointer"
    };
    std::vector<std::string>delValCode = {
        "if empty, do nothing",
        "Vertex pointer = head, prev = null",
        "while (pointer.next != val)",
        "  prev = pointer",
        "  pointer = pointer.next",
        "prev.next = pointer.next",
        "delete pointer"
    };
public:
    Delete(SSL* s);
    DeleteType DetectCurrentMode();
    void handleModeTransitTion(DeleteType newType);
    void updateCommonAnimation();
    void handleNotMode();
    void handleHeadMode();
    void handleTailMode();
    void handleValMode();
    void handleHeadCode();
    void handleTailCode();
    void handleValCode();
    void delHeadAnimation();
    void delTailAnimation(ListNode*& cur);
    void delAnimation(ListNode*& tmp, int x);
    void handleUndo();
    void handleRedo();
    void ResetDeleteState();
    void draw() override;
    void handle() override;
};
//--------------------------------
class Find : public IState {
private:
    SSL* mSSL;
    int frameCounter, framecntFind;
    bool FindProcess;
    ShadedData shadedPos;
    ListNode* currentAnimatingNode = nullptr;
    std::string textIn;
    std::string tmpText;
    std::vector<std::string> codeLines = {"if empty, return NOT_FOUND",
    "pointer = head",
    "while (pointer.val != v)",
    "   pointer = pointer.next",
    "   if pointer.next == null",
    "      return NOT_FOUND",
    "return pointer"
    };
    int curline=-1;
    int curlinetmp = -1;
    stack<pair<ListNode*, Vector2>> st;
public:
    Find(SSL* s);
    void findAnimation(ListNode*& root);
    void handleUndo();
    void handleRedo();
    void handleCodeLine();
    void ResetFindState();
    void draw() override;
    void handle() override;
};
class Clear : public IState{
private:
    SSL* mSSL;
    std::vector<ShadedData> node;
    float progress;
    bool clearProcess;
public:
    Clear(SSL* s);
    void ClearAnimation();
    void draw() override;
    void handle() override;
};
// Hàm tiện ích vẽ chung
void drawNode(Vector2 pos, const std::string& text, float radius, Color col);
void drawLinkedList(ListNode* root, Vector2 startpos, SSL* s);
void drawPartofLinkedList(ListNode* root, ListNode* EndPart, SSL* s);
void drawPartofLinkedListNotColor(ListNode* root, ListNode* EndPart, SSL* s);
void DrawButton(Rectangle rect, const std::string &text, Color &color, Font font, float fontsize);
void drawButtons();
void handleButtonsClick(SSL* SSL);
void handleButtonsHover();
void drawBlinkingLines(const std::string& text, Rectangle rect, int& frameCounter);
void drawTextIn(const std::string& text, Rectangle rect, int& frameCounter);
void drawTextInwithoutLine(const std::string& text, Rectangle rect);
Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);
float Vector2Length(Vector2 v);
Vector2 Vector2Add(Vector2 v1, Vector2 v2);
Vector2 Vector2Scale(Vector2 v, float scale);
void drawArrow(Vector2 start, Vector2 end, Color edgeRender);
void drawArrow2Node(Vector2 start, Vector2 end, Color edgeRender);
float smoothstep(float t);
Vector2 lerp(Vector2 start, Vector2 end,float progress);
float lerp(float start, float end, float progress);
void handlePos(ListNode* s, Vector2 startPos,std::vector<ShadedData>& pos);
void drawPos(std::vector<ShadedData> pos, float NodeRadiusRender, float FontSize);
void movesPos(std::vector<ShadedData>& movePos,float progressNode);
void amplifyNode(float& NodeRadiusRender, float& FontSize, Vector2 pos, int nums, float& progressNode, SSL* s);
void removeNode(float& NodeRadiusRender, float& FontSize, std::string str, Vector2 pos, float& progressNode, SSL* s);
void drawTextUp(std::string s, float fontText, Vector2 pos);
void drawTextDown(std::string s, float fontText, Vector2 pos);
void nodeNext(ListNode*& cur, Vector2& pos,int& framecntDel, stack<pair<ListNode*, Vector2>>& st);
void drawTextCode(int curline, int curlinetmp);