#include "LinkedList.h"
#include "GlobalVar.h"
#include <raylib.h>
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "tinyfiledialogs.h"
//--------------------------------
// notInMode
//--------------------------------
void notInMode::draw(){
    drawTextCode(-1,-1);
    drawButtons();
    drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
}
void notInMode::handle(){
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    handleButtonsHover();     
    handleButtonsClick(mSSL);
    if(UndoButton.isPressed()){
        mSSL->handleUndo();
    }
    if(RedoButton.isPressed()){
        mSSL->handleRedo();
    }
}
//Create
Create::Create(SSL *s) : mSSL(s), progress(0), randomProcess(false), fileProcess(false){
    fileInput = {{ buttonVar::buttonCreate.rect.x + 115, buttonVar::buttonCreate.rect.y, 120, (float)button::sizeH }, color::buttonFile, "Load File"};
    random = {{ buttonVar::buttonIns.rect.x + 115, buttonVar::buttonIns.rect.y, 120, (float)button::sizeH }, color::buttonFile, "Random"};
}
void Create::drawInputFile(){
    DrawButton(fileInput.rect, fileInput.text, fileInput.buCol, SSLFont, 18);
}
void Create::drawInitialize(){
    DrawButton(random.rect, random.text, random.buCol, SSLFont, 18);
}
void Create::handleInit(){
    const int ARROW_LENGTH = EArrow.length;
    if (!CheckCollisionPointRec(mouse, random.rect)) {
        random.buCol = color::buttonFile;
        return;
    }
    random.buCol = color::buttonFileHovered;
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;
    mSSL->delAllList();
    pos.clear();
    randomProcess = true;
    progress = 0;  
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
    int nums = GetRandomValue(1, 28);
    pos.resize(nums);
    int val = GetRandomValue(0, 100);
    mSSL->insertTailList(val);
    pos[0].node = mSSL->getRoot();
    pos[0].pos = startLinkedListPos;
    for (int i = 1; i < nums; i++){
        int val = GetRandomValue(0, 100);
        mSSL->insertTailList(val);
        pos[i].node = mSSL->getTail();
        const Vector2& lastPos = pos[i - 1].pos;
        if (lastPos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
            pos[i].pos = { startLinkedListPos.x, lastPos.y + ROW_OFFSET };
        else
            pos[i].pos = { lastPos.x + ARROW_LENGTH + NODE_SIZE, lastPos.y };
    }
    mSSL->setNumElement(pos.size());
}
void Create::handleInputFile() {
    const int ARROW_LENGTH = EArrow.length;
    if (!CheckCollisionPointRec(mouse, fileInput.rect)) {
        fileInput.buCol = color::buttonFile;
        return;
    }
    fileInput.buCol = color::buttonFileHovered;
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;
    if (fileProcess)
        return;
    const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
    if (filePath) {
        mSSL->delAllList();
        pos.clear();
        progress = 0;
        fileProcess = true;
        NodeRadiusRender = 0;
        ArrowLengthRender = 0;
        FontSize = 0;
        mSSL->clearStackUndo();
        mSSL->clearStackRedo();
        std::string pathFile = filePath;
        std::ifstream fin(pathFile);
        if (fin.is_open()) {
            mSSL->fileInput(fin);
            fin.close();
        }
    }
    if (mSSL->getRoot()) {
        handlePos(mSSL->getRoot(), startLinkedListPos,pos);
    }
    mSSL->setNumElement(pos.size());
}
void Create::randomAnimation(){
    float deltaTime = GetFrameTime();
    if (deltaTime > 0.1f) {
        deltaTime = 0.016f; 
    }
    progress += deltaTime;
    NodeRadiusRender = lerp(0, NODE_SIZE, progress);
    FontSize = lerp(0, FontNode, progress);
    drawPos(pos,NodeRadiusRender,FontSize);
    if (progress >= 1.0f) {
        randomProcess = false;
        progress = 0;
    }
}
void Create::fileAnimation() {
    float deltaTime = GetFrameTime();
    if (deltaTime > 0.1f) {
        deltaTime = 0.016f;
    }
    progress += deltaTime / 3.0f;
    NodeRadiusRender = lerp(0, NODE_SIZE, progress);
    FontSize = lerp(0, FontNode, progress);
    drawPos(pos,NodeRadiusRender,FontSize);
    if (progress >= 1.0f) {
        fileProcess = false;
        progress = 0;
    }
}
void Create::draw(){
    drawTextCode(-1,-1);
    drawButtons();
    drawInputFile();
    drawInitialize();
    if (fileProcess) {
        fileAnimation();
    }
    else if (randomProcess) {
        randomAnimation();
    }
    else {
        drawLinkedList(mSSL->getRoot(), startLinkedListPos,mSSL);
    }
}
void Create::handle(){
    handleButtonsHover();
    handleInputFile();
    handleInit();
    if (CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getnotInMode());
        mSSL->setExistVal(mSSL->getRoot());
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getInsert());
        mSSL->setExistVal(mSSL->getRoot());
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getDel());
        mSSL->setExistVal(mSSL->getRoot());
        buttonVar::buttonGo    = {{buttonVar::buttonDel.rect.x+250, buttonVar::buttonDel.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getFind());
        mSSL->setExistVal(mSSL->getRoot());
        buttonVar::buttonGo    = {{buttonVar::buttonF.rect.x+250, buttonVar::buttonF.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getClear());
        mSSL->setExistVal(mSSL->getRoot());
        mSSL->clearStackUndo();
        mSSL->clearStackRedo();
    }
    if(UndoButton.isPressed()){
        mSSL->handleUndo();
    }
    if(RedoButton.isPressed()){
        mSSL->handleRedo();
    }
}
//--------------------------------
// Insert
//--------------------------------
Insert::Insert(SSL* s)
:mSSL(s),InsertTailProcess(false),InsertHeadProcess(false),InsertIdxProcess(false),frameCounter(0),framecntInsert(0),currentAnimatingNode(nullptr),textIn(""),
posTail({1200,50}),progressNode(0), progressArrow(0),textInIndex(""),valInsert(false),idxInsert(false), Font(0)
{
InsertHead = {{ buttonVar::buttonIns.rect.x + 115,buttonVar::buttonIns.rect.y,120, static_cast<float>(button::sizeH)}, color::buttonFile, "Insert Head"};
InsertTail = {{ buttonVar::buttonDel.rect.x + 115,buttonVar::buttonDel.rect.y,120, static_cast<float>(button::sizeH)}, color::buttonFile, "Insert Tail"};
InsertIndex = {{buttonVar::buttonF.rect.x+115,buttonVar::buttonF.rect.y,120,static_cast<float>(button::sizeH)}, color::buttonFile, "Insert Index"};
inputRect = {buttonVar::buttonDel.rect.x + 115,buttonVar::buttonF.rect.y, 120, 0};
Index = {{buttonVar::buttonClear.rect.x+155,buttonVar::buttonClear.rect.y,70,0}, color::buttonFile, "Index"};
Value = {{buttonVar::buttonClear.rect.x+155,buttonVar::buttonClear.rect.y+static_cast<float>(button::sizeH)+10.0f,70,0}, color::buttonFile, "Value"};
valRect = {buttonVar::buttonClear.rect.x+235, Value.rect.y,120,0};
idxRect = {buttonVar::buttonClear.rect.x+235, buttonVar::buttonClear.rect.y,120,0};
}
void Insert::drawHeadInsert(){
    DrawButton(InsertHead.rect,InsertHead.text,InsertHead.buCol,SSLFont,18);
}
void Insert::drawTailInsert(){
    DrawButton(InsertTail.rect,InsertTail.text,InsertTail.buCol,SSLFont,18);
}
void Insert::drawIdxInsert(){
    DrawButton(InsertIndex.rect, InsertIndex.text, InsertIndex.buCol,SSLFont,18); 
}
Insert::InsertType Insert::DetectCurrentMode(){
    if (CheckCollisionPointRec(mouse, InsertHead.rect)) return InsertType::Head;
    if (CheckCollisionPointRec(mouse, InsertTail.rect)) return InsertType::Tail;
    if (CheckCollisionPointRec(mouse, InsertIndex.rect)) return InsertType::Idx;
    return InsertType::None;
}
void Insert::handleModeTransitTion(InsertType newType){
    prev = cur;
    cur = (cur == newType) ? InsertType::None : newType;
    curline = curlinetmp = -1;
    inProcess = true;
    progressAppear = 0.0f;
    inputRect.height = (cur == InsertType::None && prev != InsertType::Idx) ? button::sizeH : 0.0f;
}
void Insert::updateCommonAnimation(){
    if(!inProcess) return;
    progressAppear += GetFrameTime();
    switch (cur)
    {
    case InsertType::Head:
        textIn.clear();
        textInIndex.clear();
        handleHeadMode();
        break;
    case InsertType::Tail:
        textIn.clear();
        textInIndex.clear();
        handleTailMode();
        break;
    case InsertType::Idx:
        textIn.clear();
        textInIndex.clear();
        handleIdxMode();
        break;
    case InsertType::None:
        textIn.clear();
        textInIndex.clear();
        handleNotMode();
        break;
    default:
        break;
    }
    if(progressAppear >= 1.0f){
        inProcess = false;
        progressAppear = 0.0f;
    }
}
void Insert::handleNotMode(){
    InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonDel.rect.y,progressAppear);
    inputRect.height = lerp(inputRect.height,0,progressAppear);
    InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonF.rect.y,progressAppear);
    valRect.height = lerp(valRect.height,0,progressAppear);
    idxRect.height = lerp(idxRect.height,0,progressAppear);
    Index.rect.height = lerp(Index.rect.height,0,progressAppear);
    Value.rect.height = lerp(Value.rect.height,0,progressAppear);
    Font = lerp(Font,0,progressAppear);
}
void Insert::handleHeadMode(){
    inputRect.y = buttonVar::buttonDel.rect.y;
    inputRect.height = lerp(inputRect.height, button::sizeH,progressAppear);
    switch (prev)
    {
    case InsertType::None:
        InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonF.rect.y,progressAppear);
        InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonClear.rect.y,progressAppear);
        buttonVar::buttonGo.rect.y = buttonVar::buttonIns.rect.y;
        break;
    case InsertType::Tail:
        InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonF.rect.y,progressAppear);
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonIns.rect.y,progressAppear);
        break;
    case InsertType::Idx:
        InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonF.rect.y,progressAppear);
        InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonClear.rect.y,progressAppear);
        valRect.height = lerp(valRect.height,0,progressAppear);
        idxRect.height = lerp(idxRect.height,0,progressAppear);
        Index.rect.height = lerp(Index.rect.height,0,progressAppear);
        Value.rect.height = lerp(Value.rect.height,0,progressAppear);
        Font = lerp(Font,0,progressAppear);
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonIns.rect.y,progressAppear);
        break;
    default:
        break;
    }
}
void Insert::handleTailMode(){
    switch (prev)
    {
    case InsertType::None:
        inputRect.y = buttonVar::buttonF.rect.y;
        inputRect.height = lerp(inputRect.height,button::sizeH,progressAppear);
        InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonClear.rect.y,progressAppear);
        buttonVar::buttonGo.rect.y = buttonVar::buttonDel.rect.y;
        break;
    case InsertType::Head:{
        InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonDel.rect.y,progressAppear);
        inputRect.height = lerp(inputRect.height,button::sizeH,progressAppear);
        inputRect.y = buttonVar::buttonF.rect.y+button::sizeH-inputRect.height;
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonDel.rect.y,progressAppear);
        break;
    }
    case InsertType::Idx:{
        inputRect.y = buttonVar::buttonF.rect.y;
        inputRect.height = lerp(inputRect.height,button::sizeH,progressAppear);
        InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonClear.rect.y,progressAppear);
        valRect.height = lerp(valRect.height,0,progressAppear);
        idxRect.height = lerp(idxRect.height,0,progressAppear);
        Index.rect.height = lerp(Index.rect.height,0,progressAppear);
        Value.rect.height = lerp(Value.rect.height,0,progressAppear);
        Font = lerp(Font,0,progressAppear);
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonDel.rect.y,progressAppear);
        break;
    }
    default:
        break;
    }
}
void Insert::handleIdxMode(){
    inputRect.height = lerp(inputRect.height,0,progressAppear);
    valRect.height = lerp(valRect.height,button::sizeH,progressAppear);
    idxRect.height = lerp(idxRect.height,button::sizeH,progressAppear);
    Index.rect.height = lerp(Index.rect.height,button::sizeH,progressAppear);
    Value.rect.height = lerp(Value.rect.height,button::sizeH,progressAppear);
    Font = lerp(Font,20,progressAppear);
    switch (prev)
    {
    case InsertType::Head:
    case InsertType::Tail:
        InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonDel.rect.y,progressAppear);
        InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonF.rect.y,progressAppear);
        Index.rect.y = buttonVar::buttonClear.rect.y+button::sizeH-Index.rect.height;
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonF.rect.y,progressAppear);
        break;
    case InsertType::None:
        Index.rect.y = buttonVar::buttonClear.rect.y;
        buttonVar::buttonGo.rect.y = buttonVar::buttonF.rect.y;
        break;
    default:
        break;
    }
}
void Insert::handleHeadCode(){
    for(int i=0; i<codeHead.size(); i++){
        code[i].text = codeHead[i];
    }
    if(InsertHeadProcess && progressNode < 1.0f) curline = 0;
    else if(InsertHeadProcess && progressArrow < 1.0f) curline = 1;
    if(curline == 1 && progressArrow >= 1.0f-GetFrameTime() && progressNode >= 1.0f-GetFrameTime()) curline = 2;
}
void Insert::handleTailCode(){
    for(int i=0; i<codeTail.size(); i++){
        code[i].text = codeTail[i];
    }
}
void Insert::handleIdxCode(){
    if(modeCur == InsertType::Head){
        handleHeadCode();
    }
    else if(modeCur == InsertType::Tail){
        handleTailCode();
    }
    else{
    for(int i=0; i<codeIdx.size(); i++){
        code[i].text = codeIdx[i];
    }
    }
}
void Insert::handleCodeLine(){
    switch (cur)
    {
    case InsertType::Head:
    handleHeadCode();
    break;
    case InsertType::Tail:
    handleTailCode();
    break;
    case InsertType::Idx:
    handleIdxCode();
    default:
        break;
    }
}
void Insert::insertHeadAnimation(int x){
    std::vector<ShadedData> pos;
    handlePos(mSSL->getRoot(), startLinkedListPos,pos);
    if(progressNode >= 1.0f){
        if(!mSSL->getPause())progressArrow += GetFrameTime();
        ArrowLengthRender = lerp(NODE_SIZE,EArrow.length+NODE_SIZE,progressArrow);
        drawArrow2Node(startLinkedListPos, {startLinkedListPos.x+ArrowLengthRender,startLinkedListPos.y},color::edgeRendered);
    }
    amplifyNode(NodeRadiusRender,FontSize,startLinkedListPos,x,progressNode,mSSL);
    movesPos(pos,progressNode);
    drawTextUp("head",20,pos[0].pos);
    if(progressNode >= 1.0f && progressArrow >= 1.0f){
        InsertHeadProcess = false;
        progressNode = 0;
        progressArrow = 0;
        mSSL->insertHeadList(x);
        mSSL->setNumElement(mSSL->getNumElement()+1);
        InsertIdxProcess = false;
        ListNode* cur = mSSL->getRoot();
    }
}
void Insert::insertTailAnimation(ListNode*& tmp, Vector2& posTail) {
    std::string tailStr = std::to_string(mSSL->getTail()->data);
    drawNode(posTail, std::to_string(mSSL->getTail()->data), NODE_SIZE,color::nodeNotInMode);
    drawTextUp("vtx",20,posTail);
    if(tmp == mSSL->getRoot()) drawTextDown("pointer",22,startLinkedListPos);
    drawPartofLinkedListNotColor(mSSL->getRoot(), mSSL->getTail(), mSSL);
    drawPartofLinkedList(mSSL->getRoot(), tmp, mSSL);
    if(!tmp->next) {
        if(curline!=0 && framecntInsert>=30) curline = 5;
        Vector2 startPos;
        int startPosVal;
        Vector2 endPos = posTail;
        if(mSSL->getRoot()->next == tmp) {
            startPos = startLinkedListPos;
            startPosVal = mSSL->getRoot()->data;
        }
        else {
            startPos = mSSL->getPosEnd();
            startPosVal = mSSL->getprevTail()->data;
        }
        drawArrow2Node(startPos, endPos, color::edgeNotInMode);
        drawNode(posTail, tailStr, NODE_SIZE,color::nodeNotInMode);
        std::string startStr = std::to_string(startPosVal);
        drawNode(startPos, startStr, NODE_SIZE, color::nodeRendered);
        framecntInsert++;
        Vector2 targetPos = { startPos.x + EArrow.length + NODE_SIZE, startPos.y };
        if(targetPos.x > GetScreenWidth() - SCREEN_MARGIN) {
            targetPos = { startLinkedListPos.x, mSSL->getPosEnd().y + ROW_OFFSET };
        }
        if(!mSSL->getPause()) {
            progressNode += (GetFrameTime()/duration);
        }
        posTail = lerp({1200,50},targetPos,progressNode);
        if(fabs(posTail.x - targetPos.x) < 10 && fabs(posTail.y - targetPos.y) < 10) {
            InsertTailProcess = false;
            posTail = { 1200, 50 };  
            progressNode = 0;
            InsertIdxProcess = false;
            framecntInsert = 0;
        }
    }
    else {
        if(!mSSL->getPause())framecntInsert++;
        if(mSSL->getRoot() && mSSL->getRoot()->next == mSSL->getTail()) curline = 0;
        else if(tmp == mSSL->getRoot()){
            curline = 1;
            curlinetmp = 2;
        }
        else if(framecntInsert >= 30){
            curline = 3;
            curlinetmp = -1;
        }
        if(framecntInsert >= 60 ) {
            framecntInsert = 0;
            tmp = tmp->next;
        if(tmp != mSSL->getRoot()->next){
            curline = 4;
            curlinetmp = -1;}
        }
    }
} 
void Insert::insertIdxAnimation(ListNode*& tmp){
    if(curindex == idx){
        std::vector<ShadedData> movePos;
        handlePos(tmp,pos,movePos);
        if(progressNode>=1.0f){
            curline = 5;
            curlinetmp = 6;
            if(!mSSL->getPause())progressArrow+=GetFrameTime();
            Vector2 direct = Vector2Subtract(pos,prevpos);
            float length = Vector2Length(direct);
            float sin = direct.y/length, cos = direct.x/length;
            Vector2 endPos = { pos.x - (NODE_SIZE * cos),pos.y - (NODE_SIZE * sin) };
            float ArrowOri = Vector2Length(Vector2Subtract(prevpos,endPos));
            ArrowLengthRender = lerp(0,ArrowOri,progressArrow);
            drawArrow(prevpos,{prevpos.x+ArrowLengthRender*cos,prevpos.y+ArrowLengthRender*sin},color::edgeRendered);
            Vector2 targetPos;
            if(movePos.size()>1) targetPos = movePos[1].pos;
            else{
                if(pos.x+EArrow.length+NODE_SIZE>GetScreenWidth()-SCREEN_MARGIN) targetPos = {startLinkedListPos.x,pos.y+ROW_OFFSET};
                else targetPos = {pos.x+EArrow.length+NODE_SIZE,pos.y};
            }
            direct = Vector2Subtract(targetPos,pos);
            length = Vector2Length(direct);
            sin = direct.y/length, cos = direct.x/length;
            endPos = {targetPos.x - (NODE_SIZE * cos),targetPos.y - (NODE_SIZE * sin) };
            ArrowOri = Vector2Length(Vector2Subtract(pos,endPos));
            ArrowLengthRender = lerp(0,ArrowOri,progressArrow);
            drawArrow(pos,{pos.x+ArrowLengthRender*cos,pos.y+ArrowLengthRender*sin},color::edgeRendered);
        }
        drawPartofLinkedList(mSSL->getRoot(),tmp,mSSL);   
        amplifyNode(NodeRadiusRender,FontSize,pos,nums,progressNode, mSSL);
        if(progressNode<1.0f){
            curline = 3;
            curlinetmp = 4;
        }
        movesPos(movePos,progressNode);
        drawTextDown("aft",22,movePos[0].pos);
        if(progressNode >= 1.0f && progressArrow >= 1.0f){
            InsertIdxProcess = false;
            progressNode = progressArrow = framecntInsert = 0;
            pos = startLinkedListPos;
            curindex = 0;
            mSSL->insertIdxList(idx,nums);
            mSSL->setNumElement(mSSL->getNumElement()+1);
        }
    }
    else{
        drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
        drawPartofLinkedList(mSSL->getRoot(),tmp,mSSL);
        if(tmp == mSSL->getRoot()) {
            drawTextDown("pointer",22,startLinkedListPos);
            curline = 0;
        }
        else if(framecntInsert>=30){
            curline = 1;
        }
        if(!mSSL->getPause()) framecntInsert++;
        if(framecntInsert >= 60 ) {
            curindex++;
            prevpos = pos;
            nodeNext(tmp,pos,framecntInsert);
            if(tmp != mSSL->getRoot()->next){
                curline = 2;
            }
        }
    }
}  
void Insert::draw() {
    drawButtons();
    drawTailInsert();
    drawHeadInsert();
    drawIdxInsert();
    if(mSSL->getToggle().isStepByStep) drawTextCode(curline,curlinetmp);
    else drawTextCode(-1,-1);
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    if(cur == InsertType::Head || cur == InsertType::Tail) {
        drawTextIn(textIn, inputRect, frameCounter);
        if(mSSL->getNumElement() == 29){
            std::string str = "Linked List reaches maximum elements";
            DrawTextEx(SSLFont,str.c_str(),{inputRect.x+125.0f,inputRect.y+6.0f},21,1,RED);
        }  
    }
    else if(cur == InsertType::Idx){
        if(mSSL->getNumElement() == 29){
            std::string str = "Linked List reaches maximum elements";
            DrawTextEx(SSLFont,str.c_str(),{idxRect.x+125.0f,idxRect.y+6.0f},21,1,RED);
        } 
    }
    if(cur!=InsertType::None) DrawButton(buttonVar::buttonGo.rect,buttonVar::buttonGo.text,buttonVar::buttonGo.buCol,SSLFont,22);
    if(Index.rect.height > 2.0f && Value.rect.height > 2.0f) {
        DrawButton(Index.rect,Index.text,Index.buCol,SSLFont,Font);
        DrawButton(Value.rect,Value.text,Value.buCol,SSLFont,Font);
        DrawRectangleRounded(idxRect, 0.3f, 30, WHITE);
        DrawRectangleRounded(valRect, 0.3f, 30, WHITE);
    }
    if(cur == InsertType::Idx && idxInsert){
            drawTextInwithoutLine(textIn,valRect);
            drawTextIn(textInIndex,idxRect,frameCounter);
        }
    else if(cur == InsertType::Idx && valInsert){
            drawTextIn(textIn,valRect,frameCounter);
            drawTextInwithoutLine(textInIndex,idxRect);
        }
    if(InsertTailProcess) {
        insertTailAnimation(currentAnimatingNode, posTail);
    }
    else if(InsertHeadProcess){
        insertHeadAnimation(nums);
    }
    else if(InsertIdxProcess && idx < mSSL->getNumElement()){
        insertIdxAnimation(currentAnimatingNode);
    }
    else if(idx > mSSL->getNumElement() && cur == InsertType::Idx && mSSL->getNumElement()<29){
        std::string str = "Please enter a valid index between [1," + std::to_string(mSSL->getNumElement()+1)+"]";
            DrawTextEx(SSLFont,str.c_str(),{Index.rect.x+200,Index.rect.y+8.0f},21,1,RED);
            drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    }
    else {
        drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    }
}
void Insert::ResetInsertState() {
    textIn.clear();
    textInIndex.clear();
    InsertTailProcess = InsertIdxProcess = InsertHeadProcess = false;
    currentAnimatingNode = nullptr;
    posTail = {1200, 50};
    mSSL->setExistVal(mSSL->getRoot());
    curline = curlinetmp = -1;
    cur = InsertType::None;
    inputRect.height = Index.rect.height = Value.rect.height = idxRect.height = valRect.height = 0;
    InsertTail.rect.y = buttonVar::buttonDel.rect.y;
    InsertIndex.rect.y = buttonVar::buttonF.rect.y;
}
void Insert::handle(){
    IState::ToggleSwitch toggle = mSSL->getToggle();
    if(toggle.isStepByStep) handleCodeLine();
    handleButtonsHover();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    InsertType detectedMode = DetectCurrentMode();
    if (mousePressed && detectedMode != InsertType::None) {
        handleModeTransitTion(detectedMode);
    }
    updateCommonAnimation();
    if(CheckCollisionPointRec(mouse, InsertHead.rect)){
        InsertHead.buCol = color::buttonFileHovered;
    }
    else if(CheckCollisionPointRec(mouse, InsertTail.rect)){
        InsertTail.buCol = color::buttonFileHovered;
    }
    else if(CheckCollisionPointRec(mouse, InsertIndex.rect)){
        InsertIndex.buCol = color::buttonFileHovered;
    }
    else{
        InsertHead.buCol = color::buttonFile;
        InsertTail.buCol = color::buttonFile;
        InsertIndex.buCol = color::buttonFile;
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && mousePressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
        mSSL->setState(mSSL->getCreate());
        ResetInsertState();
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && mousePressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
        mSSL->setState(mSSL->getnotInMode());
        ResetInsertState();
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && mousePressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
        mSSL->setState(mSSL->getDel());
        ResetInsertState();
        buttonVar::buttonGo    = {{buttonVar::buttonDel.rect.x+250, buttonVar::buttonDel.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && mousePressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
        mSSL->setState(mSSL->getFind());
        ResetInsertState();
        buttonVar::buttonGo    = {{buttonVar::buttonF.rect.x+250, buttonVar::buttonF.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && mousePressed) {
        mSSL->setState(mSSL->getClear());
        ResetInsertState();
        mSSL->clearStackUndo();
        mSSL->clearStackRedo();
    }
SSL::command command;
if (cur == InsertType::Head || cur == InsertType::Tail) {
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    bool goPressed = (key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed);
    if (!textIn.empty() && goPressed && !InsertHeadProcess && !InsertTailProcess && mSSL->getNumElement()<=28) {
        if (cur == InsertType::Tail && toggle.isStepByStep) {
            if (mSSL->getRoot()) {
                InsertTailProcess = true;
                currentAnimatingNode = mSSL->getRoot();
                curline = curlinetmp = -1;
            }
            mSSL->insertTailList(std::stoi(textIn));
            mSSL->setNumElement(mSSL->getNumElement() + 1);
            mSSL->clearStackRedo();
            command = {InsertType::Tail,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if(cur == InsertType::Tail && !toggle.isStepByStep){
            if (mSSL->getRoot()) InsertTailProcess = true;
            mSSL->insertTailList(std::stoi(textIn));
            currentAnimatingNode = mSSL->getTail();
            mSSL->setNumElement(mSSL->getNumElement() + 1);
            mSSL->clearStackRedo();
            command = {InsertType::Tail,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if (cur == InsertType::Head) {
            if (mSSL->getRoot()) {
                InsertHeadProcess = true;
                nums = std::stoi(textIn);
                curline = curlinetmp = -1;
                mSSL->clearStackRedo();
                command = {InsertType::Head,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                mSSL->pushStack(mSSL->getCommandUndo(),command);
            } 
            else {
                mSSL->insertHeadList(std::stoi(textIn));
                mSSL->setNumElement(mSSL->getNumElement() + 1);
                mSSL->clearStackRedo();
                command = {InsertType::Head,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                mSSL->pushStack(mSSL->getCommandUndo(),command);
            }
        }
        textIn.clear();
    }
}
else if (cur == InsertType::Idx) {
    if (CheckCollisionPointRec(mouse, idxRect) && mousePressed) {
        idxInsert = true;
        valInsert = false;
    }
    else if (CheckCollisionPointRec(mouse, valRect) && mousePressed) {
        idxInsert = false;
        valInsert = true;
    }
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9') {
        if (valInsert)
            textIn.push_back((char)key);
        else if (idxInsert)
            textInIndex.push_back((char)key);
    }
    if (key == KEY_BACKSPACE) {
        if (valInsert && !textIn.empty())
            textIn.pop_back();
        else if (idxInsert && !textInIndex.empty())
            textInIndex.pop_back();
    }
    bool goPressed = (key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
    if (!textIn.empty() && !textInIndex.empty() && goPressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess && mSSL->getNumElement()<=28) {
        int idxInput = std::stoi(textInIndex) - 1;
        if (idxInput != mSSL->getNumElement() && idxInput != 0 && idxInput < mSSL->getNumElement() && toggle.isStepByStep) {
            nums = std::stoi(textIn);
            idx = idxInput;
            textIn.clear();
            textInIndex.clear();
            InsertIdxProcess = true;
            currentAnimatingNode = mSSL->getRoot();
            modeCur = InsertType::Idx;
            curline = curlinetmp = -1;
            mSSL->clearStackRedo();
            command = {InsertType::Idx,InsertType::Idx,DeleteType::None,DeleteType::None,idx,nums};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            pos = startLinkedListPos;
        }
        else if (idxInput != mSSL->getNumElement() && idxInput != 0 && idxInput < mSSL->getNumElement() && !toggle.isStepByStep){
            nums = std::stoi(textIn);
            idx = idxInput;
            textIn.clear();
            textInIndex.clear();
            InsertIdxProcess = true;
            currentAnimatingNode = mSSL->getRoot();
            modeCur = InsertType::Idx;
            curline = curlinetmp = -1;
            pos = startLinkedListPos;
            while(curindex<idx){
                prevpos = pos;
                curindex++;
                const int ARROW_LENGTH = EArrow.length;
                currentAnimatingNode = currentAnimatingNode->next;
                if (pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN)
                pos = { startLinkedListPos.x, pos.y + ROW_OFFSET };
                else
                pos.x += ARROW_LENGTH + NODE_SIZE;
            }
            mSSL->clearStackRedo();
            command = {InsertType::Idx,InsertType::Idx,DeleteType::None,DeleteType::None,idx,nums};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if(idxInput > mSSL->getNumElement()){
            nums = std::stoi(textIn);
            idx = idxInput;
            modeCur = InsertType::Idx;
            curline = curlinetmp = -1;
        }
        else {
            if (idxInput == mSSL->getNumElement() && toggle.isStepByStep) {
                if (mSSL->getRoot()) {
                    InsertTailProcess = true;
                    currentAnimatingNode = mSSL->getRoot();
                    modeCur = InsertType::Tail;
                    curline = curlinetmp = -1;
                }
                mSSL->insertTailList(std::stoi(textIn));
                mSSL->setNumElement(mSSL->getNumElement() + 1);
                mSSL->clearStackRedo();
                command = {InsertType::Tail,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                mSSL->pushStack(mSSL->getCommandUndo(),command);
                idx = idxInput;
            }
                else if(idxInput == mSSL->getNumElement() && !toggle.isStepByStep){
                    if (mSSL->getRoot()) InsertTailProcess = true;
                    mSSL->insertTailList(std::stoi(textIn));
                    currentAnimatingNode = mSSL->getTail();
                    mSSL->setNumElement(mSSL->getNumElement() + 1);
                    mSSL->clearStackRedo();
                    command = {InsertType::Tail,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                    mSSL->pushStack(mSSL->getCommandUndo(),command);
                    idx = idxInput;
                }
            else if(idxInput == 0){
                if (mSSL->getRoot()) {
                    InsertHeadProcess = true;
                    nums = std::stoi(textIn);
                    modeCur = InsertType::Head;
                    curline = curlinetmp = -1;
                    mSSL->clearStackRedo();
                    command = {InsertType::Head,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                    mSSL->pushStack(mSSL->getCommandUndo(),command);
                    idx = idxInput;
                }
                else {
                    mSSL->insertHeadList(std::stoi(textIn));
                    mSSL->setNumElement(mSSL->getNumElement() + 1);
                    mSSL->clearStackRedo();
                    command = {InsertType::Head,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                    mSSL->pushStack(mSSL->getCommandUndo(),command);
                    idx = idxInput;
                }
            }
            textIn.clear();
            textInIndex.clear();
        }
    }
    } 
    if(UndoButton.isPressed() && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) mSSL->handleUndo();
    if(RedoButton.isPressed() && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) mSSL->handleRedo();
}
// Delete
Delete::Delete(SSL* s)
: mSSL(s), frameCounter(0), textIn(""), existVal(true), currentAnimatingNode(nullptr), progressNode(0), progressArrow(0),FontSize(23),DeleteValProcess(false),
DeleteHeadProcess(false), DeleteTailProcess(false), framecntDel(0), NodeRadiusRender(33), ArrowLengthRender(EArrow.length), 
shadedPos({startLinkedListPos}),progressAppear(0)
{
    DeleteHead = {{buttonVar::buttonDel.rect.x+115,buttonVar::buttonDel.rect.y,120,button::sizeH},color::buttonFile, "Delete Head"};
    DeleteTail = {{buttonVar::buttonF.rect.x+115,buttonVar::buttonF.rect.y,120,button::sizeH},color::buttonFile, "Delete Tail"};
    DeleteVal = {{buttonVar::buttonClear.rect.x+115,buttonVar::buttonClear.rect.y,120,button::sizeH},color::buttonFile, "Delete Val"};
    inputRect = { buttonVar::buttonClear.rect.x + 115, buttonVar::buttonClear.rect.y+button::sizeH+10.0f, 120, 0 };
}
Delete::DeleteType Delete::DetectCurrentMode(){
    if (CheckCollisionPointRec(mouse, DeleteHead.rect)) return DeleteType::Head;
    if (CheckCollisionPointRec(mouse, DeleteTail.rect)) return DeleteType::Tail;
    if (CheckCollisionPointRec(mouse, DeleteVal.rect)) return DeleteType::Val;
    return DeleteType::None;
}
void Delete::handleHeadCode(){
    for(int i=0; i<delHeadCode.size(); i++){
        code[i].text = delHeadCode[i];
    }
    if(DeleteHeadProcess && mSSL->getRoot() && !mSSL->getRoot()->next){
        curline = 1;
        curlinetmp = 2;
        return;
    }
    if(!mSSL->getRoot()){
        curline = 0;
        curlinetmp = -1;
        return;
    }
    if(DeleteHeadProcess && progressArrow < 0.3f) curline = 3;
    else if(DeleteHeadProcess && progressArrow < 1.0f) curline = 4;
    if(curline == 4 && progressArrow >= 1.0f-GetFrameTime()) curline = 5;
}
void Delete::handleTailCode(){
    for(int i=0; i<delTailCode.size(); i++) code[i].text = delTailCode[i];
    if(!mSSL->getRoot()){
        curline = 0;
        return;
    }
}
void Delete::handleValCode(){
    if(modeCur == DeleteType::Head) handleHeadCode();
    else if(modeCur == DeleteType::Tail) handleTailCode();
    else{
        for(int i=0; i<delValCode.size(); i++) code[i].text = delValCode[i];
        if(!mSSL->getRoot()){
            curline = 0;
            return;
        }
    }
}
void Delete::handleModeTransitTion(DeleteType newType){
    prev = cur;
    cur = (cur == newType) ? DeleteType::None : newType;
    curline = -1;
    curlinetmp = -1;
    inProcess = true;
    progressAppear = 0.0f;
    inputRect.height = (prev == DeleteType::Val) ? button::sizeH : 0.0f;
}
void Delete::updateCommonAnimation(){
    if(!inProcess) return;
    progressAppear += GetFrameTime();
    switch (cur)
    {
    case DeleteType::Head:
        textIn.clear();
        handleHeadMode();
        break;
    case DeleteType::Tail:
        textIn.clear();
        handleTailMode();
        break;
    case DeleteType::Val:
        textIn.clear();
        handleValMode();
        break;
    case DeleteType::None:
        textIn.clear();
        handleNotMode();
        break;
    default:
        break;
    }
    if(progressAppear >= 1.0f){
        inProcess = false;
        progressAppear = 0.0f;
    }
}
void Delete::handleNotMode(){
    inputRect.height = lerp(inputRect.height,0,progressAppear);
}
void Delete::handleHeadMode(){
    switch (prev)
    {
    case DeleteType::None:
        buttonVar::buttonGo.rect.y = DeleteHead.rect.y;
        break;
    case DeleteType::Tail:
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,DeleteHead.rect.y,progressAppear);
        break;
    case DeleteType::Val:
        inputRect.height = lerp(inputRect.height,0,progressAppear);
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,DeleteHead.rect.y,progressAppear);
        break;
    default:
        break;
    }
}
void Delete::handleTailMode(){
    switch (prev)
    {
    case DeleteType::None:
        buttonVar::buttonGo.rect.y = DeleteTail.rect.y;
        break;
    case DeleteType::Head:{
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,DeleteTail.rect.y,progressAppear);
        break;
    }
    case DeleteType::Val:{
        inputRect.height = lerp(inputRect.height,0,progressAppear);
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,DeleteTail.rect.y,progressAppear);
        break;
    }
    default:
        break;
    }
}
void Delete::handleValMode(){
    inputRect.height = lerp(inputRect.height,button::sizeH,progressAppear);
    switch (prev)
    {
    case DeleteType::Head:
    case DeleteType::Tail:
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,DeleteVal.rect.y,progressAppear);
        break;
    case DeleteType::None:
        buttonVar::buttonGo.rect.y = DeleteVal.rect.y;
        break;
    default:
        break;
    }
}
void Delete::delHeadAnimation(){
    if(!mSSL->getRoot()->next){
        if(!mSSL->getPause()) progressArrow+=GetFrameTime();
        float fontText = lerp(22,0,progressNode);
        drawTextUp("head",fontText,startLinkedListPos);
        drawArrow(startLinkedListPos,{startLinkedListPos.x+ArrowLengthRender,startLinkedListPos.y},color::edgeRendered);
        removeNode(NodeRadiusRender,FontSize,std::to_string(mSSL->getRoot()->data),startLinkedListPos,progressNode, mSSL);
    }
    else 
    {
    std::vector<ShadedData>  movePos,copy;
    handlePos(mSSL->getRoot()->next,{startLinkedListPos.x+EArrow.length+NODE_SIZE,startLinkedListPos.y},movePos);
    handlePos(mSSL->getRoot(),startLinkedListPos,copy);
    if(progressArrow < 1.0f)
    {
    if(!mSSL->getPause()) progressArrow += GetFrameTime();
    ArrowLengthRender = lerp(0,EArrow.length,progressArrow);
    drawArrow(startLinkedListPos,{startLinkedListPos.x+EArrow.length,startLinkedListPos.y},color::edgeNotInMode);
    drawTextUp("head",22,startLinkedListPos);
    drawTextDown("pointer",22,startLinkedListPos);
    drawPos(copy,NODE_SIZE,FontNode);
    }
    drawArrow(startLinkedListPos,{startLinkedListPos.x+ArrowLengthRender,startLinkedListPos.y},color::edgeRendered);
    if(progressArrow < 1.0f) drawNode(startLinkedListPos,std::to_string(mSSL->getRoot()->data),NODE_SIZE,color::nodeNotInMode);
    if(progressArrow>=1.0f){
        ArrowLengthRender = lerp(EArrow.length,0,progressNode);
        removeNode(NodeRadiusRender, FontSize,std::to_string(mSSL->getRoot()->data),startLinkedListPos,progressNode,mSSL);
        for(int i=0; i<movePos.size(); i++){
            movePos[i].pos = lerp(movePos[i].pos,copy[i].pos,progressNode);
        }
        drawTextUp("head",22,movePos[0].pos); 
    }
    for(int i=0; i<movePos.size()-1;i++){
        drawArrow2Node(movePos[i].pos,movePos[i+1].pos,color::edgeNotInMode);
    }
    drawPos(movePos,NODE_SIZE,FontNode);
    }
    if(progressArrow >= 1.0f && progressNode>=1.0f){
        mSSL->setNumElement(mSSL->getNumElement()-1);
        mSSL->delHeadList();
        progressArrow = progressNode = 0.0f;
        DeleteHeadProcess = false;
        textIn.clear();
    }
}
void Delete::delTailAnimation(ListNode*& cur){
    const int ARROW_LENGTH = EArrow.length;
    if(!mSSL->getPause()) framecntDel++;
    if(cur == mSSL->getRoot() && framecntDel<=30) curline = 1;
    else if(framecntDel >=30) curline = 2;
    if(cur->next) drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    if(framecntDel >= 60 && cur->next) {
        prevpos = shadedPos.pos; 
        nodeNext(cur,shadedPos.pos,framecntDel);
        curline = 3;
    }
    shadedPos.node = cur;
    if(!cur->next && prevpos.x!=0 && prevpos.y!=0) {
        drawArrow2Node(prevpos,shadedPos.pos,color::edgeNotInMode);
    }
    if(!cur->next){
        curline = 4;
        drawPartofLinkedListNotColor(mSSL->getRoot(),cur,mSSL);
        drawTextUp("head",22,startLinkedListPos);
        Vector2 targetPos = (shadedPos.pos.x+ARROW_LENGTH+NODE_SIZE < GetScreenWidth()-SCREEN_MARGIN) ? Vector2{shadedPos.pos.x+ARROW_LENGTH+NODE_SIZE,shadedPos.pos.y} :Vector2{startLinkedListPos.x,shadedPos.pos.y+ROW_OFFSET};
        Vector2 direct = Vector2Subtract(targetPos, shadedPos.pos);
        float length = Vector2Length(direct);
        Vector2 endPos = { targetPos.x - (NODE_SIZE * direct.x / length),targetPos.y - (NODE_SIZE * direct.y / length) };
        float ArrowOri = Vector2Length(Vector2Subtract(shadedPos.pos, endPos));
        float sin = direct.y/length; 
        float cos = direct.x/length;
        ArrowLengthRender = lerp(ArrowOri, 0, progressNode);
        drawArrow(shadedPos.pos,{shadedPos.pos.x+ArrowLengthRender*cos,shadedPos.pos.y+ArrowLengthRender*sin},color::edgeRendered);
        removeNode(NodeRadiusRender,FontSize,std::to_string(shadedPos.node->data),shadedPos.pos,progressNode,mSSL);
    }
    if(progressNode>=1.0f){
        mSSL->setNumElement(mSSL->getNumElement()-1);
        mSSL->delTailList();
        textIn.clear();
        progressNode = framecntDel = 0;
        shadedPos = { {0, 0}, nullptr };
        prevpos = {0,0};
        DeleteTailProcess = false;
    }
    else if (cur && cur->next && framecntDel >= 30 && framecntDel < 60) {
        if (shadedPos.pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN) {
            drawArrow2Node(shadedPos.pos, { startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, color::edgeRendered);
            drawNode({ startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, std::to_string(cur->next->data), NODE_SIZE, color::nodeNotInMode);
        } else {
            drawArrow(shadedPos.pos, { shadedPos.pos.x + ARROW_LENGTH, shadedPos.pos.y }, color::edgeRendered);
        }
    }
}
void Delete::delAnimation(ListNode*& currentNode, int targetData) {
    if(!mSSL->getPause()) framecntDel++;
    if(currentNode->data!=targetData) drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    const int ARROW_LENGTH = EArrow.length; 
    if(currentNode == mSSL->getRoot() && framecntDel < 30) curline = 1;
    else if(framecntDel >= 30){
        curline = 2;
        curlinetmp = -1;
    }
    if (framecntDel >= 60 && currentNode->data != targetData) {
        prevpos = shadedPos.pos; 
        nodeNext(currentNode,shadedPos.pos,framecntDel);
        curline = 3;
        curlinetmp = 4;
    }
    shadedPos.node = currentNode;
    if(!shadedPos.node) return;
    if (currentNode && currentNode->data == targetData) {
        curline = 5;
        curlinetmp = 6;
        drawTextDown("prev",20,prevpos);
        if(prevpos.x!=0) drawArrow2Node(prevpos,shadedPos.pos,color::edgeNotInMode);
        drawPartofLinkedListNotColor(mSSL->getRoot(),currentNode,mSSL);
        std::vector<ShadedData>  movePos, copy;
        handlePos(currentNode,shadedPos.pos,movePos);
        copy = movePos;
        Vector2 targetPos = (shadedPos.pos.x+ARROW_LENGTH+NODE_SIZE < GetScreenWidth()-SCREEN_MARGIN) ? Vector2{shadedPos.pos.x+ARROW_LENGTH+NODE_SIZE,shadedPos.pos.y} :Vector2{startLinkedListPos.x,shadedPos.pos.y+ROW_OFFSET};
        Vector2 direct = Vector2Subtract(targetPos, shadedPos.pos);
        float length = Vector2Length(direct);
        Vector2 endPos = { targetPos.x - (NODE_SIZE * direct.x / length),targetPos.y - (NODE_SIZE * direct.y / length) };
        float ArrowOri = Vector2Length(Vector2Subtract(shadedPos.pos, endPos));
        float sin = direct.y/length; 
        float cos = direct.x/length;
        ArrowLengthRender = lerp(ArrowOri,0,progressNode);
        for(int i=1; i<movePos.size(); i++){
                movePos[i].pos = lerp(movePos[i].pos,copy[i-1].pos,progressNode);
        }
        drawTextDown("aft",20,movePos[1].pos);
        for(int i=1; i<movePos.size()-1;i++){
            drawArrow2Node(movePos[i].pos,movePos[i+1].pos,color::edgeNotInMode);
        }
        if (progressNode >= 1.0f) {
            existVal = mSSL->delList(targetData);
            mSSL->setNumElement(mSSL->getNumElement()-1);
            textIn.clear();
            DeleteValProcess = DeleteHeadProcess = DeleteTailProcess = false;
            progressNode = framecntDel = 0;
            shadedPos = { {0, 0}, nullptr };
            prevpos = {0,0};
        }
        drawArrow(shadedPos.pos,{shadedPos.pos.x+ArrowLengthRender*cos,shadedPos.pos.y+ArrowLengthRender*sin},color::edgeRendered);
        removeNode(NodeRadiusRender,FontSize,textIn,shadedPos.pos,progressNode, mSSL);
        for(int i=1; i<movePos.size(); i++){
            drawNode(movePos[i].pos, std::to_string(movePos[i].node->data),NODE_SIZE,color::nodeNotInMode);
        }
        }
    else if (currentNode && currentNode->next && framecntDel >= 30 && framecntDel < 60) {
        if (shadedPos.pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN) {
            drawArrow2Node(shadedPos.pos, { startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, color::edgeRendered);
            drawNode({ startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, std::to_string(currentNode->next->data), NODE_SIZE, color::nodeNotInMode);
        } else {
            drawArrow(shadedPos.pos, { shadedPos.pos.x + ARROW_LENGTH, shadedPos.pos.y }, color::edgeRendered);
        }
    }
}
void Delete::draw(){
    drawTextCode(curline,curlinetmp);
    drawButtons();
    DrawButton(DeleteHead.rect,DeleteHead.text,DeleteHead.buCol,SSLFont,19);
    DrawButton(DeleteTail.rect,DeleteTail.text,DeleteTail.buCol,SSLFont,20);
    DrawButton(DeleteVal.rect,DeleteVal.text,DeleteVal.buCol,SSLFont,20);
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    if(inputRect.height == button::sizeH) drawTextIn(textIn, inputRect, frameCounter);
    if(cur!=DeleteType::None) DrawButton(buttonVar::buttonGo.rect,buttonVar::buttonGo.text,buttonVar::buttonGo.buCol,SSLFont,22);
    if (!mSSL->getRoot() && frameCounter < 30 && cur == DeleteType::Val) {
        DrawTextEx(SSLFont, "Empty List", {inputRect.x + 125, inputRect.y+7.0f}, 22, 2, RED);
    } else {
        if (DeleteValProcess) {
                int x = std::stoi(textIn);
                delAnimation(currentAnimatingNode, x);
        }
        if (shadedPos.node && shadedPos.node->next && shadedPos.pos.x != 0 && shadedPos.pos.y != 0 && !textIn.empty() && shadedPos.node->data != std::stoi(textIn) && !DeleteTailProcess) {
            std::string str = std::to_string(shadedPos.node->data);
            drawNode(shadedPos.pos, str, NODE_SIZE, color::nodeRendered);
            drawTextDown("pointer",20,shadedPos.pos);
        }
        else if(shadedPos.node && shadedPos.pos.x != 0 && shadedPos.pos.y != 0 && DeleteTailProcess && shadedPos.node!=mSSL->getTail()){
            std::string str = std::to_string(shadedPos.node->data);
            drawNode(shadedPos.pos, str, NODE_SIZE, color::nodeRendered);
            drawTextDown("pointer",20,shadedPos.pos);
        }
        if (!existVal && frameCounter < 30 && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
            DrawTextEx(SSLFont, "Value Not Found", {inputRect.x +125.0f, inputRect.y + 7.0f}, 20, 2, RED);
        }
        if(DeleteHeadProcess){
            delHeadAnimation();
        }
        if(DeleteTailProcess){
            delTailAnimation(currentAnimatingNode);
        }
        if (shadedPos.node && shadedPos.node->next && shadedPos.pos.x != 0 && shadedPos.pos.y != 0 && DeleteTailProcess) {
            std::string str = std::to_string(shadedPos.node->data);
            drawNode(shadedPos.pos, str, NODE_SIZE, color::nodeRendered);
        }
        if (!DeleteValProcess && !DeleteHeadProcess && !DeleteTailProcess) {
            drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
        }
    }
}
void Delete::ResetDeleteState(){
    textIn.clear();
    DeleteValProcess = DeleteHeadProcess = DeleteTailProcess = false;
    progressNode = framecntDel = 0;
    shadedPos = { {0, 0}, nullptr };
    cur = DeleteType::None;
    inputRect.height = 0;
    curline = curlinetmp = -1;
}
void Delete::handle(){
    handleButtonsHover();
    IState::ToggleSwitch toggle = mSSL->getToggle();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    DeleteType detectedMode = DetectCurrentMode();
    if (mousePressed && detectedMode != DeleteType::None) {
        handleModeTransitTion(detectedMode);
    }
    updateCommonAnimation();
    if(cur == DeleteType::Head && toggle.isStepByStep) handleHeadCode();
    if(cur == DeleteType::Tail && toggle.isStepByStep) handleTailCode();
    if(cur == DeleteType::Val && toggle.isStepByStep) handleValCode();
    if(CheckCollisionPointRec(mouse, DeleteHead.rect)){
        DeleteHead.buCol = color::buttonFileHovered;
    }
    else if(CheckCollisionPointRec(mouse, DeleteTail.rect)){
        DeleteTail.buCol = color::buttonFileHovered;
    }
    else if(CheckCollisionPointRec(mouse, DeleteVal.rect)){
        DeleteVal.buCol = color::buttonFileHovered;
    }
    else{
        DeleteHead.buCol = color::buttonFile;
        DeleteTail.buCol = color::buttonFile;
        DeleteVal.buCol = color::buttonFile;     
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && mousePressed && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
        mSSL->setState(mSSL->getCreate());
        ResetDeleteState();
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && mousePressed && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
        mSSL->setState(mSSL->getInsert());
        ResetDeleteState();
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && mousePressed && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
        mSSL->setState(mSSL->getnotInMode());
        ResetDeleteState();
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && mousePressed && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
        mSSL->setState(mSSL->getFind());
        ResetDeleteState();
        buttonVar::buttonGo    = {{buttonVar::buttonF.rect.x+250, buttonVar::buttonF.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && mousePressed) {
        mSSL->setState(mSSL->getClear());
        ResetDeleteState();
        mSSL->clearStackUndo();
        mSSL->clearStackRedo();
    }
    SSL::command command;
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9') {
        textIn.push_back(static_cast<char>(key));
    }
    if (key == KEY_BACKSPACE && !textIn.empty()) {
        textIn.pop_back();
    }
    if (cur == DeleteType::Val && !textIn.empty() && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed))) {
        if (!mSSL->getRoot()) {
            textIn.clear();
        }
        if(mSSL->getRoot() && !mSSL->findList(std::stoi(textIn))){
            shadedPos.pos = {0,0};
            curline = curlinetmp = -1;
            existVal = false;
        }
        else if (mSSL->getRoot() && mSSL->findList(std::stoi(textIn))!=mSSL->getRoot() && mSSL->findList(std::stoi(textIn))!=mSSL->getTail() && toggle.isStepByStep) {
            shadedPos.node = nullptr;
            shadedPos.pos = startLinkedListPos;
            DeleteValProcess = true;
            currentAnimatingNode = mSSL->getRoot();
            modeCur = DeleteType::Val;
            curline = curlinetmp = -1;
            ListNode* cur = mSSL->getRoot();
            int idx = 0;
            while(cur != mSSL->findList(std::stoi(textIn))){
                idx++;
                cur = cur->next;
            }   
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Val,idx,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if (mSSL->getRoot() && mSSL->findList(std::stoi(textIn))!=mSSL->getRoot() && mSSL->findList(std::stoi(textIn))!=mSSL->getTail() && !toggle.isStepByStep) {
            shadedPos.node = mSSL->getRoot();
            shadedPos.pos = startLinkedListPos;
            DeleteValProcess = true;
            currentAnimatingNode = mSSL->getRoot();
            modeCur = DeleteType::Val;
            curline = curlinetmp = -1;
            int idx = 0;
            while(currentAnimatingNode != mSSL->findList(std::stoi(textIn))){
                idx++;
                prevpos = shadedPos.pos;
                nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel);
                shadedPos.node = currentAnimatingNode;
            }   
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Val,idx,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if(mSSL->getRoot() && mSSL->findList(std::stoi(textIn))==mSSL->getRoot() && toggle.isStepByStep){
            shadedPos.pos = {0,0};
            DeleteHeadProcess = true;
            NodeRadiusRender = NODE_SIZE;
            ArrowLengthRender = EArrow.length;
            FontSize = FontNode;
            modeCur = DeleteType::Head;
            curline = curlinetmp = -1;
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Head,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if(mSSL->getRoot() && mSSL->findList(std::stoi(textIn))==mSSL->getTail() && toggle.isStepByStep){
            shadedPos.node = nullptr;
            shadedPos.pos = startLinkedListPos;
            DeleteTailProcess = true;
            currentAnimatingNode = mSSL->getRoot();
            modeCur = DeleteType::Tail;
            curline = curlinetmp = -1;
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Tail,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
        else if(mSSL->getRoot() && mSSL->findList(std::stoi(textIn))==mSSL->getTail() && !toggle.isStepByStep){
            shadedPos.node = mSSL->getRoot();
            shadedPos.pos = startLinkedListPos;
            currentAnimatingNode = mSSL->getRoot();
            while(currentAnimatingNode->next){
                prevpos = shadedPos.pos; 
                nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel);
                shadedPos.node = currentAnimatingNode;
            }
            DeleteTailProcess = true;
            curline = curlinetmp = -1;
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Tail,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
        }
    }
    if(!DeleteHeadProcess && CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed && ((mSSL->getRoot() && cur == DeleteType::Head) || (mSSL->getRoot() && !mSSL->getRoot()->next))){
        shadedPos.pos = {0,0};
        DeleteHeadProcess = true;
        NodeRadiusRender = NODE_SIZE;
        ArrowLengthRender = EArrow.length;
        FontSize = FontNode;
        curline = curlinetmp = -1;
        command = {InsertType::None,InsertType::None,DeleteType::Head,DeleteType::None,1,mSSL->getRoot()->data};
        mSSL->pushStack(mSSL->getCommandUndo(),command);
    }
    if(toggle.isStepByStep && !DeleteTailProcess && mSSL->getRoot() && mSSL->getRoot()->next && cur == DeleteType::Tail && CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed){
        shadedPos.node = nullptr;
        shadedPos.pos = startLinkedListPos;
        DeleteTailProcess = true;
        currentAnimatingNode = mSSL->getRoot();
        curline = curlinetmp = -1;
        command = {InsertType::None,InsertType::None,DeleteType::Tail,DeleteType::None,1,mSSL->getTail()->data};
        mSSL->pushStack(mSSL->getCommandUndo(),command);
    }
    if(!toggle.isStepByStep && !DeleteTailProcess && mSSL->getRoot() && mSSL->getRoot()->next && cur == DeleteType::Tail && CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed){
        shadedPos.node = mSSL->getRoot();
        shadedPos.pos = startLinkedListPos;
        currentAnimatingNode = mSSL->getRoot();
        while(currentAnimatingNode->next){
            prevpos = shadedPos.pos; 
            nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel);
            shadedPos.node = currentAnimatingNode;
        }
        DeleteTailProcess = true;
        curline = curlinetmp = -1;
        command = {InsertType::None,InsertType::None,DeleteType::Tail,DeleteType::None,1,mSSL->getTail()->data};
        mSSL->pushStack(mSSL->getCommandUndo(),command);
    }
    if(UndoButton.isPressed() && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) mSSL->handleUndo();
    if(RedoButton.isPressed() && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) mSSL->handleRedo();
}
// Find
Find::Find(SSL* s)
: mSSL(s), frameCounter(0), textIn(""), framecntFind(0), FindProcess(false), shadedPos({0,0}) {}
void Find::findAnimation(ListNode*& root) {
    if (!root) {
        FindProcess = false;
        shadedPos = { {0, 0}, nullptr };
        textIn.clear();
        curline = 4;
        curlinetmp = 5;
        return;
    }
    if(!mSSL->getPause()) framecntFind++;
    drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    const int ARROW_LENGTH = EArrow.length; 
    if(root == mSSL->getRoot() && framecntFind<30) curline = 1;
    if (shadedPos.pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth()-SCREEN_MARGIN && framecntFind>=30 && framecntFind < 60 && root->next) {
        drawArrow2Node(shadedPos.pos,{ startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET },color::edgeRendered);
        curline = 2;
    } else if(shadedPos.pos.x + EArrow.length + NODE_SIZE < GetScreenWidth()-SCREEN_MARGIN && framecntFind>=30 && framecntFind < 60 && root->next) {
        drawArrow(shadedPos.pos,{ shadedPos.pos.x+EArrow.length, shadedPos.pos.y},color::edgeRendered);
        curline = 2;
    }
    if (framecntFind >= 60) {
        nodeNext(root,shadedPos.pos,framecntFind);
        curline = 3;
    }
    shadedPos.node = root;
    if(!shadedPos.node) return;
    if (root == mSSL->getExistVal()) {
        curline = 6;
        curlinetmp = -1;
        FindProcess = false;
        textIn.clear(); 
        return;
    }
}
void Find::handleCodeLine(){
    for(int i=0; i<codeLines.size(); i++){
        code[i].text = codeLines[i];
    }
    if(!mSSL->getRoot()) {
        curline = 0;
        curlinetmp = -1;
        return;
    }
    if(currentAnimatingNode && !currentAnimatingNode->next && framecntFind>0 && framecntFind<=30) {
        curline = 4;
        curlinetmp = 2;
    }
    if(curline == 4 && !currentAnimatingNode){
        curline = 5;
        curlinetmp = -1;
    }
}
void Find::draw() {
    drawButtons();
    drawTextCode(curline,curlinetmp);
    Rectangle inputRect = { buttonVar::buttonF.rect.x + 115,buttonVar::buttonF.rect.y,120, (float)button::sizeH };
    DrawRectangleRounded(inputRect, 0.3f, 30, WHITE);
    drawTextIn(textIn, inputRect, frameCounter);
    DrawButton(buttonVar::buttonGo.rect,buttonVar::buttonGo.text,buttonVar::buttonGo.buCol,SSLFont,22);
    if (!mSSL->getRoot() && frameCounter < 30) {
        DrawTextEx(SSLFont, "Empty List", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
    }
    else {  
        if (FindProcess) findAnimation(currentAnimatingNode);
        else if (mSSL->getRoot() && !mSSL->getExistVal() && frameCounter < 30 && !FindProcess) {
            DrawTextEx(SSLFont, "Value Not Found",{ inputRect.x - 3, inputRect.y - 20 }, 22, 2, RED);
        }
        if(!FindProcess) drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL); 
        if (shadedPos.node && shadedPos.pos.x != 0 && shadedPos.pos.y != 0) {
            std::string str = std::to_string(shadedPos.node->data);
            drawNode(shadedPos.pos, str, NODE_SIZE,color::nodeRendered);
            drawTextDown("pointer",20,shadedPos.pos);
            if(frameCounter < 30 && !FindProcess){
                DrawTextEx(SSLFont, "Value Found", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);}
        }
    }  
}
void Find::ResetFindState(){
    textIn.clear();
    mSSL->setExistVal(mSSL->getRoot());
    FindProcess = false;
    currentAnimatingNode = nullptr;
    shadedPos = {0,0};
    curline = -1;
    curlinetmp = -1;
}
void Find::handle(){
        IState::ToggleSwitch toggle = mSSL->getToggle();
        if(toggle.isStepByStep) handleCodeLine();
        handleButtonsHover();
        if(CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect)){
            buttonVar::buttonGo.buCol = color::buttonFileHovered;
        }
        else buttonVar::buttonGo.buCol = color::buttonFile;
        if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !FindProcess) {
            mSSL->setState(mSSL->getCreate());
            ResetFindState();
        }
        if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !FindProcess) {
            mSSL->setState(mSSL->getInsert());
            ResetFindState();
        }
        if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !FindProcess) {
            mSSL->setState(mSSL->getDel());
            ResetFindState();
            buttonVar::buttonGo    = {{buttonVar::buttonDel.rect.x+250, buttonVar::buttonDel.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
        }
        if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !FindProcess) {
            mSSL->setState(mSSL->getnotInMode());
            ResetFindState();
        }
        if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            mSSL->setState(mSSL->getClear());
            ResetFindState();
            mSSL->clearStackUndo();
            mSSL->clearStackRedo();
        }
        int key = GetKeyPressed();
        if(key >= '0' && key <= '9') {
            textIn.push_back((char)key);
        }
        if(key == KEY_BACKSPACE && !textIn.empty()) {
            textIn.pop_back();
        }
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        if(!textIn.empty() && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed)) && !toggle.isStepByStep && mSSL->findList(std::stoi(textIn))){
            shadedPos.pos = startLinkedListPos;
            mSSL->setExistVal(mSSL->findList(std::stoi(textIn)));
            currentAnimatingNode = mSSL->getRoot();
            shadedPos.node = currentAnimatingNode;
            while (currentAnimatingNode != mSSL->getExistVal())
            {
                nodeNext(currentAnimatingNode,shadedPos.pos,framecntFind);
                shadedPos.node = currentAnimatingNode;
            }  
            textIn.clear();       
        }  
        if(!textIn.empty() && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed)) && !toggle.isStepByStep && !mSSL->findList(std::stoi(textIn))){
            mSSL->setExistVal(mSSL->findList(std::stoi(textIn)));
            textIn.clear();
        } 
        if(!textIn.empty() && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed)) && toggle.isStepByStep) {
            curline = 1;
            int x = std::stoi(textIn);
            mSSL->setExistVal(mSSL->findList(x));
            if(mSSL->getRoot()) {
                FindProcess = true;
                shadedPos.pos = startLinkedListPos;
                currentAnimatingNode = mSSL->getRoot();
        }
    }
    if(UndoButton.isPressed() && !FindProcess){
        mSSL->handleUndo();
        shadedPos = {0,0};
    }
    if(RedoButton.isPressed() && !FindProcess){
        mSSL->handleRedo();
        shadedPos = {0,0};
    }
}
//Clear
Clear::Clear(SSL* s) 
:mSSL(s) {}
void Clear::draw(){
    drawTextCode(-1,-1);
    drawButtons();
}
void Clear::handle(){
    mSSL->delAllList();
    handleButtonsHover();
    if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getCreate());
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getInsert());
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getDel());
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getFind());
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getnotInMode());
        mSSL->setExistVal(mSSL->getRoot());
    }
}