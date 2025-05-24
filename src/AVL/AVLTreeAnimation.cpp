#include "AVLTree.h"
#include "GlobalVar.h"
#include <raylib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
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
CreateAVL::CreateAVL(AVL* AVL) : mAVL(AVL), progress(0.0f), type(Type::None), fileProcess(false), initProcess(false), inProcess(false), slideProcess(false) {
    fileInput = {{ buttonVar::buttonCreate.rect.x + 115, buttonVar::buttonCreate.rect.y, 120, (float)button::sizeH }, color::buttonFile, "Load File"};
    random = {{ buttonVar::buttonIns.rect.x + 115, buttonVar::buttonIns.rect.y, 120, (float)button::sizeH }, color::buttonFile, "Init"};
}
void CreateAVL::drawFile(){
    DrawButtonAVL(fileInput.rect, fileInput.text, fileInput.buCol, SSLFont, 18);
}
void CreateAVL::drawInit(){
    DrawButtonAVL(random.rect, random.text, random.buCol, SSLFont, 18);
}
CreateAVL::Type CreateAVL::DetectCurrentMode(){
    if (CheckCollisionPointRec(mouse, fileInput.rect)) {
        fileInput.buCol = color::buttonFileHovered;
        return Type::File;
    }
    if (CheckCollisionPointRec(mouse, random.rect)) {
        random.buCol = color::buttonFileHovered;
        return Type::Init;
    }
    fileInput.buCol = color::buttonFile;
    random.buCol = color::buttonFile;
    return Type::None;
}
void CreateAVL::handleModeTransitTion(Type newType){
    if(progress != 0.0f) return;
    prevtype = type;
    type = (type == newType) ? Type::None : newType;
    slideProcess = true;
    progress = 0.0f;
}
void CreateAVL::updateCommonAnimation(){
        if(!slideProcess) return;
        switch (type)
        {
        case Type::File:
            handleFileMode();
            break;
        case Type::Init:
            handleRandomMode();
            break;
        default:
            break;
        }
}
void CreateAVL::handleFileMode(){
    //Di chuyển button go
    switch(prevtype){
        case Type::None:
        slideProcess = false;
        buttonVar::buttonGo.rect.y = buttonVar::buttonCreate.rect.y;
        break;
        case Type::Init:
        progress += deltaTimeAVL;
        buttonVar::buttonGo.rect.y = lerpAVL(buttonVar::buttonGo.rect.y,buttonVar::buttonCreate.rect.y,progress);
        if(buttonVar::buttonGo.rect.y == buttonVar::buttonCreate.rect.y){
            slideProcess = false;
            progress = 0.0f;
        }
        break;
        default:
        break;
    }
}
void CreateAVL::handleRandomMode(){
    //Di chuyển button Go
    switch(prevtype){
        case Type::None:
        slideProcess = false;
        buttonVar::buttonGo.rect.y = buttonVar::buttonIns.rect.y;
        break;
        case Type::File:
        progress += deltaTimeAVL;
        buttonVar::buttonGo.rect.y = lerpAVL(buttonVar::buttonGo.rect.y,buttonVar::buttonIns.rect.y,progress);
        if(buttonVar::buttonGo.rect.y == buttonVar::buttonIns.rect.y){
            slideProcess = false;
            progress = 0.0f;
        }
        break;
        default:
        break;
    }
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
    if(type != Type::None) DrawButtonAVL(buttonVar::buttonGo.rect, buttonVar::buttonGo.text, buttonVar::buttonGo.buCol, SSLFont,22);
    drawFile();
    drawInit();
    drawButtonAVL();
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    if(inProcess) CreateAnimation();
}
void CreateAVL::handle(){
    if(progress == 0) mAVL->getToggle().transfer = true;
    else mAVL->getToggle().transfer = false;
    handleButtonsHoverAVL();
    if(!fileProcess && !initProcess && !slideProcess && !inProcess) handleButtonsClickAVL(mAVL);
    if ((CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    && !fileProcess && !initProcess && !slideProcess && !inProcess) {
    type = Type::None;
    }   
    if(initProcess) handleInit();
    if(fileProcess) handleFile();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Type detectedMode = DetectCurrentMode();
    if (mousePressed && detectedMode != Type::None) {
        handleModeTransitTion(detectedMode);
    }
    updateCommonAnimation();
    if(type != Type::None){
        int key = GetKeyPressed();
        if((key == KEY_ENTER  || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) && progress == 0.0f && type == Type::Init) initProcess = true;
        if((key == KEY_ENTER || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) && type == Type::File && progress == 0.0f) fileProcess = true;
    }
}
//Insert
InsertAVL::InsertAVL(AVL* AVL) : mAVL(AVL), frameCounter(0), framecntIns(0), textIn(""), progress(0.0f), prev(nullptr), tmp(nullptr), type(RotateType::None),
inProcess(false), insertRoot(false), InsertNodeFlag(false), returnNode(false), runAtOnce(false), undoFlag(false), redoFlag(false), undoRotateFlag(false), deleteUndoFlag(false) {
    inputRect = {buttonVar::buttonIns.rect.x + 115,buttonVar::buttonIns.rect.y, button::sizeW, button::sizeH};
    avlBox = AnnouncementBox(Rectangle{900, 400, 420, 320}, "Insert");
    avlBox.SetContent({
        "insert v",
        "if v exists, return",
        "check balance factor of this and its children",
        "   case1: this.rotateRight",
        "   case2: this.left.rotateLeft, this.rotateRight",
        "   case3: this.rotateLeft",
        "   case4: this.right.rotateRight, this.rotateLeft",
        "   case5: this.balanced",
        "this is balanced"
    });
}   
void InsertAVL::handleUndo(){
    if(!mAVL->getRoot()){
        undoFlag = false;
        return;
    } 
    mAVL->setPause(true);
    framecntIns = 0;
    if(mAVL->getRoot()->data == value && !mAVL->getRoot()->left && !mAVL->getRoot()->right) {
        mAVL->getRoot() = deleteNode(mAVL->getRoot(),value);
        undoFlag = false;
        insertRoot = true;
        avlBox.SetHighlightLines(0,0);
    }
    else if(InsertNodeFlag){
        avlBox.SetHighlightLines(0,0);
        if(prev == mAVL->getRoot()){
            undoFlag = false;
            return;
        }
        if(!path.empty()){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            prev->color = WHITE;
            DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                edge.color = BLACK;
                tmp = prev;
                prev = path.back();
                path.pop_back();
                undoFlag = false;
                progress = 0.0f;
            }
        }
    } 
    else if(returnNode){
        avlBox.SetHighlightLines(1,1);
        if(!undoPath.empty()){
            EdgeAVL& edge = findEdge(prev,undoPath.back(),mAVL->getEdge());
            DrawEdgeColor(prev->pos,undoPath.back()->pos,progress,YELLOW);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                if(prev->data != value) path.push_back(prev);
                edge.color = YELLOW;
                prev = undoPath.back();
                undoPath.pop_back();
                prev->color = YELLOW;
                undoFlag = false;
                progress = 0.0f;
                if(undoPath.empty()){
                    avlBox.SetHighlightLines(0,0);
                    InsertNodeFlag = true;
                    returnNode = false;
                }
            }
        }
        else{
            undoFlag = false;
            InsertNodeFlag = true;
            returnNode = false;
        }
    }
    else if(inProcess && undoPath.empty()){
        avlBox.SetHighlightLines(0,0);
        oldPos.clear(); newPos.clear();
        std::vector<Node*> node = mAVL->BFSOrder();
        for(auto u : node) {
            if(u->data == value) continue;
            oldPos.push_back(u->pos);
        }
        Node* child = (prev->left && prev->left->data == value) ? prev->left : prev->right;
        auto& edges = mAVL->getEdge();  
        auto it = std::find_if(edges.begin(), edges.end(), [&](const EdgeAVL& e) {
            return e.par == prev && e.child == child;
        });
        if (it != edges.end()) {
            edges.erase(it);
        }
        mAVL->getRoot() = deleteNode(mAVL->getRoot(),value);
        newPos = calposAVLTree(mAVL->getRoot());
        undoFlag = false;
        for(int i = 0; i<oldPos.size(); i++){
            if(oldPos[i].x != newPos[i].x)
            deleteUndoFlag = true;
        }
        if(!deleteUndoFlag){
            InsertNodeFlag = true;
            inProcess = false;
        }
    }  
    else if(!inProcess && !InsertNodeFlag && !returnNode){
        if(rotate.empty() || (undoPath.back() != rotate.back().node)){
            prev = undoPath.back();
            undoPath.pop_back();
            prev->color = YELLOW;
            undoFlag = false;
            if(undoPath[0]->data == value) {
                returnNode = true;
                avlBox.SetHighlightLines(1,1);
            }
            else {
                inProcess = true;
                avlBox.SetHighlightLines(2,7);
            }
        }
        else{
            prev = undoPath.back();
            undoPath.pop_back();
            prev->color = YELLOW;
            undoFlag = false;
            undoRotateFlag = true;
            inProcess = true;
        }
    }
    else{
        EdgeAVL& edge = findEdge(prev,undoPath.back(),mAVL->getEdge());
        if(edge.par != nullptr){
            avlBox.SetHighlightLines(2,7);
            DrawEdgeColor(prev->pos,undoPath.back()->pos,progress,YELLOW);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                prev->color = YELLOW;
                path.push_back(prev);
                edge.color = YELLOW;
                prev = undoPath.back();
                prev->color = YELLOW;
                undoPath.pop_back();
                progress = 0.0f;
                undoFlag = false;
            }
        }
        else{
            path.push_back(prev);
            prev = undoPath.back();
            undoPath.pop_back();
            prev->color = YELLOW;
            undoFlag = false;
            undoRotateFlag = true;
        }
    } 
}
void InsertAVL::deleteUndo(){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            InsertNodeFlag = true;
            progress = 0.0f;
            inProcess = false;
            deleteUndoFlag = false;
        }
}
void InsertAVL::undoRotate(){
    if (!rotate.empty() && rotate.back().type == RotateType::LeftLeft && type == RotateType::None && prevType == RotateType::None) {
        avlBox.SetHighlightLines(2,5);
        rotate.pop_back();
        type = RotateType::RightRight;
        Node* par = findParent(mAVL->getRoot(),prev);
        rightRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }  // LL case
    else if (!rotate.empty() && rotate.back().type == RotateType::RightRight && type == RotateType::None && prevType == RotateType::None) {
        avlBox.SetHighlightLines(2,3);
        rotate.pop_back();
        type = RotateType::LeftLeft;
        Node* par = findParent(mAVL->getRoot(),prev);
        leftRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    } // RR case
    else if (!rotate.empty() && rotate.back().type == RotateType::RightLeft && type == RotateType::None && prevType == RotateType::None) {
        avlBox.SetHighlightLines(2,4);
        rotate.pop_back();
        type = RotateType::LeftRight;
        Node* par = findParent(mAVL->getRoot(),prev);
        leftRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }   // LR case
    else if (!rotate.empty() && rotate.back().type == RotateType::LeftRight && type == RotateType::None && prevType == RotateType::None){
        avlBox.SetHighlightLines(2,6);
        rotate.pop_back();
        type = RotateType::RightLeft;
        Node* par = findParent(mAVL->getRoot(),prev);
        rightRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }
    else if (type == RotateType::None && prevType == RotateType::LeftRight) {
        type = RotateType::LeftRight;
        Node* child = prev->left;
        rightRotate(mAVL->getRoot(),child,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }
    else if (type == RotateType::None && prevType == RotateType::RightLeft) {
        type = RotateType::RightLeft;
        Node* child = prev->right;
        leftRotate(mAVL->getRoot(),child,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }
    if(type == RotateType::RightRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None; 
            undoRotateFlag = false;
        }
    }
    else if(type == RotateType::LeftLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            undoRotateFlag = false;
        }
    }
    else if(type == RotateType::RightLeft && prevType == RotateType::None){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::RightLeft;
        }
    }
    else if(type == RotateType::LeftRight && prevType == RotateType::None){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::LeftRight;
        }
    }
    else if(type == RotateType::LeftRight && prevType == RotateType::LeftRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            undoRotateFlag = false;
        }
    }
    else if(type == RotateType::RightLeft && prevType == RotateType::RightLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            undoRotateFlag = false;
        }
    }
}
void InsertAVL::handleRedo(){
    mAVL->setPause(false);
    if(insertRoot)
        InsertRoot(mAVL->getRoot(),value);
    else if(InsertNodeFlag)
        InsertNode(mAVL->getRoot(),value);
    else if(inProcess)
        InsertAnimation(value);
    else if(returnNode)
        returnAnimation();
}
void InsertAVL::InsertRoot(Node*& pRoot, int x){
    pRoot = new Node(x,posRoot);
    newNodeAVL(mAVL,pRoot,progress);
    avlBox.SetHighlightLines(0,0);
    if(progress >= 1.0f){
        insertRoot = false;
        progress = 0.0f;
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntIns = 0;
        }
    }
}
void InsertAVL::InsertNode(Node*& pRoot, int x){
    prev->color = YELLOW;
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
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntIns = 0;
        }
    }
    if(prev->data == x && !mAVL->getPause()){
        returnNode = true;
        InsertNodeFlag = false;
        avlBox.SetHighlightLines(1,1);
        return;
    }
    if(!tmp && !mAVL->getPause()) {
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
        InsertNodeFlag = false;
        inProcess = true;
        for(int i=path.size()-1; i>=0; i--) {
        path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right))+1;
        }
        handlePos(mAVL, oldPos, newPos);
    }
    if(progress == 0.0f && !mAVL->getPause()) framecntIns++;
    if(tmp && framecntIns >= 20) {
        DrawEdgeColor(prev->pos,tmp->pos,progress,YELLOW);
        DrawNodeAVL(prev);
    }
    avlBox.SetHighlightLines(0,0);
}
void InsertAVL::InsertAnimation(int x){
    if(path.empty() && framecntIns >= 22){
        prev->color = WHITE;
        undoPath.push_back(prev);
        inProcess = false;
        framecntIns = 0;
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntIns = 0;
        }
        return;
    }
    if(progress == 0.0f && type == RotateType::None && !mAVL->getPause()) framecntIns++;

    if(prev->data == x){
        newNodeAVL(mAVL,prev,progress);
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
            avlBox.SetHighlightLines(2,7);
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntIns = 0;
            }
        }
    }
    if(framecntIns > 22 && type == RotateType::None){
        if(abs(getHeight(prev)) <= 1) avlBox.SetHighlightLines(2,7);
        prev->color = WHITE;
        if(findEdge(path.back(),prev,mAVL->getEdge()).par != nullptr) DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(findEdge(path.back(),prev,mAVL->getEdge()).par == nullptr){
            if(prevType == RotateType::None) undoPath.push_back(prev);
            framecntIns = 0;
            prev = path.back();
            path.pop_back();
            if(redoFlag && prevType == RotateType::None){
                redoFlag = false;
                mAVL->setPause(true);
                framecntIns = 0;
            }
        }
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            if(prevType == RotateType::None) undoPath.push_back(prev);
            edge.color = BLACK;
            framecntIns = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
            if(redoFlag && prevType == RotateType::None){
                redoFlag = false;
                mAVL->setPause(true);
                framecntIns = 0;
            }
        }
    }
    if (getBalance(prev) > 1 && ((x < prev->left->data && !mAVL->getPause()) || prevType == RotateType::RightLeft) && progress == 0.0f && !mAVL->getPause()) {
        type = RotateType::RightRight;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
        if(prevType == RotateType::None){
            UndoState state(prev,RotateType::RightRight);
            rotate.push_back(state);
        }
        else if(prevType == RotateType::RightLeft){
            UndoState state(prev,RotateType::RightLeft);
            rotate.push_back(state);
        }
    }  // LL case
    else if (getBalance(prev) < -1 && ((x > prev->right->data && !mAVL->getPause()) || prevType == RotateType::LeftRight) && progress == 0.0f) {
        type = RotateType::LeftLeft;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
        if(prevType == RotateType::None){
            UndoState state(prev,RotateType::LeftLeft);
            rotate.push_back(state);
        }
        else if(prevType == RotateType::LeftRight){
            UndoState state(prev,RotateType::LeftRight);
            rotate.push_back(state);
        }
    }    // RR case
    else if (getBalance(prev) > 1 && x > prev->left->data && progress == 0.0f && type == RotateType::None && !mAVL->getPause()) {
        type = RotateType::RightLeft;
        path.push_back(prev);
        prev = prev->left;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }   // LR case
    else if (getBalance(prev) < -1 && x < prev->right->data && progress == 0.0f && type == RotateType::None && !mAVL->getPause()){
        type = RotateType::LeftRight;
        path.push_back(prev);
        prev = prev->right;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    };
    if(type == RotateType::RightRight){
        if(prevType == RotateType::None) avlBox.SetHighlightLines(2,3);
        else avlBox.SetHighlightLines(2,4);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None; 
        }
    }
    if(type == RotateType::LeftLeft){
        if(prevType == RotateType::None) avlBox.SetHighlightLines(2,5);
        else avlBox.SetHighlightLines(2,6);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
        }
    }
    if(type == RotateType::RightLeft){
        avlBox.SetHighlightLines(2,4);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::RightLeft;
        }
    }
    if(type == RotateType::LeftRight){
        avlBox.SetHighlightLines(2,6);
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
    if(progress == 0.0f && !mAVL->getPause()) framecntIns++;
    if(path.empty() && framecntIns >= 22){
        prev->color = WHITE;
        undoPath.push_back(prev);
        returnNode = false;
        framecntIns = 0;
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntIns = 0;
        }
        return;
    }
    if(framecntIns > 22){
        prev->color = WHITE;
        DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            edge.color = BLACK;
            undoPath.push_back(prev);
            framecntIns = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
            if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntIns = 0;
            }
        }   
    }
}
void InsertAVL::InsertAtOnce(Node* pRoot, int x){
    if(progress == 0.0f){
        pRoot = insertNode(pRoot,x);
        Node* prevwalk = nullptr;
        Node* walk = pRoot;
        while(walk->data != x) {
            prevwalk = walk;
            walk = (walk->data < x) ? walk->right : walk->left;
        }
        if(prevwalk){
            if(walk == prevwalk->left) walk->pos = {prevwalk->pos.x-50, prevwalk->pos.y+50};
            else walk->pos = {prevwalk->pos.x+50, prevwalk->pos.y+50};
        }
        mAVL->getEdge().clear();
        std::vector<Node*> node = mAVL->BFSOrder();
        oldPos.clear(); newPos.clear();
        for(auto u : node){
            oldPos.push_back(u->pos);
            if(u->left){
                EdgeAVL edge(BLACK,u,u->left);
                mAVL->getEdge().push_back(edge);
            }
            if(u->right){
                EdgeAVL edge(BLACK,u,u->right);
                mAVL->getEdge().push_back(edge);
            }
        }
        newPos = calposAVLTree(pRoot);
    }
    progress += deltaTimeAVL;
    updatePos(pRoot);
    if(progress >= 1.0f){
        progress = 0.0f;
        runAtOnce = false;
    }
}
void InsertAVL::updatePos(Node*& pRoot){
    std::vector<Node*> node = mAVL->BFSOrder();
    for(int i=0; i<node.size(); i++){
        node[i]->pos = lerpAVL(oldPos[i],newPos[i],progress);
    }
}
void InsertAVL::draw(){
    avlBox.DrawForAVL();
    drawButtonAVL();
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextInAVL(textIn, inputRect, frameCounter);
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    DrawButtonAVL(buttonVar::buttonGo.rect, buttonVar::buttonGo.text, buttonVar::buttonGo.buCol, SSLFont,22);
    if(insertRoot && !undoFlag && !redoFlag) InsertRoot(mAVL->getRoot(),value);
    if(InsertNodeFlag && !undoFlag && !redoFlag) InsertNode(mAVL->getRoot(),value);
    if(inProcess && !undoFlag && !deleteUndoFlag && !undoRotateFlag && !redoFlag) InsertAnimation(value);
    if(returnNode && !undoFlag && !redoFlag) returnAnimation();
    if(runAtOnce) InsertAtOnce(mAVL->getRoot(),value);
    if(undoFlag) handleUndo();
    if(redoFlag) handleRedo();
    if(undoRotateFlag) undoRotate();
    if(deleteUndoFlag) deleteUndo();
    if(mAVL->getRoot() && !returnNode && !insertRoot && !InsertNodeFlag && !inProcess && !undoRotateFlag && !redoFlag && !undoFlag && !deleteUndoFlag) avlBox.SetHighlightLines(8,8);
}
void InsertAVL::handle(){
    if(!inProcess && !insertRoot && !InsertNodeFlag && !returnNode && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) mAVL->getToggle().transfer = true;
    else mAVL->getToggle().transfer = false;
    handleButtonsHoverAVL();
    if(!inProcess && !insertRoot && !InsertNodeFlag && !returnNode && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) handleButtonsClickAVL(mAVL);
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    if ((key == KEY_ENTER || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) && !textIn.empty() && mAVL->getToggle().isStepByStep
    && !inProcess && !insertRoot && !InsertNodeFlag && !returnNode && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) {
        value = std::stoi(textIn);
        if(!mAVL->getRoot()) insertRoot = true;
        else {
            prev = mAVL->getRoot();
            tmp = (prev->data < value) ? prev->right : prev->left; 
            InsertNodeFlag = true;
            prevType = RotateType::None;
        }
        textIn.clear();
        mAVL->setPrev(mAVL->getInsert());
        mAVL->setPause(false); 
        path.clear(); undoPath.clear(); rotate.clear();
    }
    if ((key == KEY_ENTER || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) && !textIn.empty() && !mAVL->getToggle().isStepByStep
    && !inProcess && !insertRoot && !InsertNodeFlag && !returnNode && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) {
        value = std::stoi(textIn);
        if(mAVL->getRoot())
        runAtOnce = true;
        else{
            mAVL->getRoot() = new Node(value,posRoot,NodeSizeAVL,FontNodeAVL);
        }
        textIn.clear();
        mAVL->setPause(false);
    }
    if(UndoButton.isPressed() && progress == 0.0f && mAVL->getPrev() == mAVL->getInsert() && mAVL->getToggle().isStepByStep && !undoFlag && !deleteUndoFlag && !undoRotateFlag)
        undoFlag = true;
    if(RedoButton.isPressed() && progress == 0.0f && (insertRoot || InsertNodeFlag || inProcess || returnNode) && mAVL->getToggle().isStepByStep && !redoFlag){
        redoFlag = true;
        framecntIns = 20;
    }
}
//Delete
DeleteAVL::DeleteAVL(AVL* AVL) : mAVL(AVL), frameCounter(0), framecntDel(0), progress(0.0f), type(RotateType::None), prevType(RotateType::None), undoRoot(nullptr),
inProcess(false), returnNode(false), DeleteNodeFlag(false), runAtOnce(false), deleteLeaf(false), firstDelete(false), undoFlag(false), redoFlag(false), undoRotateFlag(false), deleteUndoFlag(false) {
    inputRect = {buttonVar::buttonDel.rect.x + 115,buttonVar::buttonDel.rect.y, button::sizeW, button::sizeH};
    avlBox = AnnouncementBox(Rectangle{900, 400, 420, 320}, "Delete");
    avlBox.SetContent({
        "remove v",
        "if v doesn't exist, return",
        "check balance factor of this and its children",
        "   case1: this.rotateRight",
        "   case2: this.left.rotateLeft, this.rotateRight",
        "   case3: this.rotateLeft",
        "   case4: this.right.rotateRight, this.rotateLeft",
        "   case5: this.balanced",
        "this is balanced"
    });
}
DeleteAVL::~DeleteAVL(){
    clearAVL(undoRoot);
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
    DrawNodeAVL(x);
    DrawNodeAVL(y);
    updatePos(pRoot);
    x->color = lerpAVL(BLUE, theme.getTheme(), progress);
    x->textColor = lerpAVL(BLACK,theme.getTheme(), progress);
    // Finalize deletion when animation completes
    if (progress >= 1.0f) {
        delete x;
        x = path.back();
        path.pop_back();
        progress = 0.0f;
        firstDelete = false;
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntDel = 0;
        }
    }
}
void DeleteAVL::rotateAVL(){
    if(progress == 0.0f && type == RotateType::None && ((prevType == RotateType::None && !mAVL->getPause()) || (prevType != RotateType::None))) framecntDel++;
    if(path.empty() && framecntDel >= 23){
        prev->color = WHITE;
        undoPath.push_back(prev);
        inProcess = false;
        deleteLeaf = false;
        framecntDel = 0;
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntDel = 0;
        }
        return;
    }
    if(framecntDel > 22 && type == RotateType::None){
        avlBox.SetHighlightLines(2,7); 
        prev->color = WHITE;
        if(findEdge(path.back(),prev,mAVL->getEdge()).par != nullptr) DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(findEdge(path.back(),prev,mAVL->getEdge()).par == nullptr || prev == path.back()){
            if(prevType == RotateType::None && prev != path.back()) undoPath.push_back(prev);
            framecntDel = 0;
            prev = path.back();
            path.pop_back();
            if(redoFlag && !path.empty() && prev != path.back()){
                redoFlag = false;
                mAVL->setPause(true);
                framecntDel = 0;
            }
        }
        if(progress >= 1.0f){   
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            if(prevType == RotateType::None) undoPath.push_back(prev);
            edge.color = BLACK;
            framecntDel = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntDel = 0;
            }
        }
    }
    if (getBalance(prev) > 1 && ((getBalance(prev->left) >= 0 && !mAVL->getPause()) || prevType == RotateType::RightLeft) && progress == 0.0f) {
        type = RotateType::RightRight;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
        if(prevType == RotateType::None){
            UndoState state(prev,RotateType::RightRight);
            rotate.push_back(state);
        }
        else if(prevType == RotateType::RightLeft){
            UndoState state(prev,RotateType::RightLeft);
            rotate.push_back(state);
        }
    }  // LL case
    if (getBalance(prev) < -1 && ((getBalance(prev->right) <= 0 && !mAVL->getPause()) || prevType == RotateType::LeftRight) && progress == 0.0f) {
        type = RotateType::LeftLeft;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
        if(prevType == RotateType::None){
            UndoState state(prev,RotateType::LeftLeft);
            rotate.push_back(state);
        }
        else if(prevType == RotateType::LeftRight){
            UndoState state(prev,RotateType::LeftRight);
            rotate.push_back(state);
        }
    }    // RR case
    if (getBalance(prev) > 1 && getBalance(prev->left) < 0 && progress == 0.0f && type == RotateType::None && !mAVL->getPause()) {
        type = RotateType::RightLeft;
        path.push_back(prev);
        prev = prev->left;
        leftRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }   // LR case
    if (getBalance(prev) < -1 && getBalance(prev->right) > 0 && progress == 0.0f && type == RotateType::None && !mAVL->getPause()){
        type = RotateType::LeftRight;
        path.push_back(prev);
        prev = prev->right;
        rightRotate(mAVL->getRoot(),prev,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    };
    if(type == RotateType::RightRight){
        if(prevType == RotateType::None) avlBox.SetHighlightLines(2,3);
        else avlBox.SetHighlightLines(2,4);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None; 
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntDel = 0;
            }
        }
    }
    if(type == RotateType::LeftLeft){
        if(prevType == RotateType::None) avlBox.SetHighlightLines(2,5);
        else avlBox.SetHighlightLines(2,6);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntDel = 0;
            }
        }
    }
    if(type == RotateType::RightLeft){
        avlBox.SetHighlightLines(2,4);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::RightLeft;
        }
    }
    if(type == RotateType::LeftRight){
        avlBox.SetHighlightLines(2,6);
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::LeftRight;
        }
    }
}
void DeleteAVL::replicateTree(Node*& ori, Node*& rep, std::vector<EdgeAVL>& edge){
    clearAVL(rep);
    if (!ori) {
        rep = nullptr;
        return;
    }
    std::queue<Node*> qOri;
    std::queue<Node*> qDu;
    qOri.push(ori);
    rep = new Node(ori->data,ori->pos,ori->radius,ori->font,ori->height,ori->color,ori->textColor);
    qDu.push(rep);
    while (!qOri.empty()) {
        auto uOri = qOri.front(); qOri.pop();
        auto uDu = qDu.front(); qDu.pop();
        if (uOri->left) {
            Node* curTmp = new Node(uOri->left->data,uOri->left->pos,uOri->left->radius,uOri->left->font,uOri->left->height,uOri->left->color,uOri->left->textColor);
            uDu->left = curTmp;
            qOri.push(uOri->left);
            qDu.push(curTmp);
        }
        if (uOri->right) {
            Node* curTmp = new Node(uOri->right->data,uOri->right->pos,uOri->right->radius,uOri->right->font,uOri->right->height,uOri->right->color,uOri->right->textColor);
            uDu->right = curTmp;
            qOri.push(uOri->right);
            qDu.push(curTmp);
        }
    }
    edge.clear();
    qDu.push(rep);
    qOri.push(ori);
    while(!qDu.empty()){
        auto uDu = qDu.front();
        qDu.pop();
        auto uOri = qOri.front();
        qOri.pop();
        if(uDu->left){
            EdgeAVL curEdge(BLACK,uDu,uDu->left);
            EdgeAVL& tmpEdge = findEdge(uOri,uOri->left,mAVL->getEdge());
            if(tmpEdge.par != nullptr) curEdge.color = tmpEdge.color;
            edge.push_back(curEdge);
            qDu.push(uDu->left);
            qOri.push(uOri->left);
        }
        if(uDu->right){
            EdgeAVL curEdge(BLACK,uDu,uDu->right);
            EdgeAVL& tmpEdge = findEdge(uOri,uOri->right,mAVL->getEdge());
            if(tmpEdge.par != nullptr) curEdge.color = tmpEdge.color;
            edge.push_back(curEdge);
            qDu.push(uDu->right);
            qOri.push(uOri->right);
        }
    }
}
void DeleteAVL::handleUndo(){
    mAVL->setPause(true);
    framecntDel = 0;
    if(!deleteLeaf && !inProcess && !returnNode && !DeleteNodeFlag){
        if(rotate.empty() || (undoPath.back() != rotate.back().node)){
            prev = undoPath.back();
            undoPath.pop_back();
            if(prev->data == undoValue) prev->color = BLUE;
            else prev->color = YELLOW;
            undoFlag = false;
            if(!findNode(mAVL->getRoot(),value) && !findNode(undoRoot,value)) {
                returnNode = true;
                avlBox.SetHighlightLines(1,1);
            }
            else if(findNode(undoRoot,value) && !findNode(undoRoot,value)->left && !findNode(undoRoot,value)->right) deleteLeaf = true;
            else {
                inProcess = true; 
                avlBox.SetHighlightLines(2,7);
            }
        }
        else{
            prev = undoPath.back();
            undoPath.pop_back();
            if(prev->data == undoValue) prev->color = BLUE;
            else prev->color = YELLOW;
            undoFlag = false;
            undoRotateFlag  = true;
            if(findNode(undoRoot,value) && !findNode(undoRoot,value)->left && !findNode(undoRoot,value)->right) deleteLeaf = true;
            else inProcess = true; 
        }
    }
    else if(DeleteNodeFlag){
        if(prev == mAVL->getRoot() && prev->data != value){
            undoFlag = false;
            return;
        }
        if(prev == mAVL->getRoot() && prev == tmp){
            undoFlag = false;
            return;
        }
        if(prev->data == value && !prev->left && !prev->right){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            prev->color = WHITE;
            DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                edge.color = BLACK;
                tmp = prev;
                prev = path.back();
                path.pop_back();
                undoFlag = false;
                progress = 0.0f;
            }
        }
        else if(prev->data != value || prev == tmp){
            if(!path.empty()){
                EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
                prev->color = WHITE;
                DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
                DrawNodeAVL(prev);
                if(progress >= 1.0f){
                    edge.color = BLACK;
                    tmp = prev;
                    prev = path.back();
                    path.pop_back();
                    undoFlag = false;
                    progress = 0.0f;
                }
            }
        }
        else{
            EdgeAVL& edge = findEdge(path.back(),tmp,mAVL->getEdge());
            tmp->color = WHITE;
            DrawEdgeColor(tmp->pos,path.back()->pos,progress,BLACK);
            DrawNodeAVL(tmp);
            if(progress >= 1.0f){
                edge.color = BLACK;
                tmp = path.back();
                path.pop_back();
                undoFlag = false;
                progress = 0.0f;
            }
        }
    }
    else if(returnNode){
        avlBox.SetHighlightLines(1,1);
        if(!undoPath.empty()){
            EdgeAVL& edge = findEdge(prev,undoPath.back(),mAVL->getEdge());
            DrawEdgeColor(prev->pos,undoPath.back()->pos,progress,YELLOW);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                if(prev->data != value) path.push_back(prev);
                edge.color = YELLOW;
                prev = undoPath.back();
                undoPath.pop_back();
                prev->color = YELLOW;
                undoFlag = false;
                progress = 0.0f;
                if(undoPath.empty()){
                    avlBox.SetHighlightLines(0,0);
                    DeleteNodeFlag = true;
                    returnNode = false;
                }
            }
        }
        else{
            undoFlag = false;
            DeleteNodeFlag = true;
            returnNode = false;
        }
    }
    else if((inProcess || deleteLeaf) && undoPath.empty()){
        avlBox.SetHighlightLines(0,0);
        oldPos.clear(); newPos.clear();
        std::vector<Node*> node = mAVL->BFSOrder();
        for(auto u : node) {
            oldPos.push_back(u->pos);
        }
        for(auto u : node){
            Node* curTmp = findNode(undoRoot,u->data);
            newPos.push_back(curTmp->pos);
        }
        for(int i = 0; i<oldPos.size(); i++){
            if(oldPos[i].x != newPos[i].x) deleteUndoFlag = true;
        }
        undoFlag = false;
        if(!deleteUndoFlag){
            DeleteNodeFlag = true;
            path.clear();
            mAVL->getEdge() = undoEdge;
            replicateTree(undoRoot,mAVL->getRoot(),undoEdge);
            mAVL->getEdge() = undoEdge;
            if(inProcess){
                tmp = findNode(mAVL->getRoot(),undoValue);
                prev = findNode(mAVL->getRoot(),value);
                Node* walk = mAVL->getRoot();
                while(walk != tmp){
                    path.push_back(walk);
                    walk = (walk->data < undoValue) ? walk->right : walk->left;
                }
            }
            else if(deleteLeaf) {
                prev = findNode(mAVL->getRoot(),value);
                Node* walk = mAVL->getRoot();
                while(walk != prev){
                    path.push_back(walk);
                    walk  = (walk->data < undoValue) ? walk->right : walk->left;
                }
            }
            deleteLeaf = false;
            inProcess = false;
            prev->color = BLUE;
        }
    }
    else if(inProcess){
            EdgeAVL& edge = findEdge(prev,undoPath.back(),mAVL->getEdge());
            if(edge.par != nullptr){
                avlBox.SetHighlightLines(2,7);
                DrawEdgeColor(prev->pos,undoPath.back()->pos,progress,YELLOW);
                DrawNodeAVL(prev);
                if(progress >= 1.0f){
                    path.push_back(prev);
                    edge.color = YELLOW;
                    prev = undoPath.back();
                    if(prev->data == undoValue) prev->color = BLUE;
                    else prev->color = YELLOW;
                    undoPath.pop_back();
                    progress = 0.0f;
                    undoFlag = false;
                }
            }
            else{
                path.push_back(prev);
                prev = undoPath.back();
                undoPath.pop_back();
                if(prev->data == undoValue) prev->color = BLUE;
                else prev->color = YELLOW;
                undoFlag = false;
                undoRotateFlag = true;
            }
    }
    else if(deleteLeaf){
        EdgeAVL& edge = findEdge(prev,undoPath.back(),mAVL->getEdge());
        if(edge.par != nullptr){
            avlBox.SetHighlightLines(2,7);
            DrawEdgeColor(prev->pos,undoPath.back()->pos,progress,YELLOW);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                path.push_back(prev);
                edge.color = YELLOW;
                prev = undoPath.back();
                prev->color = YELLOW;
                undoPath.pop_back();
                progress = 0.0f;
                undoFlag = false;
            }
        }
        else{
            path.push_back(prev);
            prev = undoPath.back();
            undoPath.pop_back();
            prev->color = YELLOW;
            undoFlag = false;
            undoRotateFlag = true;
        }
    }
}
void DeleteAVL::deleteUndo(){
    avlBox.SetHighlightLines(0,0);
    std::vector<Node*> node = mAVL->BFSOrder();
    progress += deltaTimeAVL;
    for(int i = 0; i<oldPos.size(); i++){
        node[i]->pos = lerpAVL(oldPos[i], newPos[i],progress);
    }
    Node* val = findNode(undoRoot,value);
    DrawNodeAVL(val);
    if(progress >= 1.0f){
        deleteUndoFlag = false;
        DeleteNodeFlag = true;
        progress = 0.0f;
        mAVL->getEdge() = undoEdge;
        replicateTree(undoRoot,mAVL->getRoot(),undoEdge);
        mAVL->getEdge() = undoEdge;
        if(inProcess){
            tmp = findNode(mAVL->getRoot(),undoValue);
            prev = findNode(mAVL->getRoot(),value);
            path.clear();
            Node* walk = mAVL->getRoot();
            while(walk != tmp){
                path.push_back(walk);
                walk = (walk->data < undoValue) ? walk->right : walk->left;
            }
        }
        else if(deleteLeaf) {
            prev = findNode(mAVL->getRoot(),value);
            path.clear();
            Node* walk = mAVL->getRoot();
            while(walk != prev){
                path.push_back(walk);
                walk = (walk->data < undoValue) ? walk->right : walk->left;
            }
        }
        deleteLeaf = false;
        inProcess = false;
        prev->color = BLUE;
    }
}
void DeleteAVL::rotateUndo(){
    if (!rotate.empty() && rotate.back().type == RotateType::LeftLeft && type == RotateType::None && prevType == RotateType::None) {
        avlBox.SetHighlightLines(2,5);
        rotate.pop_back();
        type = RotateType::RightRight;
        Node* par = findParent(mAVL->getRoot(),prev);
        rightRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }  // LL case
    else if (!rotate.empty() && rotate.back().type == RotateType::RightRight && type == RotateType::None && prevType == RotateType::None) {
        avlBox.SetHighlightLines(2,3);
        rotate.pop_back();
        type = RotateType::LeftLeft;
        Node* par = findParent(mAVL->getRoot(),prev);
        leftRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--) path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    } // RR case
    else if (!rotate.empty() && rotate.back().type == RotateType::RightLeft && type == RotateType::None && prevType == RotateType::None) {
        avlBox.SetHighlightLines(2,4);
        rotate.pop_back();
        type = RotateType::LeftRight;
        Node* par = findParent(mAVL->getRoot(),prev);
        leftRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }   // LR case
    else if (!rotate.empty() && rotate.back().type == RotateType::LeftRight && type == RotateType::None && prevType == RotateType::None){
        avlBox.SetHighlightLines(2,6);
        rotate.pop_back();
        type = RotateType::RightLeft;
        Node* par = findParent(mAVL->getRoot(),prev);
        rightRotate(mAVL->getRoot(),par,mAVL->getEdge());
        par = findParent(mAVL->getRoot(),prev);
        EdgeAVL& edge = findEdge(par,prev,mAVL->getEdge());
        edge.color = YELLOW;
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }
    else if (type == RotateType::None && prevType == RotateType::LeftRight) {
        type = RotateType::LeftRight;
        Node* child = prev->left;
        rightRotate(mAVL->getRoot(),child,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }
    else if (type == RotateType::None && prevType == RotateType::RightLeft) {
        type = RotateType::RightLeft;
        Node* child = prev->right;
        leftRotate(mAVL->getRoot(),child,mAVL->getEdge());
        for(int i = path.size()-1; i>=0; i--)  path[i]->height = std::max(getHeight(path[i]->left),getHeight(path[i]->right)) + 1;
        handlePos(mAVL,oldPos,newPos);
    }
    if(type == RotateType::RightRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None; 
            undoRotateFlag = false;
        }
    }
    else if(type == RotateType::LeftLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            undoRotateFlag = false;
        }
    }
    else if(type == RotateType::RightLeft && prevType == RotateType::None){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::RightLeft;
        }
    }
    else if(type == RotateType::LeftRight && prevType == RotateType::None){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::LeftRight;
        }
    }
    else if(type == RotateType::LeftRight && prevType == RotateType::LeftRight){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            undoRotateFlag = false;
        }
    }
    else if(type == RotateType::RightLeft && prevType == RotateType::RightLeft){
        progress += deltaTimeAVL;
        updatePos(mAVL->getRoot());
        if(progress >= 1.0f){
            progress = 0.0f;
            type = RotateType::None;
            prevType = RotateType::None;
            undoRotateFlag = false;
        }
    }
}
void DeleteAVL::handleRedo(){
    mAVL->setPause(false);
    if(DeleteNodeFlag) DeleteNode(mAVL->getRoot(),value);
    else if(deleteLeaf) DeleteLeaf(mAVL->getRoot());
    else if(inProcess) DeleteAnimation(mAVL->getRoot(),value);
    else if(returnNode) returnAnimation();
}
void DeleteAVL::DeleteNode(Node*& pRoot, int x){
    if(prev->data != x && prev->data != undoValue) prev->color = YELLOW;
    if(((!prev->left && prev->data > x) || (!prev->right && prev->data < x)) && !mAVL->getPause()){
        returnNode = true;
        DeleteNodeFlag = false;
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
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntDel = 0;
        }
    }
    if(prev->data == x) {
        prev->color = BLUE;
        if(!prev->left && !prev->right && !mAVL->getPause()){
            DeleteNodeFlag = false;
            deleteLeaf = true;
            firstDelete = true;
            undoValue = prev->data;
            replicateTree(mAVL->getRoot(),undoRoot,undoEdge);
        }
        else if(!prev->right && prev->left){
            if(tmp == prev->left && !mAVL->getPause()){
                path.push_back(prev);
                inProcess = true;
                DeleteNodeFlag = false;
                firstDelete = true;
                undoValue = prev->left->data;
                replicateTree(mAVL->getRoot(),undoRoot,undoEdge);
            }
            else {
            if(progress > 0.0f || (progress == 0.0f && !mAVL->getPause())) DrawEdgeColor(prev->pos,prev->left->pos,progress,YELLOW);
            DrawNodeAVL(prev);
            if(progress >= 1.0f){
                path.push_back(prev);
                EdgeAVL& pretmp = findEdge(prev,prev->left,mAVL->getEdge());
                pretmp.color = YELLOW;
                tmp = prev->left;
                tmp->color = BLUE;
                progress = 0.0f;
                if(redoFlag){
                    redoFlag = false;
                    mAVL->setPause(true);
                    framecntDel = 0;
                }
            }
            }
        }
        else{
            if(tmp == prev){
                if(progress == 0.0f && !mAVL->getPause()) framecntDel++;
                if(framecntDel >= 22){  
                DrawEdgeColor(prev->pos,prev->right->pos,progress,YELLOW);
                DrawNodeAVL(prev);
                }
                if(progress >= 1.0f){
                    path.push_back(prev);
                    EdgeAVL& pretmp = findEdge(prev,prev->right,mAVL->getEdge());
                    pretmp.color = YELLOW;
                    tmp = prev->right;
                    if(!tmp->left){
                        progress = 0.0f;
                        tmp->color = BLUE;
                        framecntDel = 0;
                    }
                    else{
                        progress = 0.0f;
                        framecntDel = 0;
                        tmp->color = YELLOW;
                    }
                    if(redoFlag){
                        redoFlag = false;
                        mAVL->setPause(true);
                        framecntDel = 0;
                    }
                }
            }
            else{
                if(tmp->left){
                    if(progress == 0.0f && !mAVL->getPause()) framecntDel++;
                    if(framecntDel >= 22){
                    DrawEdgeColor(tmp->pos,tmp->left->pos,progress,YELLOW);
                    DrawNodeAVL(tmp);
                    }
                    if(progress >= 1.0f){
                        EdgeAVL& pretmp = findEdge(tmp,tmp->left,mAVL->getEdge());
                        pretmp.color = YELLOW;
                        progress = 0.0f; 
                        framecntDel = 0;
                        tmp = tmp->left;
                        if(!tmp->left) tmp->color = BLUE;
                        else tmp->color = YELLOW;
                        if(redoFlag){
                            redoFlag = false;
                            mAVL->setPause(true);
                            framecntDel = 0;
                        }
                    }
                }
                else if(!tmp->left && !mAVL->getPause()){
                    framecntDel = 0;
                    inProcess = true;
                    DeleteNodeFlag = false;
                    firstDelete = true;
                    undoValue = tmp->data;
                    replicateTree(mAVL->getRoot(),undoRoot,undoEdge);
                }
            }
        } 
    }
    if(progress == 0.0f && !mAVL->getPause()) framecntDel++;
    if(prev->data != x && framecntDel >= 20) {
        DrawEdgeColor(prev->pos,tmp->pos,progress,YELLOW);
        DrawNodeAVL(prev);
    }
}
void DeleteAVL::DeleteLeaf(Node*& pRoot) {
    if (firstDelete) {
        // Shrink node radius for deletion animation
        if(progress == 0.0f){
            Node* par = findParent(mAVL->getRoot(),prev);
            if(par->left == prev) par->left = nullptr;
            else par->right = nullptr;
            for (int i = (int)path.size() - 1; i >= 0; --i) {
                path[i]->height = std::max(getHeight(path[i]->left), getHeight(path[i]->right)) + 1;
            }
            handlePos(mAVL,oldPos,newPos);
        }
        progress += deltaTimeAVL;
        if(!path.empty()){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            edge.color = lerpAVL(YELLOW,theme.getTheme(), progress);
        }
        prev->color = lerpAVL(BLUE, theme.getTheme(), progress);
        prev->textColor = lerpAVL(BLACK, theme.getTheme(), progress);
        updatePos(mAVL->getRoot());
        DrawNodeAVL(prev);
        if (progress >= 1.0f) {
            if (!path.empty()) {
                EdgeAVL& prevtmp = findEdge(path.back(), prev, mAVL->getEdge());
                auto it = std::find_if(mAVL->getEdge().begin(), mAVL->getEdge().end(), 
                    [&](const EdgeAVL& e) { return &e == &prevtmp; });
                if (it != mAVL->getEdge().end())  
                    mAVL->getEdge().erase(it);
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
                pRoot = nullptr; 
            } else {
                prev = path.back();
                path.pop_back();
            }
            progress = 0.0f;
            firstDelete = false; 
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntDel = 0;
            }
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
void DeleteAVL::DeleteAtOnce(Node*& pRoot, int x){
    if(progress == 0.0f){
    pRoot = deleteNode(pRoot,x);
    mAVL->getEdge().clear();
    std::vector<Node*> node = mAVL->BFSOrder();
    oldPos.clear(); newPos.clear();
    for(auto u : node){
        if(u->left){
            EdgeAVL edge(BLACK,u,u->left);
            mAVL->getEdge().push_back(edge);
        }
        if(u->right){
            EdgeAVL edge(BLACK,u,u->right);
            mAVL->getEdge().push_back(edge);
        }
        oldPos.push_back(u->pos);
    }
    newPos = calposAVLTree(pRoot);
    }
    progress += deltaTimeAVL;
    updatePos(pRoot);
    if(progress >= 1.0f){
        progress = 0.0f;
        runAtOnce = false;
    }
}
void DeleteAVL::returnAnimation(){
    avlBox.SetHighlightLines(1,1);
    if(progress == 0.0f && !mAVL->getPause()) framecntDel++;
    if(path.empty() && framecntDel >= 22){
        prev->color = WHITE;
        undoPath.push_back(prev);
        inProcess = false;
        returnNode = false;
        framecntDel = 0;
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntDel = 0;
        }
        return;
    }
    if(framecntDel > 22){
        prev->color = WHITE;
        DrawEdgeColor(prev->pos,path.back()->pos,progress,BLACK);
        DrawNodeAVL(prev);
        if(progress >= 1.0f){
            EdgeAVL& edge = findEdge(path.back(),prev,mAVL->getEdge());
            undoPath.push_back(prev);
            edge.color = BLACK;
            framecntDel = 0;
            prev = path.back();
            path.pop_back();
            progress = 0.0f;
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntDel = 0;
            }
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
    avlBox.DrawForAVL();
    drawButtonAVL();
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextInAVL(textIn, inputRect, frameCounter);
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    DrawButtonAVL(buttonVar::buttonGo.rect, buttonVar::buttonGo.text, buttonVar::buttonGo.buCol, SSLFont,22);
    if(DeleteNodeFlag && !undoFlag && !undoRotateFlag && !deleteUndoFlag && !redoFlag) DeleteNode(mAVL->getRoot(),value);
    if(deleteLeaf && !undoFlag && !undoRotateFlag && !deleteUndoFlag && !redoFlag) DeleteLeaf(mAVL->getRoot());
    if(inProcess && !undoFlag && !undoRotateFlag && !deleteUndoFlag && !redoFlag) DeleteAnimation(mAVL->getRoot(),value);
    if(returnNode && !undoFlag && !redoFlag) returnAnimation();
    if(runAtOnce) DeleteAtOnce(mAVL->getRoot(),value);
    if(undoFlag) handleUndo();
    if(deleteUndoFlag) deleteUndo();
    if(undoRotateFlag) rotateUndo();
    if(redoFlag) handleRedo();
    if(mAVL->getRoot() && !inProcess && !DeleteNodeFlag && !returnNode && !deleteLeaf && !firstDelete && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) avlBox.SetHighlightLines(8,8);
}
void DeleteAVL::handle(){
    handleButtonsHoverAVL();
    if(!inProcess && !DeleteNodeFlag && !returnNode && !deleteLeaf && !firstDelete && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) {
        handleButtonsClickAVL(mAVL);
        mAVL->getToggle().transfer = true;
    }
    else mAVL->getToggle().transfer = false;
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    if ((key == KEY_ENTER || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) && !textIn.empty()  && mAVL->getToggle().isStepByStep
    && !inProcess && !DeleteNodeFlag && !returnNode && !deleteLeaf && !firstDelete && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) {
        value = std::stoi(textIn);
        if(mAVL->getRoot()) {
            prev = mAVL->getRoot();
            if(prev->data == value) tmp = prev;
            else{
                tmp = (prev->data < value) ? prev->right : prev->left;
            } 
            DeleteNodeFlag = true;
            prevType = RotateType::None;
            path.clear();
            undoPath.clear();
        }
        textIn.clear();
        mAVL->setPrev(mAVL->getDelete());
        clearAVL(undoRoot);
        avlBox.SetHighlightLines(0,0);
        mAVL->setPause(false);
    }
    if ((key == KEY_ENTER || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) && !textIn.empty()  && !mAVL->getToggle().isStepByStep
    && !inProcess && !DeleteNodeFlag && !returnNode && !deleteLeaf && !firstDelete && !runAtOnce && !undoFlag && !redoFlag && !undoRotateFlag && !deleteUndoFlag) {
        value = std::stoi(textIn);
        if(mAVL->getRoot()) {
            runAtOnce = true;            
            path.clear();
            undoPath.clear();   
        }
        textIn.clear();
        clearAVL(undoRoot);
        mAVL->setPause(false);
    }
    if(UndoButton.isPressed() && progress == 0.0f && mAVL->getPrev() == mAVL->getDelete() && mAVL->getToggle().isStepByStep && !undoFlag && !undoRotateFlag && !deleteUndoFlag){
        undoFlag = true;
    }
    if(RedoButton.isPressed() && progress == 0.0f && mAVL->getPrev() == mAVL->getDelete() && mAVL->getToggle().isStepByStep && (deleteLeaf || DeleteNodeFlag || returnNode || inProcess) && !redoFlag){
        redoFlag = true;
        framecntDel = 20;
    }
}
FindAVL::FindAVL(AVL* AVL) : mAVL(AVL), frameCounter(0), framecntF(0), progress(0), FindNodeFlag(false), undoFlag(false), redoFlag(false), prev(nullptr) {
    inputRect = {buttonVar::buttonF.rect.x + 115,buttonVar::buttonF.rect.y, button::sizeW, button::sizeH};
    avlBox = AnnouncementBox(Rectangle{900, 400, 420, 320}, "Find");
    avlBox.SetContent({
        "if this == null",
        "  return null",
        "else if this key == search value",
        "  return this",    
        "else if this key < search value",
        "  search right",
        "else search left"
    });
}
void FindAVL::FindAnimation(Node*& pRoot, int x){
    prev->color = YELLOW;
    if(prev->data == x && !mAVL->getPause()){
        FindNodeFlag = false;
        prev->color = BLUE;
        avlBox.SetHighlightLines(2,3);
        if(redoFlag){
            redoFlag = false;
            mAVL->setPause(true);
            framecntF = 0;
        }
        return;
    }
    if(prev == mAVL->getRoot()){
        if(tmp == prev->left) avlBox.SetHighlightLines(6,6);
        else avlBox.SetHighlightLines(4,5);
    }
    if(progress >= 1.0f && tmp && !mAVL->getPause()){
        progress = 0.0f;
        framecntF = 0;
        prev->color = WHITE;
        if(tmp == prev->left) avlBox.SetHighlightLines(6,6);
        else avlBox.SetHighlightLines(4,5);
        prev = tmp;
        if(tmp->data < x) tmp = tmp->right;
        else if(tmp->data > x) tmp = tmp->left;
        if(redoFlag){
            prev->color = YELLOW;
            redoFlag = false;
            mAVL->setPause(true);
            framecntF = 0;
            if(prev->data == x){
                FindNodeFlag = false;
                prev->color = BLUE;
                avlBox.SetHighlightLines(2,3);
            }
        }
    }
    if(progress == 0 && !mAVL->getPause()) {
        framecntF++;
        if(!tmp){
            if(value < prev->data) avlBox.SetHighlightLines(6,6);
            else avlBox.SetHighlightLines(4,5);
        }
    }
    if(!tmp && framecntF >= 20){
            framecntF = 0;
            prev->color = WHITE;
            FindNodeFlag = false;
            avlBox.SetHighlightLines(0,1);
            if(redoFlag){
                redoFlag = false;
                mAVL->setPause(true);
                framecntF = 0;
            }
    }
    if(tmp && framecntF >= 20 && tmp != prev) {
        if(tmp == prev->left) avlBox.SetHighlightLines(6,6);
        else avlBox.SetHighlightLines(4,5);
        DrawEdgeColor(prev->pos,tmp->pos,progress,YELLOW);
        DrawNodeAVL(prev);
    }
}
void FindAVL::handleUndo(){
    mAVL->setPause(true);
    framecntF = 0;
    if(prev == mAVL->getRoot()) {
        undoFlag = false;
        return;
    }
    if(FindNodeFlag){
        prev->color = WHITE;
        Node* par = findParent(mAVL->getRoot(),prev);
        if(par->data > value) avlBox.SetHighlightLines(6,6);
        else avlBox.SetHighlightLines(4,5);
        DrawEdgeColor(prev->pos,par->pos,progress,YELLOW);
        DrawNodeAVL(prev);
        if(progress >= 1.0f){
            tmp = prev;
            prev = par;
            prev->color = YELLOW;
            undoFlag = false;
            progress = 0.0f;
        }
    }
    else if(!FindNodeFlag && !isSameColor(prev->color,BLUE) && progress == 0.0f) {
        prev->color = YELLOW;
        FindNodeFlag = true;
        undoFlag = false;
        if(prev->data > value) avlBox.SetHighlightLines(6,6);
        else avlBox.SetHighlightLines(4,5);
    }
    else if(!FindNodeFlag && ((isSameColor(prev->color,BLUE) && progress == 0.0f) || (!isSameColor(prev->color,BLUE) && progress > 0.0f))){
        prev->color = WHITE;
        Node* par = findParent(mAVL->getRoot(),prev);
        if(par->data > value) avlBox.SetHighlightLines(6,6);
        else avlBox.SetHighlightLines(4,5);
        DrawEdgeColor(prev->pos,par->pos,progress,YELLOW);
        DrawNodeAVL(prev);
        if(progress >= 1.0f){
            tmp = prev;
            prev = par;
            prev->color = YELLOW;
            FindNodeFlag = true;
            undoFlag = false;
            progress = 0.0f;
        }
    }
}
void FindAVL::handleRedo(){
    mAVL->setPause(false);
    FindAnimation(mAVL->getRoot(),value);
}
void FindAVL::draw(){
    drawButtonAVL();
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextInAVL(textIn, inputRect, frameCounter);
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    DrawButtonAVL(buttonVar::buttonGo.rect, buttonVar::buttonGo.text, buttonVar::buttonGo.buCol, SSLFont,22);
    if(FindNodeFlag && !undoFlag && !redoFlag) FindAnimation(mAVL->getRoot(),value);
    if(undoFlag) handleUndo();
    if(redoFlag) handleRedo();
    avlBox.DrawForAVL();
}
void FindAVL::handle(){
    handleButtonsHoverAVL();
    if(!FindNodeFlag && !undoFlag && !redoFlag){
        handleButtonsClickAVL(mAVL);
        mAVL->getToggle().transfer = true;
    }
    else mAVL->getToggle().transfer = false;
    if ((CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
    (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    && !FindNodeFlag && !undoFlag && !redoFlag) {
    if (prev) prev->color = WHITE;
    }   
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    if (key == KEY_ENTER && !textIn.empty() || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && mAVL->getToggle().isStepByStep) {
        value = std::stoi(textIn);
        if(mAVL->getRoot()) {
            if(prev) prev->color = WHITE;
            prev = mAVL->getRoot();
            if(prev->data == value) tmp = prev;
            else{
                tmp = (prev->data < value) ? prev->right : prev->left;
            } 
            FindNodeFlag = true;
        }
        textIn.clear();
        mAVL->setPrev(mAVL->getFind());
        mAVL->setPause(false);
    }
    if (key == KEY_ENTER && !textIn.empty() || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !mAVL->getToggle().isStepByStep) {
        value = std::stoi(textIn);
        if(mAVL->getRoot()) {
            if(prev) prev->color = WHITE;
            prev = findNode(mAVL->getRoot(),value);
            if(prev) prev->color = BLUE;
        }
        textIn.clear();
        mAVL->setPause(false);
    }
    if(UndoButton.isPressed() && progress == 0.0f && mAVL->getPrev() == mAVL->getFind())
    undoFlag = true;
    if(RedoButton.isPressed() && progress == 0.0f && mAVL->getPrev() == mAVL->getFind())
    {
        redoFlag = true;
        framecntF = 20;
    }
}
ClearAVL::ClearAVL(AVL* AVL) : mAVL(AVL), progress(0.0f), clearAll(false) {}
void ClearAVL::clearAnimation(){
    progress += deltaTimeAVL;
    std::vector<Node*> node = mAVL->BFSOrder();
    for(auto u : node) {
        u->color = lerpAVL(WHITE,theme.getTheme(), progress);
        u->textColor = lerpAVL(BLACK,theme.getTheme(), progress);
    }
    for(auto& edge : mAVL->getEdge()) edge.color = lerpAVL(BLACK,theme.getTheme(), progress);
    if(progress >= 1.0f){
        progress = 0.0f;
        clearAll = false;
        clearAVL(mAVL->getRoot());
    }
}
void ClearAVL::draw(){
    drawButtonAVL();
    DrawAVLTree(mAVL->getRoot(),mAVL->getEdge());
    DrawButtonAVL(buttonVar::buttonGo.rect, buttonVar::buttonGo.text, buttonVar::buttonGo.buCol, SSLFont,22);
    if(clearAll) clearAnimation();
}
void ClearAVL::handle(){
    handleButtonsHoverAVL();
    if(progress == 0.0f) {
        handleButtonsClickAVL(mAVL);
        mAVL->getToggle().transfer = true;
    }
    else mAVL->getToggle().transfer = false;
    int key = GetKeyPressed();
    if(key == KEY_ENTER || (CheckCollisionPointRec(GetMousePosition(),buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && progress == 0.0f){
        if(mAVL->getRoot()) clearAll = true;
    }
}