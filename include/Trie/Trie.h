#pragma once
#include "GlobalVar.h"
#include <raylib.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "LinkedList.h"

using namespace std;


//------------------
//Định dạng Trie Node
//---------------------

class TrieNode {
public:

    // pointer array for child nodes of each node
    map<char, TrieNode*> children;

    // Used for indicating ending of string
    bool isWord;

    //Used for drawing
    int subtreeSize;
    Vector2 pos, posPrev, posCur;
    bool isNewNode;
    bool isDeleted;
    Color colorNode, colorText;
    char character;
    

    TrieNode();
};

struct Step{
    string description;
    int line;
    TrieNode* node;
    int function;
    char character;
};

enum functionStep {
    moving = 1,
    createnode = 2,
    markword = 3,
    invalid = 4,
    erase = 5,
};

//-----------------------
//      Function chinh
//----------------------
void InsertTrie(TrieNode*& root, const string& key,queue<Step>& steps);

bool FindTrie(TrieNode*& root, const string& key, queue<Step>& steps);
bool FindTrieNoStep(TrieNode*& root, const string& key);

void deleteNode(TrieNode* root, bool isStepbyStep);

bool isEmpty(TrieNode* root);

TrieNode* DeleteTrieOrigin(TrieNode* root, const string& key, int depth, queue<Step>& steps, bool isStepbyStep);

void InputFileTrie(const string& file_name, TrieNode*& root, queue<Step> steps);

void ClearTrie(TrieNode*& root, int depth, queue<Step>& steps, bool isStepbyStep);

//Draw

void drawArrow(Vector2 start, Vector2 end, Color edgeRender);

void drawButtons();

void handleButtonsHover();

void drawBlinkingLines(const std::string& text, Rectangle rect, int& frameCounter);

void drawTextIn(const std::string& text, Rectangle rect, int& frameCounter);






