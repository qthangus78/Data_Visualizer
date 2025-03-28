#pragma once
#include <string>
#include <fstream>
#include <raylib.h>
#include "GlobalVar.h"

//--------------------------------
// Các hàm thao tác LinkedList
//--------------------------------
void insertAl(ListNode*& root, ListNode*& tail, int x);
bool delAl(ListNode*& root, ListNode*& tail,int x);
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
    IState* mInsert;
    IState* mDelete;
    IState* mFind;
    IState* mClear;

public:
    SSL();
    ~SSL();
    // getter/setter
    ListNode* getRoot();
    ListNode* getTail();
    ListNode* getprevTail();
    void setprevTail(ListNode* APrev);
    void setRoot(ListNode* tmp);
    void setExistVal(ListNode* tmp);
    ListNode* getExistVal();
    Vector2 getPosEnd();
    void setPosEnd(Vector2 APos);
    // hàm thao tác LinkedList
    void insertList(int x);
    bool delList(int x);
    ListNode* findList(int x);
    void delAllList();
    void fileInput(std::ifstream& fin);
    // chuyển state
    void setState(IState* state);
    IState* getnotInMode();
    IState* getInsert();
    IState* getDel();
    IState* getFind();
    IState* getClear();
    // main loop
    void draw();
    void handle();
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
//--------------------------------
class Insert : public IState {
private:
    SSL* mSSL;
    int frameCounter;
    int framecntInsert;
    std::string textIn;
    std::string errorMessage;
    button fileInput;
    bool InsertProcess;
    ListNode* currentAnimatingNode;
    Vector2 posTail;
public:
    Insert(SSL* s);
    void drawInputFile();
    void handleInputFile();
    void insertAnimation(ListNode*& tmp,Vector2& posTail, int deltax, int deltay);
    void draw() override;
    void handle() override;
};
//--------------------------------
class Delete : public IState {
private:
    SSL* mSSL;
    int frameCounter;
    std::string textIn;
    bool existVal;
public:
    Delete(SSL* s);
    void draw() override;
    void handle() override;
};
//--------------------------------
class Find : public IState {
private:
    SSL* mSSL;
    int frameCounter;
    int framecntFind;
    bool FindProcess;
    ShadedData shadedPos;
    ListNode* currentAnimatingNode;
    std::string textIn;
public:
    Find(SSL* s);
    void findAnimation(ListNode*& root);
    void draw() override;
    void handle() override;
};
class Clear : public IState{
private:
    SSL* mSSL;
public:
    Clear(SSL* s);
    void draw() override;
    void handle() override;
};
//--------------------------------
// Hàm tiện ích vẽ chung
//--------------------------------
void drawNode(Vector2 pos, const std::string& text, float radius);
void drawArrow(Vector2 start, Vector2 end, Color edgeRender);
void drawLinkedList(ListNode* root);
void drawPartofLinkedList(ListNode* root, ListNode* EndPart);
void drawPartofLinkedListNotColor(ListNode* root, ListNode* EndPart, SSL* s);
void drawButtons();
void handleButtonsClick(SSL* SSL);
void handleButtonsHover();
void drawBlinkingLines(const std::string& text, Rectangle rect, int& frameCounter);
void drawTextIn(const std::string& text, Rectangle rect, int& frameCounter);
Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);
float Vector2Length(Vector2 v);
Vector2 Vector2Add(Vector2 v1, Vector2 v2);
Vector2 Vector2Scale(Vector2 v, float scale);
void drawArrow2Node(Vector2 start, Vector2 end, Color edgeRender);