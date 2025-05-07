#include "AVLTree.h"
#include "GlobalVar.h"
#include <raylib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "tinyfiledialogs.h"
//NotInMode
notInModeAVL::notInModeAVL(AVL* AVL) : mAVL(AVL) {}
void notInModeAVL::draw(){
    drawButtonAVL();
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
}
void notInModeAVL::handle(){
    handleButtonsHoverAVL();
    handleButtonsClickAVL(mAVL);
}
//Create
CreateAVL::CreateAVL(AVL* AVL) : mAVL(AVL), progress(0.0f), type(Type::None), fileProcess(false), initProcess(false), inProcess(false) {
    fileInput = {{ buttonVar::buttonCreate.rect.x + 115, buttonVar::buttonCreate.rect.y, 120, (float)button::sizeH }, color::buttonFile, "Load File"};
    random = {{ buttonVar::buttonIns.rect.x + 115, buttonVar::buttonIns.rect.y, 120, (float)button::sizeH }, color::buttonFile, "Init"};
}
void CreateAVL::drawFile(){
    DrawButtonAVL(fileInput.rect, fileInput.text, fileInput.buCol, SSLFont, 18);
}
void CreateAVL::drawInit(){
    DrawButtonAVL(random.rect, random.text, random.buCol, SSLFont, 18);
}
void CreateAVL::handleFile(){
    const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
    if(filePath){
        std::string pathFile = filePath;
        std::ifstream fin(pathFile);
        if(fin.is_open())
        {
        clearAVL(mAVL->getRoot());
        mAVL->getEdge().clear();
        FileInput(fin,mAVL->getRoot());
        inProcess = true;
        fileProcess = false;
        }
    }
}
void CreateAVL::handleInit() {
    clearAVL(mAVL->getRoot());
    mAVL->getEdge().clear();
    int nums = GetRandomValue(1, 31);
    for (int i = 0; i < nums; i++) {
        int value = GetRandomValue(0, 1000);
        mAVL->getRoot() = insertNode(mAVL->getRoot(), value);
        if (mAVL->getRoot()->height >= 6) {
            int maxAttempts = 100;
            while (mAVL->getRoot() && mAVL->getRoot()->height >= 6 && maxAttempts--) {
                int valdel = mAVL->getRoot()->data;
                mAVL->getRoot() = deleteNode(mAVL->getRoot(), valdel);
            }
            break; 
        }
    }
    inProcess = true;
    initProcess = false;
}
void CreateAVL::CreateAnimation(){
    std::vector<Node*> node = mAVL->BFSOrder();
    if(progress == 0.0f){
        for(auto u : node){
            if(u->left){
                EdgeAVL edge(BLACK,u,u->left);
                mAVL->getEdge().push_back(edge);
            }
            if(u->right){
                EdgeAVL edge(BLACK,u,u->right);
                mAVL->getEdge().push_back(edge);
            }
        }
    }
    progress += deltaTimeAVL;
    std::vector<Vector2> oldPos;
    std::vector<Vector2> newPos;
    for(int i = 0; i<node.size(); i++) oldPos.push_back(posRoot);
    newPos = calposAVLTree(mAVL->getRoot());
    for(int i = 0; i < node.size(); i++) node[i]->pos = lerpAVL(oldPos[i],newPos[i],progress);
    if(progress >= 1.0f){
        inProcess = false;
        progress = 0.0f;
    }
}
void CreateAVL::draw(){
    drawFile();
    drawInit();
    drawButtonAVL();
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    if(inProcess) CreateAnimation();
}
void CreateAVL::handle(){
    handleButtonsHoverAVL();
    handleButtonsClickAVL(mAVL);
    if ((CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
    type = Type::None;
    }   
    if(initProcess) handleInit();
    if(fileProcess) handleFile();
    if (CheckCollisionPointRec(mouse, fileInput.rect)){
        fileInput.buCol = color::buttonFileHovered;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        type = (type == Type::None || type == Type::Init) ? Type::File : Type::None;
    }
    else if (CheckCollisionPointRec(mouse, random.rect)){
        random.buCol = color::buttonFileHovered;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        type = (type == Type::None || type == Type::File) ? Type::Init : Type::None;
    }
    else{
        fileInput.buCol = color::buttonFile;
        random.buCol = color::buttonFile;
    }
    if(type != Type::None){
        int key = GetKeyPressed();
        if(key == KEY_ENTER && type == Type::Init) initProcess = true;
        if(key == KEY_ENTER && type == Type::File) fileProcess = true;
    }
}
//Insert
InsertAVL::InsertAVL(AVL* AVL) : mAVL(AVL), frameCounter(0), framecntIns(0), textIn(""), progress(0.0f), prev(nullptr), tmp(nullptr), type(RotateType::None),
inProcess(false), insertRoot(false), insertNode(false), returnNode(false) {
    inputRect = {buttonVar::buttonIns.rect.x + 115,buttonVar::buttonIns.rect.y, button::sizeW, button::sizeH};
}
void InsertAVL::InsertRoot(Node*& pRoot, int x){
    pRoot = new Node(x,posRoot);
    newNodeAVL(pRoot,progress);
    if(progress >= 1.0f){
        insertRoot = false;
        progress = 0.0f;
    }
}
void InsertAVL::InsertNode(Node*& pRoot, int x){
    prev->color = YELLOW;
    if(prev->data == x){
        returnNode = true;
        insertNode = false;
        return;
    }
    if(progress >= 1.0f){
        path.push_back(prev);
        EdgeAVL& pretmp = findEdge(prev,tmp,mAVL->getEdge());
        pretmp.color = YELLOW;
        progress = 0.0f;
        framecntIns = 0;
        prev = tmp;
        prev->color = YELLOW;
        if(tmp->data < x) tmp = tmp->right;
        else if(tmp->data > x) tmp = tmp->left;
        else if(tmp->data == x){
        returnNode = true;
        insertNode = false;
        return;
        }
    }
    if(!tmp) {
        path.push_back(prev);
        Node* newNode;
        if(x < prev->data){
            newNode = new Node(x, {prev->pos.x - 50, prev->pos.y + 50});
            prev->left = newNode;
        } else {
            newNode = new Node(x, {prev->pos.x + 50, prev->pos.y + 50});
            prev->right = newNode;
        }
        mAVL->pushEdge(EdgeAVL(BLACK, prev, newNode));
        prev = newNode;
        insertNode = false;
        inProcess = true;
        for(int i=path.size()-1; i>=0; i--) {
        path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right))+1;
        }
        handlePos(mAVL, oldPos, newPos);
    }
    if(progress == 0.0f) framecntIns++;
    if(tmp && framecntIns >= 20) {
        DrawEdgeColor(prev->pos,tmp->pos,progress,YELLOW);
        DrawNodeAVL(prev);
    }
}
void InsertAVL::InsertAnimation(int x){
    if(progress == 0.0f && type == RotateType::None) framecntIns++;
    if(path.empty() && framecntIns >= 20){
        prev->color = WHITE;
        inProcess = false;
        framecntIns = 0;
        return;
    }
    if(prev->data == x){
        newNodeAVL(prev,progress);
        updatePos(mAVL->getRoot());
        if(progress > 1.0f){
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
        }
    }
    if(framecntIns > 20 && type == RotateType::None){
        prev->color = WHITE;
        if(findEdge(path.back(),prev,mAVL->getEdge()).par != nullptr) DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(findEdge(path.back(),prev,mAVL->getEdge()).par == nullptr){
            framecntIns = 0;
            prev = path.back();
            path.pop_back();
        }
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            edge.color = BLACK;
            framecntIns = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
        }
    }
    if (getBalance(prev) > 1 && (x < prev->left->data || prevType == RotateType::RightLeft) && progress == 0.0f) {
        type = RotateType::RightRight;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }  // LL case
    else if (getBalance(prev) < -1 && (x > prev->right->data || prevType == RotateType::LeftRight) && progress == 0.0f) {
        type = RotateType::LeftLeft;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }    // RR case
    else if (getBalance(prev) > 1 && x > prev->left->data && progress == 0.0f && type == RotateType::None) {
        type = RotateType::RightLeft;
        path.push_back(prev);
        prev = prev->left;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }   // LR case
    else if (getBalance(prev) < -1 && x < prev->right->data && progress == 0.0f && type == RotateType::None){
        type = RotateType::LeftRight;
        path.push_back(prev);
        prev = prev->right;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    };
    if(type == RotateType::RightRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None; 
        }
    }
    if(type == RotateType::LeftLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
        }
    }
    if(type == RotateType::RightLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::RightLeft;
        }
    }
    if(type == RotateType::LeftRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::LeftRight;
        }
    }
}
void InsertAVL::returnAnimation(){
    if(progress == 0.0f) framecntIns++;
    if(path.empty() && framecntIns >= 20){
        prev->color = WHITE;
        inProcess = false;
        framecntIns = 0;
        return;
    }
    if(framecntIns > 20){
        prev->color = WHITE;
        DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            edge.color = BLACK;
            framecntIns = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
        }   
    }
}
void InsertAVL::updatePos(Node*& pRoot){
    std::vector<Node*> node = mAVL->BFSOrder();
    for(int i=0; i<node.size(); i++){
        node[i]->pos = lerpAVL(oldPos[i],newPos[i],progress);
    }
}
void InsertAVL::draw(){
    drawButtonAVL();
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextInAVL(textIn, inputRect, frameCounter);
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    if(insertRoot) InsertRoot(mAVL->getRoot(),value);
    if(insertNode) InsertNode(mAVL->getRoot(),value);
    if(inProcess) InsertAnimation(value);
    if(returnNode) returnAnimation();
}
void InsertAVL::handle(){
    handleButtonsHoverAVL();
    handleButtonsClickAVL(mAVL);
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    if (key == KEY_ENTER && !textIn.empty()) {
        value = std::stoi(textIn);
        if(!mAVL->getRoot()) insertRoot = true;
        else {
            prev = mAVL->getRoot();
            tmp = (prev->data < value) ? prev->right : prev->left; 
            insertNode = true;
            prevType = RotateType::None;
        }
        textIn.clear();
    }
}
//Delete
DeleteAVL::DeleteAVL(AVL* AVL) : mAVL(AVL), frameCounter(0), framecntDel(0), progress(0.0f), type(RotateType::None),
inProcess(false), returnNode(false), deleteNode(false), deleteLeaf(false), firstDelete(false) {
    inputRect = {buttonVar::buttonDel.rect.x + 115,buttonVar::buttonDel.rect.y, button::sizeW, button::sizeH};
}
void DeleteAVL::DeleteAl(Node*& pRoot, Node*& x, Node*& y, std::vector<EdgeAVL>& edges, float& progress) {
    // On first frame, set up parents, path and rewire tree
    if (progress == 0.0f) {
        Node* parX = findParent(pRoot, x);
        Node* parY = findParent(pRoot, y);
        // Replace x with y in the tree structure
        if (parX) {
            if (parX->left == x) parX->left = y;
            else                   parX->right = y;
            auto& e_px = findEdge(parX, x, edges);
            e_px.child = y;
        } else {
            pRoot = y;
        }
        // Remove y from its original position and rewire its subtree
        if (parY != x) {
            parY->left = y->right;
            if (y->right) {
                auto& e_py = findEdge(parY, y, edges);
                e_py.child = y->right;
                e_py.color = BLACK;
                // remove the old edge from y -> y->right
                edges.erase(std::remove_if(edges.begin(), edges.end(), [&](auto& e){ return &e == &findEdge(y, y->right, edges); }), edges.end());
            } else {
                // remove the old edge between parY and y
                edges.erase(std::remove_if(edges.begin(), edges.end(), [&](auto& e){ return &e == &findEdge(parY, y, edges); }), edges.end());
            }
        } else {
            // y was direct child of x; just remove x->y edge
            edges.erase(std::remove_if(edges.begin(), edges.end(), [&](auto& e){ return &e == &findEdge(x, y, edges); }), edges.end());
        }
        // Attach x's children under y
        if (x->left && y != x->left) {
            y->left = x->left;
            auto& e_l = findEdge(x, x->left, edges);
            e_l.par = y;
        }
        if (x->right && y != x->right) {
            y->right = x->right;
            auto& e_r = findEdge(x, x->right, edges);
            e_r.par = y;
        }
        // Build the path for animation
        path.clear();
        Node* walk = pRoot;
        Node* stopAt = (parY == x) ? y : parY;
        if (stopAt) {
            while (walk != stopAt) {
                path.push_back(walk);
                walk = (walk->data < stopAt->data ? walk->right : walk->left);
            }
        }
        if (walk) path.push_back(walk);
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right))+1;
        handlePos(mAVL, oldPos, newPos);
    }
    // Animate the transition
    progress = std::min(progress + deltaTimeAVL, 1.0f);
    updatePos(pRoot);
    DrawNodeAVL(x);
    x->color = lerpAVL(BLUE, {192, 245, 242, 100}, progress);
    x->textColor = lerpAVL(BLACK,{192, 245, 242, 100}, progress);
    // Finalize deletion when animation completes
    if (progress >= 1.0f) {
        delete x;
        x = path.back();
        progress = 0.0f;
        firstDelete = false;
    }
}
void DeleteAVL::rotateAVL(){
    if(progress == 0.0f && type == RotateType::None) framecntDel++;
    if(path.empty() && framecntDel >= 20){
        prev->color = WHITE;
        inProcess = false;
        deleteLeaf = false;
        framecntDel = 0;
        return;
    }
    if(framecntDel > 20 && type == RotateType::None){
        prev->color = WHITE;
        if(findEdge(path.back(),prev,mAVL->getEdge()).par != nullptr) DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(findEdge(path.back(),prev,mAVL->getEdge()).par == nullptr){
            framecntDel = 0;
            prev = path.back();
            path.pop_back();
        }
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            edge.color = BLACK;
            framecntDel = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
        }
    }
    if (getBalance(prev) > 1 && (getBalance(prev->left) >= 0 || prevType == RotateType::RightLeft) && progress == 0.0f) {
        type = RotateType::RightRight;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }  // LL case
    if (getBalance(prev) < -1 && (getBalance(prev->right) <= 0 || prevType == RotateType::LeftRight) && progress == 0.0f) {
        type = RotateType::LeftLeft;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }    // RR case
    if (getBalance(prev) > 1 && getBalance(prev->left) < 0 && progress == 0.0f && type == RotateType::None) {
        type = RotateType::RightLeft;
        path.push_back(prev);
        prev = prev->left;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }   // LR case
    if (getBalance(prev) < -1 && getBalance(prev->right) > 0 && progress == 0.0f && type == RotateType::None){
        type = RotateType::LeftRight;
        path.push_back(prev);
        prev = prev->right;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    };
    if(type == RotateType::RightRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None; 
        }
    }
    if(type == RotateType::LeftLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
        }
    }
    if(type == RotateType::RightLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::RightLeft;
        }
    }
    if(type == RotateType::LeftRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::LeftRight;
        }
    }
}
void DeleteAVL::DeleteNode(Node*& pRoot, int x){
    if(prev->data != x) prev->color = YELLOW;
    if((!prev->left && prev->data > x) || (!prev->right && prev->data < x)){
        returnNode = true;
        deleteNode = false;
        return;
    }
    if(progress >= 1.0f && prev->data != x){
        EdgeAVL& pretmp = findEdge(prev,tmp,mAVL->getEdge());
        path.push_back(prev);
        pretmp.color = YELLOW;
        progress = 0.0f;
        framecntDel = 0;
        prev = tmp;
        if(tmp->data < x) tmp = tmp->right;
        else if(tmp->data > x) tmp = tmp->left;
    }
    if(prev->data == x) {
        prev->color = BLUE;
        if(!prev->left && !prev->right){
            deleteNode = false;
            deleteLeaf = true;
            firstDelete = true;
        }
        else if(!prev->right){
            DrawEdgeColor(prev->pos,prev->left->pos,progress,YELLOW);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                EdgeAVL& pretmp = findEdge(prev,prev->left,mAVL->getEdge());
                pretmp.color = YELLOW;
                tmp = prev->left;
                tmp->color = BLUE;
                progress = 0.0f;
                inProcess = true;
                deleteNode = false;
                firstDelete = true;
            }
        }
        else{
            if(tmp == prev){
                if(progress == 0.0f) framecntDel++;
                if(framecntDel >= 20){
                DrawEdgeColor(prev->pos,prev->right->pos,progress,YELLOW);
                DrawNodeAVL(prev);
                }
                if(progress >= 1.0f){
                    EdgeAVL& pretmp = findEdge(prev,prev->right,mAVL->getEdge());
                    pretmp.color = YELLOW;
                    tmp = prev->right;
                    if(!tmp->left){
                        progress = 0.0f;
                        tmp->color = BLUE;
                        framecntDel = 0;
                        inProcess = true;
                        deleteNode = false;
                        firstDelete = true;
                    }
                    else{
                        progress = 0.0f;
                        framecntDel = 0;
                        tmp->color = YELLOW;
                    }
                }
            }
            else{
                if(tmp->left){
                    if(progress == 0.0f) framecntDel++;
                    if(framecntDel >= 20){
                    DrawEdgeColor(tmp->pos,tmp->left->pos,progress,YELLOW);
                    DrawNodeAVL(tmp);
                    }
                    if(progress >= 1.0f){
                        EdgeAVL& pretmp = findEdge(tmp,tmp->left,mAVL->getEdge());
                        pretmp.color = YELLOW;
                        progress = 0.0f; 
                        framecntDel = 0;
                        tmp = tmp->left;
                        tmp->color = YELLOW;
                    }
                }
                else{
                    framecntDel = 0;
                    tmp->color = BLUE;
                    inProcess = true;
                    deleteNode = false;
                    firstDelete = true;
                }
            }
        } 
    }
    if(progress == 0.0f) framecntDel++;
    if(prev->data != x && framecntDel >= 20) {
        DrawEdgeColor(prev->pos,tmp->pos,progress,YELLOW);
        DrawNodeAVL(prev);
    }
}
void DeleteAVL::DeleteLeaf(Node*& pRoot) {
    if (firstDelete) {
        // Shrink node radius for deletion animation
        progress += deltaTimeAVL;
        Node* par = findParent(mAVL->getRoot(),prev);
        if(par){
            EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
            edge.color = lerpAVL(YELLOW,{192, 245, 242, 100}, progress);
        }
        prev->color = lerpAVL(BLUE, {192, 245, 242, 100}, progress);
        prev->textColor = lerpAVL(BLACK, {192, 245, 242, 100}, progress);
        if (progress >= 1.0f) {
            if (!path.empty()) {
                EdgeAVL& prevtmp = findEdge(path.back(), prev, mAVL->getEdge());
                auto it = std::find_if(mAVL->getEdge().begin(), mAVL->getEdge().end(), 
                    [&](const EdgeAVL& e) { return &e == &prevtmp; });
                if (it != mAVL->getEdge().end())  
                    mAVL->getEdge().erase(it);
                // Update parent's child pointer
                Node* parent = path.back();
                if (parent->left == prev) parent->left = nullptr;
                else if (parent->right == prev) parent->right = nullptr;
            }
            delete prev;
            prev = nullptr;
            // Update heights on path
            for (int i = (int)path.size() - 1; i >= 0; --i) {
                path[i]->height = std::max(getHeight(path[i]->left), getHeight(path[i]->right)) + 1;
            }
            // Move to parent or finish
            if (path.empty()) {
                deleteLeaf = false;
                std::cout << deleteLeaf;
                pRoot = nullptr; 
            } else {
                prev = path.back();
                path.pop_back();
            }
            progress = 0.0f;
            firstDelete = false; 
        }
    } else {
        // After leaf removal, rebalance
        rotateAVL();
    }
}
void DeleteAVL::DeleteAnimation(Node*& pRoot, int x){
    if(firstDelete) DeleteAl(mAVL->getRoot(),prev,tmp,mAVL->getEdge(),progress);
    else rotateAVL();
}
void DeleteAVL::returnAnimation(){
    if(progress == 0.0f) framecntDel++;
    if(path.empty() && framecntDel >= 20){
        prev->color = WHITE;
        inProcess = false;
        returnNode = false;
        framecntDel = 0;
        return;
    }
    if(framecntDel > 20){
        prev->color = WHITE;
        DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            edge.color = BLACK;
            framecntDel = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
        }   
    }
}
void DeleteAVL::updatePos(Node*& pRoot){
    std::vector<Node*> node = mAVL->BFSOrder();
    for(int i=0; i<node.size(); i++){
        node[i]->pos = lerpAVL(oldPos[i],newPos[i],progress);
    }
}
void DeleteAVL::draw(){
    drawButtonAVL();
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextInAVL(textIn, inputRect, frameCounter);
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    if(deleteNode) DeleteNode(mAVL->getRoot(),value);
    if(deleteLeaf) DeleteLeaf(mAVL->getRoot());
    if(inProcess) DeleteAnimation(mAVL->getRoot(),value);
    if(returnNode) returnAnimation();
}
void DeleteAVL::handle(){
    handleButtonsHoverAVL();
    handleButtonsClickAVL(mAVL);
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    if (key == KEY_ENTER && !textIn.empty()) {
        value = std::stoi(textIn);
        if(mAVL->getRoot()) {
            prev = mAVL->getRoot();
            if(prev->data == value) tmp = prev;
            else{
                tmp = (prev->data < value) ? prev->right : prev->left;
            } 
            deleteNode = true;
            prevType = RotateType::None;
            path.clear();
        }
        textIn.clear();
    }
}
FindAVL::FindAVL(AVL* AVL) : mAVL(AVL), frameCounter(0), framecntF(0), progress(0), findNode(false), prev(nullptr) {
    inputRect = {buttonVar::buttonF.rect.x + 115,buttonVar::buttonF.rect.y, button::sizeW, button::sizeH};
}
void FindAVL::FindAnimation(Node*& pRoot, int x){
    prev->color = YELLOW;
    if(prev->data == x){
        findNode = false;
        prev->color = BLUE;
        return;
    }
    if(progress >= 1.0f && tmp){
        progress = 0.0f;
        framecntF = 0;
        prev->color = WHITE;
        prev = tmp;
        if(tmp->data < x) tmp = tmp->right;
        else if(tmp->data > x) tmp = tmp->left;
    }
    if(progress == 0) framecntF++;
    if(!tmp && framecntF >= 20){
            framecntF = 0;
            prev->color = WHITE;
            findNode = false;
    }
    if(tmp && framecntF >= 20 && tmp != prev) {
        DrawEdgeColor(prev->pos,tmp->pos,progress,YELLOW);
        DrawNodeAVL(prev);
    }
}
void FindAVL::draw(){
    drawButtonAVL();
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextInAVL(textIn, inputRect, frameCounter);
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    if(findNode) FindAnimation(mAVL->getRoot(),value);
}
void FindAVL::handle(){
    handleButtonsHoverAVL();
    handleButtonsClickAVL(mAVL);
    if ((CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
    if (prev) prev->color = WHITE;
    }   
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    if (key == KEY_ENTER && !textIn.empty()) {
        value = std::stoi(textIn);
        if(mAVL->getRoot()) {
            if(prev) prev->color = WHITE;
            prev = mAVL->getRoot();
            if(prev->data == value) tmp = prev;
            else{
                tmp = (prev->data < value) ? prev->right : prev->left;
            } 
            findNode = true;
        }
        textIn.clear();
    }
}
ClearAVL::ClearAVL(AVL* AVL) : mAVL(AVL), progress(0.0f), clearAll(false) {}
void ClearAVL::clearAnimation(){
    progress += deltaTimeAVL;
    std::vector<Node*> node = mAVL->BFSOrder();
    for(auto u : node) {
        u->color = lerpAVL(WHITE,{192, 245, 242, 100}, progress);
        u->textColor = lerpAVL(BLACK,{192, 245, 242, 100}, progress);
    }
    for(auto& edge : mAVL->getEdge()) edge.color = lerpAVL(BLACK,{192, 245, 242, 100}, progress);
    if(progress >= 1.0f){
        progress = 0.0f;
        clearAll = false;
        clearAVL(mAVL->getRoot());
    }
}
void ClearAVL::draw(){
    drawButtonAVL();
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    if(clearAll) clearAnimation();
}
void ClearAVL::handle(){
    handleButtonsHoverAVL();
    handleButtonsClickAVL(mAVL);
    int key = GetKeyPressed();
    if(key == KEY_ENTER){
        if(mAVL->getRoot()) clearAll = true;
    }
}