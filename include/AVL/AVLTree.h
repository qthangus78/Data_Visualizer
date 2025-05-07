#pragma once
#include "GlobalVar.h"
#include "button.h"
#include <raylib.h>
#include <vector>
#include <string>
#include <fstream>
//Struct of AVL
struct Node{
    int data;
    Vector2 pos;
    float radius, font;
    int height;
    Color color, textColor;
    Node* left, * right;
    Node(int data, Vector2 pos, float radius = 0.0f, float font = 0.0f, int height = 1, Color color = WHITE, Color textColor = BLACK, Node* left = nullptr, Node* right = nullptr) 
    : data(data), pos(pos), color(color), textColor(textColor), left(left), right(right), radius(radius), font(font), height(height) {}
};
const float NodeSizeAVL = 25.0f, FontNodeAVL = 18.0f;
const float deltaTimeAVL = 0.01f, dis = 45.0f;
const Vector2 posRoot = {screenWidth/2.0f,screenHeight/4.0f};
//Edge
struct EdgeAVL{
    Color color;
    Node* par, *child;
    EdgeAVL(Color color = BLACK, Node* par = nullptr, Node* child = nullptr) : color(color), par(par), child(child) {}
};
//Function associated with AVLTree
int getHeight(Node* node);
int getBalance(Node* node);
Node* findParent(Node*& pRoot, Node*& y);
Node* rightRotate(Node*& pRoot, Node*& y, std::vector<EdgeAVL>& edge);
Node* leftRotate(Node*& pRoot, Node*& y, std::vector<EdgeAVL>& edge);
Node* rightRotate( Node* y);
Node* leftRotate(Node* y);
Node* insertNode(Node* pRoot, int x);
void FileInput(std::ifstream& fin, Node*& pRoot);
Node* minValueNode(Node* node);
Node* deleteNode(Node* root, int key);
void clearAVL(Node*& pRoot);
//Interface
class IStateAVL{
public:
    enum RotateType {RightRight, RightLeft, LeftLeft, LeftRight, None};
    virtual void draw() = 0;
    virtual void handle() = 0;
};
//AVL
class AVL{
private: 
    Node* pRoot;
    IStateAVL* mCurrent;
    IStateAVL* mNotInMode;
    IStateAVL* mCreate;
    IStateAVL* mInsert;
    IStateAVL* mDelete;
    IStateAVL* mFind;
    IStateAVL* mClear;
    std::vector<EdgeAVL> edge;
public: 
    AVL();
    ~AVL();
    //getter/setter
    Node*& getRoot();
    IStateAVL* getCurrent();
    IStateAVL* getNotInMode();
    IStateAVL* getCreate();
    IStateAVL* getInsert();
    IStateAVL* getDelete();
    IStateAVL* getFind();
    IStateAVL* getClear();
    void setState(IStateAVL* state);
    std::vector<EdgeAVL>& getEdge();
    void pushEdge(EdgeAVL newEdge);
    std::vector<Node*> BFSOrder();
    void draw();
    void handle();
};
//notInMode
class notInModeAVL : public IStateAVL{
private:
    AVL* mAVL;
public:
    notInModeAVL(AVL* AVL);
    void draw() override;
    void handle() override;
};
class CreateAVL : public IStateAVL{
public:
    enum class Type {File, Init, None};
private: 
    AVL* mAVL;
    float progress;
    Type type;
    button fileInput, random;
    bool fileProcess, initProcess, inProcess;
public:
    CreateAVL(AVL* AVL);
    void handle() override;
    void draw() override;
    void drawFile();
    void drawInit();
    void handleFile();
    void handleInit();
    void CreateAnimation();
}; 
//Insert
class InsertAVL : public IStateAVL{  
private: 
    AVL* mAVL;
    int frameCounter, framecntIns, value;
    std::string textIn;
    float progress;
    Rectangle inputRect;
    std::vector<Vector2> oldPos;
    std::vector<Vector2> newPos;
    std::vector<Node*> path;
    bool inProcess, insertRoot, insertNode, returnNode;
    Node* prev, *tmp;
    RotateType type, prevType;
public: 
    InsertAVL(AVL* AVL);
    void InsertNode(Node*& pRoot, int x);
    void InsertRoot(Node*& pRoot, int x);
    void InsertAnimation(int x);
    void returnAnimation();
    void updatePos(Node*& pRoot);
    void draw() override;
    void handle() override;
};
class DeleteAVL : public IStateAVL{
private: 
    AVL* mAVL;
    int frameCounter, framecntDel, value;
    std::string textIn;
    float progress;
    Rectangle inputRect;
    std::vector<Vector2> oldPos;
    std::vector<Vector2> newPos;
    std::vector<Node*> path;
    bool inProcess, deleteNode, returnNode, deleteLeaf, firstDelete;
    Node* prev,* tmp;
    RotateType type, prevType;
public:
    DeleteAVL(AVL* AVL);
    void handle() override;
    void draw() override;
    void DeleteAl(Node*& pRoot, Node*& x, Node*& y, std::vector<EdgeAVL>& edge, float& progress);
    void updatePos(Node*& pRoot);
    void rotateAVL();
    void returnAnimation();
    void DeleteNode(Node*& pRoot, int x);
    void DeleteLeaf(Node*& pRoot);
    void DeleteAnimation(Node*& pRoot, int x);
};
class FindAVL : public IStateAVL{
private:
    AVL* mAVL;
    int frameCounter, framecntF, value;
    std::string textIn;
    float progress;
    Rectangle inputRect;
    bool findNode;
    Node* prev,* tmp;
public:
    FindAVL(AVL* AVL);
    void handle() override;
    void draw() override;
    void FindAnimation(Node*& pRoot, int x);
};
class ClearAVL : public IStateAVL{
private:
    AVL* mAVL;
    float progress;
    bool clearAll;
public:
    ClearAVL(AVL* AVL);
    void handle() override;
    void draw() override;
    void clearAnimation();
};
//Reuseable utility
void DrawNodeAVL(Node* node);
void DrawEdge(EdgeAVL edge);
void DrawEdgeColor(Vector2 start, Vector2 end, float& progress, Color color);
EdgeAVL& findEdge(Node* par, Node* child, std::vector<EdgeAVL>& edge);
void DrawAVLTree(Node* pRoot, std::vector<EdgeAVL>& edge);
void DrawButtonAVL(Rectangle rect, const std::string &text, Color &color, Font font, float fontsize);
void drawButtonAVL();
void handleButtonsHoverAVL();
void handleButtonsClickAVL(AVL* AVL);
void drawBlinkingLinesAVL(const std::string& text, Rectangle rect, int& frameCounter);
void drawTextInAVL(const std::string& text, Rectangle rect, int& frameCounter);
std::vector<Vector2> calposAVLTree(Node*& pRoot);
Vector2 Vector2SubtractAVL(Vector2 v1, Vector2 v2);
float Vector2LengthAVL(Vector2 v);
Vector2 Vector2AddAVL(Vector2 v1, Vector2 v2);
Vector2 Vector2ScaleAVL(Vector2 v, float scale);
float smoothstepAVL(float t);
Vector2 lerpAVL(Vector2 start, Vector2 end, float progress);
float lerpAVL(float start, float end, float progress);
Color lerpAVL(Color start, Color end, float progress);
void newNodeAVL(Node*& node, float& progress);
void handlePos(AVL*& AVL, std::vector<Vector2>& oldPos, std::vector<Vector2>& newPos);