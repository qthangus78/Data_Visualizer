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
    IState::ToggleSwitch toggle = mSSL->getToggle();
    toggle.Update(mouse);
    mSSL->setToggle(toggle);
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
IState::CreateType Create::DetectCurrentMode(){
    if (CheckCollisionPointRec(mouse, fileInput.rect)) return CreateType::File;
    if (CheckCollisionPointRec(mouse, random.rect)) return CreateType::Random;
    return CreateType::None;
}
void Create::handleModeTransitTion(CreateType newType){
    prev = cur;
    cur = (cur == newType) ? CreateType::None : newType;
    inProcess = true;
    progressGo = 0.0f;
}
void Create::updateCommonAnimation(){
        if(!inProcess) return;
        progressGo += GetFrameTime();
        switch (cur)
        {
        case CreateType::File:
            handleFileMode();
            break;
        case CreateType::Random:
            handleRandomMode();
            break;
        default:
            break;
        }
        if(progressGo >= 1.0f){
            inProcess = false;
            progressGo = 0.0f;
        }
}
void Create::handleFileMode(){
    switch(prev){
        case CreateType::None:
        buttonVar::buttonGo.rect.y = buttonVar::buttonCreate.rect.y;
        break;
        case CreateType::Random:
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonCreate.rect.y,progressGo);
        break;
        default:
        break;
    }
}
void Create::handleRandomMode(){
    switch(prev){
        case CreateType::None:
        buttonVar::buttonGo.rect.y = buttonVar::buttonIns.rect.y;
        break;
        case CreateType::File:
        buttonVar::buttonGo.rect.y = lerp(buttonVar::buttonGo.rect.y,buttonVar::buttonIns.rect.y,progressGo);
        break;
        default:
        break;
    }
}
void Create::handleInit(){
    const int ARROW_LENGTH = EArrow.length;
    if(cur!=CreateType::Random) return;
    if (!CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect)) {
        buttonVar::buttonGo.buCol = color::buttonFile;
        return;
    }
    buttonVar::buttonGo.buCol = color::buttonFileHovered;
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
    if(cur!=CreateType::File) return;
    if (!CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect)) {
        buttonVar::buttonGo.buCol = color::buttonFile;
        return;
    }
    buttonVar::buttonGo.buCol = color::buttonFileHovered;
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
    float fraction = mSSL->getFraction();
    progress += fraction * deltaTime;
    float NodeRadiusRender = lerp(0, NODE_SIZE, progress);
    float FontSize = lerp(0, FontNode, progress);
    drawPos(pos,NodeRadiusRender,FontSize);
    if (progress >= 1.0f) {
        randomProcess = false;
        progress = 0;
    }
}
void Create::fileAnimation() {
    float fraction = mSSL->getFraction();
    progress += fraction * deltaTime;
    float NodeRadiusRender = lerp(0, NODE_SIZE, progress);
    float FontSize = lerp(0, FontNode, progress);
    drawPos(pos,NodeRadiusRender,FontSize);
    if (progress >= 1.0f) {
        fileProcess = false;
        progress = 0;
    }
}
void Create::draw(){
    if(cur!=CreateType::None) DrawButton(buttonVar::buttonGo.rect,buttonVar::buttonGo.text,buttonVar::buttonGo.buCol,SSLFont,22);
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
    if(CheckCollisionPointRec(mouse, fileInput.rect)){
        fileInput.buCol = color::buttonFileHovered;
    }
    else if(CheckCollisionPointRec(mouse, random.rect)){
        random.buCol = color::buttonFileHovered;
    }
    else{
        fileInput.buCol = color::buttonFile;
        random.buCol = color::buttonFile;
    }
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    CreateType detectedMode = DetectCurrentMode();
    if (mousePressed && detectedMode != CreateType::None) {
        handleModeTransitTion(detectedMode);
    }
    updateCommonAnimation();
    if (CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getnotInMode());
        mSSL->setExistVal(mSSL->getRoot());
        cur = CreateType::None;
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getInsert());
        mSSL->setExistVal(mSSL->getRoot());
        cur = CreateType::None;
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getDel());
        mSSL->setExistVal(mSSL->getRoot());
        cur = CreateType::None;
        buttonVar::buttonGo    = {{buttonVar::buttonDel.rect.x+250, buttonVar::buttonDel.rect.y,60,button::sizeH}, color::buttonColor, "Go"};
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getFind());
        mSSL->setExistVal(mSSL->getRoot());
        cur = CreateType::None;
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250,buttonVar::buttonGo.rect.y = buttonVar::buttonF.rect.y;
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getClear());
        mSSL->setExistVal(mSSL->getRoot());
        cur = CreateType::None;
        buttonVar::buttonGo.rect.x = buttonVar::buttonClear.rect.x+120, buttonVar::buttonGo.rect.y = buttonVar::buttonClear.rect.y;
    }
    if(UndoButton.isPressed()){
        mSSL->handleUndo();
    }
    if(RedoButton.isPressed()){
        mSSL->handleRedo();
    }
    IState::ToggleSwitch toggle = mSSL->getToggle();
    toggle.Update(mouse);
    mSSL->setToggle(toggle);
}
//--------------------------------
// Insert
//--------------------------------
Insert::Insert(SSL* s)
:mSSL(s),InsertTailProcess(false),InsertHeadProcess(false),InsertIdxProcess(false),frameCounter(0),framecntInsert(0),currentAnimatingNode(nullptr),textIn(""),
posTail({1200,70}),progressNode(0), progressArrow(0),textInIndex(""),valInsert(false),idxInsert(false), Font(0)
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
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
    overValue = false;
    InsertTail.rect.y = lerp(InsertTail.rect.y,buttonVar::buttonDel.rect.y,progressAppear);
    inputRect.height = lerp(inputRect.height,0,progressAppear);
    InsertIndex.rect.y = lerp(InsertIndex.rect.y,buttonVar::buttonF.rect.y,progressAppear);
    valRect.height = lerp(valRect.height,0,progressAppear);
    idxRect.height = lerp(idxRect.height,0,progressAppear);
    Index.rect.height = lerp(Index.rect.height,0,progressAppear);
    Value.rect.height = lerp(Value.rect.height,0,progressAppear);
    Font = lerp(Font,0,progressAppear);
    while(!st.empty()) st.pop();
}
void Insert::handleHeadMode(){
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
    inputRect.y = buttonVar::buttonDel.rect.y;
    inputRect.height = lerp(inputRect.height, button::sizeH,progressAppear);
    overValue = false;
    while(!st.empty()) st.pop();
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
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
    overValue = false;
    while(!st.empty()) st.pop();
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
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
    overValue = false;
    while(!st.empty()) st.pop();
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
    float fraction = mSSL->getFraction();
    if(curline == 1 && progressArrow >= 1.0f-fraction*deltaTime && progressNode >= 1.0f-fraction*deltaTime) curline = 2;
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
        float fraction = mSSL->getFraction();
        if(!mSSL->getPause()) progressArrow += fraction*deltaTime;
        float ArrowLengthRender = lerp(NODE_SIZE,EArrow.length+NODE_SIZE,progressArrow);
        drawArrow2Node(startLinkedListPos, {startLinkedListPos.x+ArrowLengthRender,startLinkedListPos.y},color::edgeRendered);
    }
    float NodeRadiusRender, FontSize = 0;
    amplifyNode(NodeRadiusRender,FontSize,startLinkedListPos,x,progressNode,mSSL);
    movesPos(pos,progressNode);
    drawTextUp("head",20,pos[0].pos);
    if(progressNode >= 1.0f && progressArrow >= 1.0f){
        InsertHeadProcess = InsertIdxProcess = false;
        progressNode = progressArrow = 0;
        mSSL->insertHeadList(x);
        mSSL->setNumElement(mSSL->getNumElement()+1);;
        ListNode* cur = mSSL->getRoot();
    }
}
void Insert::insertTailAnimation(ListNode*& tmp, Vector2& posTail) {
    float fraction = mSSL->getFraction();
    std::string tailStr = std::to_string(mSSL->getTail()->data);
    drawNode(posTail, tailStr, NODE_SIZE,color::nodeNotInMode);
    drawTextUp("vtx",20,posTail);
    if(tmp == mSSL->getRoot()) drawTextDown("pointer",22,startLinkedListPos);
    drawPartofLinkedListNotColor(mSSL->getRoot(), mSSL->getTail(), mSSL);
    drawPartofLinkedList(mSSL->getRoot(), tmp, mSSL);
    if(!tmp->next) {
        mSSL->setPause(false);
        if(curline != 0 && framecntInsert >= (int)speedNode/(2*fraction)) curline = 5;
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
            progressNode += (fraction*deltaTime);
        }
        posTail = lerp({1200,70},targetPos,progressNode);
        if(fabs(posTail.x - targetPos.x) < 10 && fabs(posTail.y - targetPos.y) < 10) {
            InsertTailProcess = InsertIdxProcess = false;
            posTail = { 1200, 70 };  
            progressNode = 0;
            framecntInsert = 0;
        }
    }
    else {
        if(!mSSL->getPause()) framecntInsert++;
        if(mSSL->getRoot() && mSSL->getRoot()->next == mSSL->getTail()) curline = 0;
        else if(tmp == mSSL->getRoot()){
            curline = 1;
            curlinetmp = 2;
        }
        else if(framecntInsert >= (int) speedNode/(2*fraction)){
            curline = 3;
            curlinetmp = -1;
        }
        if(framecntInsert >= (int) speedNode/fraction) {
            framecntInsert = 0;
            st.push({tmp,{0,0}});
            tmp = tmp->next;
        if(tmp != mSSL->getRoot()->next){
            curline = 4;
            curlinetmp = -1;}
        }
    }
} 
void Insert::insertIdxAnimation(ListNode*& tmp){
    float fraction = mSSL->getFraction();
    if(curindex == idx){
        mSSL->setPause(false);
        std::vector<ShadedData> movePos;
        handlePos(tmp,pos,movePos);
        if(progressNode>=1.0f){
            curline = 5;
            curlinetmp = 6;
            if(!mSSL->getPause()) progressArrow += fraction*deltaTime;
            Vector2 direct = Vector2Subtract(pos,prevpos);
            float length = Vector2Length(direct);
            float sin = direct.y/length, cos = direct.x/length;
            Vector2 endPos = { pos.x - (NODE_SIZE * cos),pos.y - (NODE_SIZE * sin) };
            float ArrowOri = Vector2Length(Vector2Subtract(prevpos,endPos));
            float ArrowLengthRender = lerp(0,ArrowOri,progressArrow);
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
        float NodeRadiusRender,FontSize = 0; 
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
            mSSL->insertIdxList(idx,nums);
            mSSL->setNumElement(mSSL->getNumElement()+1);
            std::cout << mSSL->getNumElement();
        }
    }
    else{
        drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
        drawPartofLinkedList(mSSL->getRoot(),tmp,mSSL);
        if(tmp == mSSL->getRoot()) {
            drawTextDown("pointer",22,startLinkedListPos);
            curline = 0;
        }
        else if(framecntInsert >= (int)speedNode/(2*fraction)){
            curline = 1;
        }
        if(!mSSL->getPause()) framecntInsert++;
        if(framecntInsert >= speedNode/fraction) {
            curindex++;
            prevpos = pos;
            nodeNext(tmp,pos,framecntInsert,st);
            if(tmp != mSSL->getRoot()->next){
                curline = 2;
            }
        }
    }
}  
void Insert::handleUndo(){
    if(mSSL->getCommandUndo().empty()) return;
    SSL::command prev = mSSL->getTop();
    if(prev.curDeleteType != IState::DeleteType::None || prev.modeDeleteType != IState::DeleteType::None) return;
    if((cur == InsertType::Head || cur == InsertType::Idx) && modeCur != InsertType::Tail && !InsertHeadProcess && !InsertIdxProcess) {
        mSSL->handleUndo();
        mSSL->pushStack(mSSL->getCommandUndo(),prev);
        mSSL->popStack(mSSL->getCommnadRedo());
    }
    float fraction = mSSL->getFraction();
    mSSL->setPause(true);
    if(prev.curInsertType == InsertType::Head || prev.modeInsertType == InsertType::Head){
        InsertHeadProcess = true;
    }
    if(!st.empty()) {
        curline = curlinetmp = -1;
    if(InsertTailProcess || prev.curInsertType == InsertType::Tail || prev.modeInsertType == InsertType::Tail){
            InsertTailProcess = true;
            currentAnimatingNode = st.top().first;
            framecntInsert = 0;
            st.pop();
        }
    if(InsertIdxProcess || (prev.curInsertType == InsertType::Idx && prev.modeInsertType == InsertType::Idx)){
        InsertIdxProcess = true;
        framecntInsert = 0;
        currentAnimatingNode = st.top().first; 
        pos = st.top().second;
        curindex--;
        st.pop();       
    }
}
}
void Insert::handleRedo(){
    if(InsertHeadProcess){
        mSSL->setPause(false);
    }
    if(InsertTailProcess)
    {
    if(currentAnimatingNode->next){
    mSSL->setPause(true);
    st.push({currentAnimatingNode,{0,0}});
    currentAnimatingNode = currentAnimatingNode->next;
    if(!mSSL->getRoot()->next->next){
        curline = 0;
        curlinetmp = -1;
    }
    else if(currentAnimatingNode == mSSL->getRoot()->next){
        curline = 1;
        curlinetmp = 2;
    }
    else{
        curline = 4;
        curlinetmp = -1;
    }
    }
    else{
        mSSL->setPause(false);
        curline = 5;
        curlinetmp = -1;
    }
    }
    if(InsertIdxProcess){
        if(curindex != idx){
            mSSL->setPause(true);
            prevpos = pos;
            nodeNext(currentAnimatingNode,pos,framecntInsert,st);
            curindex++;
            if(curindex == 1){
                curline = 0;
                curlinetmp = -1;
            }
            else{
            curline = 2;
            curlinetmp = -1;
            }
            }
        else{
            mSSL->setPause(false);
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
        if(mSSL->getNumElement() == 29 && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess){
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
            DrawTextEx(SSLFont,str.c_str(),{Index.rect.x+210,Index.rect.y+8.0f},21,1,RED);
            drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    }
    else {
        if(overValue){
            std::string str = "Enter value [0,100]";
            if(cur == InsertType::Head || cur == InsertType::Tail) DrawTextEx(SSLFont,str.c_str(),{inputRect.x+130,inputRect.y+8.0f},21,1,RED);
            else if(cur == InsertType::Idx) DrawTextEx(SSLFont,str.c_str(),{Value.rect.x+210,Value.rect.y+8.0f},21,1,RED);
        }
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
    overValue = false;
    inIndex - true;
    while(!st.empty()) st.pop();
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
}
void Insert::handle(){
    IState::ToggleSwitch toggle = mSSL->getToggle();
    bool step = toggle.isStepByStep;
    static bool prevStep = toggle.isStepByStep;
    if(!InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess){
        toggle.Update(mouse);
        mSSL->setToggle(toggle);
        if(step != toggle.isStepByStep){
            prevStep = step;
        }
        step = toggle.isStepByStep;
    }
    if(toggle.isStepByStep) handleCodeLine();
    handleButtonsHover();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    InsertType detectedMode = DetectCurrentMode();
    if (mousePressed && detectedMode != InsertType::None && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
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
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && mousePressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
        mSSL->setState(mSSL->getFind());
        ResetInsertState();
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250, buttonVar::buttonGo.rect.y = buttonVar::buttonF.rect.y;
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && mousePressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess) {
        mSSL->setState(mSSL->getClear());
        ResetInsertState();
        buttonVar::buttonGo.rect.x = buttonVar::buttonClear.rect.x+120, buttonVar::buttonGo.rect.y = buttonVar::buttonClear.rect.y;
    }
    SSL::command command;
    if (cur == InsertType::Head || cur == InsertType::Tail) {
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9')
        textIn.push_back((char)key);
    if (key == KEY_BACKSPACE && !textIn.empty())
        textIn.pop_back();
    bool goPressed = (key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed);
    if(!textIn.empty() && goPressed && !InsertHeadProcess && !InsertTailProcess && mSSL->getNumElement()<=28 && (std::stoi(textIn) < 0 || std::stoi(textIn) > 100)){
        overValue = true;
        while(!st.empty()) st.pop();
    }
    else if (!textIn.empty() && goPressed && !InsertHeadProcess && !InsertTailProcess && mSSL->getNumElement()<=28) {
        if (cur == InsertType::Tail) {
            mSSL->clearStackRedo();
            command = {InsertType::Tail,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            overValue = false;
            while(!st.empty()) st.pop();
            if(mSSL->getRoot()) inIndex = true;
            else if(!mSSL->getRoot()) inIndex = false;
            if(toggle.isStepByStep){
                if (mSSL->getRoot()) {
                InsertTailProcess = true;
                currentAnimatingNode = mSSL->getRoot();
                curline = curlinetmp = -1;
                mSSL->setNumElement(mSSL->getNumElement()+1);
                }
                else{
                    mSSL->setNumElement(mSSL->getNumElement()+1);
                }
                prevStep = toggle.isStepByStep;
            }
            mSSL->insertTailList(std::stoi(textIn));
            if(!toggle.isStepByStep){
                if (mSSL->getRoot()) {
                    InsertTailProcess = true;
                    mSSL->setNumElement(mSSL->getNumElement()+1);
                }
                else{
                    mSSL->setNumElement(mSSL->getNumElement()+1);
                }
                currentAnimatingNode = mSSL->getTail();
            }
        }
        else if (cur == InsertType::Head) {
            mSSL->clearStackRedo();
            command = {InsertType::Head,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            overValue = false;
            while(!st.empty()) st.pop();
            if(mSSL->getRoot()) inIndex = true;
            else if(!mSSL->getRoot()) inIndex = false;
            prevStep = toggle.isStepByStep;
            if (mSSL->getRoot()) {
                InsertHeadProcess = true;
                nums = std::stoi(textIn);
                curline = curlinetmp = -1;
            } 
            else {
                mSSL->insertHeadList(std::stoi(textIn));
                mSSL->setNumElement(mSSL->getNumElement() + 1);
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
    if (!textIn.empty() && !textInIndex.empty() && std::stoi(textInIndex) > 0 && goPressed && !InsertHeadProcess && !InsertTailProcess && !InsertIdxProcess && mSSL->getNumElement()<=28) {
        int idxInput = std::stoi(textInIndex) - 1;
        if(idxInput < mSSL->getNumElement() && (std::stoi(textIn) < 0 || std::stoi(textIn) > 100)){
            overValue = true;
            idx = idxInput;
            while(!st.empty()) st.pop();
        }
        else if (idxInput != mSSL->getNumElement() && idxInput != 0 && idxInput < mSSL->getNumElement()) {
            currentAnimatingNode = mSSL->getRoot();
            pos = startLinkedListPos;
            overValue = false;
            modeCur = InsertType::Idx;
            curline = curlinetmp = -1;
            curindex = 0;
            nums = std::stoi(textIn);
            idx = idxInput;
            textIn.clear();
            textInIndex.clear();
            InsertIdxProcess = true;
            mSSL->clearStackRedo();
            command = {InsertType::Idx,InsertType::Idx,DeleteType::None,DeleteType::None,idx,nums};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            while(!st.empty()) st.pop();
            if(mSSL->getRoot()) inIndex = true;
            else if(!mSSL->getRoot()) inIndex = false;
            if(toggle.isStepByStep){
            prevStep = toggle.isStepByStep;
            }
            else{
                currentAnimatingNode = mSSL->getRoot();
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
            }
        }
        else if(idxInput > mSSL->getNumElement()){
            nums = std::stoi(textIn);
            idx = idxInput;
            modeCur = InsertType::Idx;
            curline = curlinetmp = -1;
            while(!st.empty()) st.pop();
            inIndex = false;
        }
        else {
                if (idxInput == mSSL->getNumElement()) {
                mSSL->clearStackRedo();
                command = {InsertType::Tail,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                mSSL->pushStack(mSSL->getCommandUndo(),command);
                idx = idxInput;
                overValue = false;
                while(!st.empty()) st.pop();
                if(mSSL->getRoot()) inIndex = true;
                else if(!mSSL->getRoot()) inIndex = false;
                if(toggle.isStepByStep)
                {
                    if (mSSL->getRoot()) {
                    InsertTailProcess = true;
                    currentAnimatingNode = mSSL->getRoot();
                    modeCur = InsertType::Tail;
                    curline = curlinetmp = -1;
                    mSSL->setNumElement(mSSL->getNumElement()+1);
                    }
                    else{
                        mSSL->setNumElement(mSSL->getNumElement()+1);
                    }
                    prevStep = toggle.isStepByStep;
                }
                mSSL->insertTailList(std::stoi(textIn));
                if(!toggle.isStepByStep){
                    if (mSSL->getRoot()) {
                        InsertTailProcess = true;
                        mSSL->setNumElement(mSSL->getNumElement()+1);
                    }
                    else{
                        mSSL->setNumElement(mSSL->getNumElement()+1);
                    }
                    currentAnimatingNode = mSSL->getTail();
                }
                textIn.clear();
                textInIndex.clear();
            }
            else if(idxInput == 0){
                idx = idxInput;
                mSSL->clearStackRedo();
                command = {InsertType::Head,InsertType::None,DeleteType::None,DeleteType::None,1,std::stoi(textIn)};
                mSSL->pushStack(mSSL->getCommandUndo(),command);
                overValue = false;
                while(!st.empty()) st.pop();
                if(mSSL->getRoot()) inIndex = true;
                else if(!mSSL->getRoot()) inIndex = false;
                prevStep = toggle.isStepByStep;
                if (mSSL->getRoot()) {
                    InsertHeadProcess = true;
                    nums = std::stoi(textIn);
                    modeCur = InsertType::Head;
                    curline = curlinetmp = -1;
                }
                else {
                    mSSL->insertHeadList(std::stoi(textIn));
                    mSSL->setNumElement(mSSL->getNumElement()+1);
                }
                textIn.clear();
                textInIndex.clear();
                }
            }
        }
    } 
    SSL::command prev;
    if(!mSSL->getCommandUndo().empty()) prev = mSSL->getTop();
    if(UndoButton.isPressed() && mSSL->getRoot() && mSSL->getRoot()->next && prevStep && progressArrow == 0 && progressNode == 0 && inIndex && !overValue && step && !mSSL->getCommandUndo().empty() && prev.curDeleteType == IState::DeleteType::None && prev.modeDeleteType == IState::DeleteType::None) {
        handleUndo();
    }
    if(RedoButton.isPressed() && progressArrow == 0 && progressNode == 0 && (InsertHeadProcess || InsertTailProcess || InsertIdxProcess)) handleRedo();
}
// Delete
Delete::Delete(SSL* s)
: mSSL(s), frameCounter(0), textIn(""), existVal(true), currentAnimatingNode(nullptr), progressNode(0), progressArrow(0),DeleteValProcess(false),
DeleteHeadProcess(false), DeleteTailProcess(false), framecntDel(0), ArrowLengthRender(EArrow.length),shadedPos({startLinkedListPos}),progressAppear(0)
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
    float fraction = mSSL->getFraction();
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
    if(curline == 4 && progressArrow >= 1.0f-fraction*deltaTime) curline = 5;
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
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
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
    existVal = true;
    inputRect.height = lerp(inputRect.height,0,progressAppear);
    mSSL->clearStackRedo();
}
void Delete::handleHeadMode(){
    existVal = true;
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
    existVal = true;
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
    existVal = true;
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
    float fraction = mSSL->getFraction();
    if(!mSSL->getRoot()->next){
        if(!mSSL->getPause()) progressArrow += fraction*deltaTime;
        ArrowLengthRender = lerp(EArrow.length,0,progressArrow);
        float fontText = lerp(22,0,progressNode);
        drawTextUp("head",fontText,startLinkedListPos);
        drawArrow(startLinkedListPos,{startLinkedListPos.x+ArrowLengthRender,startLinkedListPos.y},color::edgeRendered);
        float NodeRadiusRender,FontSize = 0;
        removeNode(NodeRadiusRender,FontSize,std::to_string(mSSL->getRoot()->data),startLinkedListPos,progressNode, mSSL);
    }
    else 
    {
    std::vector<ShadedData>  movePos,copy;
    handlePos(mSSL->getRoot()->next,{startLinkedListPos.x+EArrow.length+NODE_SIZE,startLinkedListPos.y},movePos);
    handlePos(mSSL->getRoot(),startLinkedListPos,copy);
    if(progressArrow < 1.0f)
    {
    if(!mSSL->getPause()) progressArrow += fraction*deltaTime;
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
        float NodeRadiusRender, FontSize = 0;
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
    float fraction = mSSL->getFraction();
    const int ARROW_LENGTH = EArrow.length;
    if(!mSSL->getPause()) framecntDel++;
    if(cur == mSSL->getRoot() && framecntDel <= (int)speedNode/(2*fraction)) curline = 1;
    else if(framecntDel >= (int)speedNode/(2*fraction)) curline = 2;
    if(cur->next) drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    if(framecntDel >= (int)speedNode/fraction && cur->next) {
        prevpos = shadedPos.pos; 
        nodeNext(cur,shadedPos.pos,framecntDel,st);
        curline = 3;
    }
    shadedPos.node = cur;
    if(!cur->next && prevpos.x!=0 && prevpos.y!=0) {
        drawArrow2Node(prevpos,shadedPos.pos,color::edgeNotInMode);
    }
    if(!cur->next){
        mSSL->setPause(false);
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
        float NodeRadiusRender,FontSize = 0;
        removeNode(NodeRadiusRender,FontSize,std::to_string(shadedPos.node->data),shadedPos.pos,progressNode,mSSL);
    }
    if(progressNode>=1.0f){
        mSSL->setNumElement(mSSL->getNumElement()-1);
        mSSL->delTailList();
        textIn.clear();
        progressArrow = progressNode = framecntDel = 0;
        shadedPos = { {0, 0}, nullptr };
        prevpos = {0,0};
        DeleteTailProcess = false;
    }
    else if (cur && cur->next && framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction) {
        if (shadedPos.pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN) {
            drawArrow2Node(shadedPos.pos, { startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, color::edgeRendered);
            drawNode({ startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, std::to_string(cur->next->data), NODE_SIZE, color::nodeNotInMode);
        } else {
            drawArrow(shadedPos.pos, { shadedPos.pos.x + ARROW_LENGTH, shadedPos.pos.y }, color::edgeRendered);
        }
    }
}
void Delete::delAnimation(ListNode*& currentNode, int targetData) {
    float fraction = mSSL->getFraction();
    if(!mSSL->getPause()) framecntDel++;
    if(currentNode->data!=targetData) drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
    const int ARROW_LENGTH = EArrow.length; 
    if(currentNode == mSSL->getRoot() && framecntDel < (int)speedNode/(2*fraction)) curline = 1;
    else if(framecntDel >= (int)speedNode/fraction){
        curline = 2;
        curlinetmp = -1;
    }
    if (framecntDel >= (int)speedNode/fraction && currentNode->data != targetData) {
        prevpos = shadedPos.pos; 
        nodeNext(currentNode,shadedPos.pos,framecntDel,st);
        curline = 3;
        curlinetmp = 4;
    }
    shadedPos.node = currentNode;
    if(!shadedPos.node) return;
    if (currentNode && currentNode->data == targetData) {
        mSSL->setPause(false);
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
            framecntDel = 0;
            progressNode = 0.0f;
            shadedPos = { {0, 0}, nullptr };
            prevpos = {0,0}; 
        }
        drawArrow(shadedPos.pos,{shadedPos.pos.x+ArrowLengthRender*cos,shadedPos.pos.y+ArrowLengthRender*sin},color::edgeRendered);
        float NodeRadiusRender,FontSize = 0;
        removeNode(NodeRadiusRender,FontSize,textIn,shadedPos.pos,progressNode, mSSL);
        for(int i=1; i<movePos.size(); i++){
            drawNode(movePos[i].pos, std::to_string(movePos[i].node->data),NODE_SIZE,color::nodeNotInMode);
        }
        }    else if (currentNode && currentNode->next && framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction) {
        if (shadedPos.pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth() - SCREEN_MARGIN) {
            drawArrow2Node(shadedPos.pos, { startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, color::edgeRendered);
            drawNode({ startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET }, std::to_string(currentNode->next->data), NODE_SIZE, color::nodeNotInMode);
            curline = 2;
            curlinetmp = -1;
        } else {
            drawArrow(shadedPos.pos, { shadedPos.pos.x + ARROW_LENGTH, shadedPos.pos.y }, color::edgeRendered);
            curline = 2;
            curlinetmp = -1;
        }
    }
}
void Delete::handleUndo(){
    if( mSSL->getCommandUndo().empty()) return;
    SSL::command prev = mSSL->getTop();
    if(prev.curInsertType != InsertType::None && prev.modeInsertType != InsertType::None) return;
    if(!DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
        mSSL->handleUndo();
        mSSL->pushStack(mSSL->getCommandUndo(),prev);
        mSSL->popStack(mSSL->getCommnadRedo());
    }
    float fraction = mSSL->getFraction();
    mSSL->setPause(true);
    if(prev.curDeleteType == DeleteType::Head || prev.modeDeleteType == DeleteType::Head){
        DeleteHeadProcess = true;
    }
    if(st.empty() && (DeleteTailProcess || DeleteValProcess) && framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction){
        framecntDel = 0;
    }
    if(!st.empty()) {
        if(DeleteTailProcess || prev.curDeleteType == DeleteType::Tail || prev.modeDeleteType == DeleteType::Tail){
        DeleteTailProcess = true;
        if(framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction){
            framecntDel = 0;
        }
        else if(framecntDel < (int)speedNode/(2*fraction)){
            shadedPos.node = currentAnimatingNode = st.top().first;
            shadedPos.pos = st.top().second;
            framecntDel = (int)speedNode/(2*fraction)+1;
            st.pop();
        }
    }
    if(DeleteValProcess || (prev.curDeleteType == DeleteType::Val && prev.modeDeleteType == DeleteType::Val)){
        DeleteValProcess = true;
        if(framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction){
            framecntDel = 0;
        }
        else if(framecntDel < (int)speedNode/(2*fraction)){
            shadedPos.node = currentAnimatingNode = st.top().first;
            shadedPos.pos = st.top().second;
            framecntDel = (int)speedNode/(2*fraction)+1;
            st.pop();
        }
    }
    }
}
void Delete::handleRedo(){
    if(DeleteHeadProcess){
        mSSL->setPause(false);
    }
    if(DeleteTailProcess)
    {
    if(currentAnimatingNode->next)
    {
    float fraction = mSSL->getFraction();
    mSSL->setPause(true);
    if(framecntDel < (int)speedNode/(2*fraction)){
        framecntDel = (int)speedNode/(2*fraction) + 1;
        curline = 2;
        curlinetmp = -1;
    }
    else if(framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction){
        prevpos = shadedPos.pos;
        nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel,st);
        curline = 3;
    }
    }
    else{
        mSSL->setPause(false);
    }
    }
    if(DeleteValProcess){
        if(currentAnimatingNode != mSSL->findList(std::stoi(textIn))){
            float fraction = mSSL->getFraction();
            mSSL->setPause(true);
            if(framecntDel < (int)speedNode/(2*fraction)){
                framecntDel = (int)speedNode/(2*fraction) + 1;
                curline = 2;
            }
            else if(framecntDel >= (int)speedNode/(2*fraction) && framecntDel < (int)speedNode/fraction){
                prevpos = shadedPos.pos;
                nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel,st);
                curline = 3;
                curlinetmp = 4;
            }
            }
            else{
                mSSL->setPause(false);
            }
    }
}
void Delete::draw(){
    if(mSSL->getToggle().isStepByStep) drawTextCode(curline,curlinetmp);
    else drawTextCode(-1,-1);
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
        if (!existVal && frameCounter < 30 && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess && cur == DeleteType::Val) {
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
    while(!st.empty()) st.pop();
    mSSL->clearStackUndo();
    mSSL->clearStackRedo();
}
void Delete::handle(){
    handleButtonsHover();
    IState::ToggleSwitch toggle = mSSL->getToggle();
    bool step = toggle.isStepByStep;
    static bool prevStep = toggle.isStepByStep;
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    DeleteType detectedMode = DetectCurrentMode();
    if (mousePressed && detectedMode != DeleteType::None && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
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
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250,buttonVar::buttonGo.rect.y = buttonVar::buttonF.rect.y;
    }
    if (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && mousePressed && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess) {
        mSSL->setState(mSSL->getClear());
        ResetDeleteState();
        buttonVar::buttonGo.rect.x = buttonVar::buttonClear.rect.x+120, buttonVar::buttonGo.rect.y = buttonVar::buttonClear.rect.y;
    }
    SSL::command command;
    if(!DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess){
        toggle.Update(mouse);
        mSSL->setToggle(toggle);
        if(step != toggle.isStepByStep) prevStep = step;
        step = toggle.isStepByStep;
    }
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9' && !DeleteValProcess) {
        textIn.push_back(static_cast<char>(key));
    }
    if (key == KEY_BACKSPACE && !textIn.empty() && !DeleteValProcess) {
        textIn.pop_back();
    }
    if(!textIn.empty()) tmpText = textIn;
    if (cur == DeleteType::Val && !textIn.empty() && !DeleteHeadProcess && !DeleteTailProcess && !DeleteValProcess && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed))) {
        if (!mSSL->getRoot()) {
            textIn.clear();
        }
        if(mSSL->getRoot() && !mSSL->findList(std::stoi(textIn))){
            shadedPos.pos = {0,0};
            curline = curlinetmp = -1;
            existVal = false;
            while(!st.empty()) st.pop();
        }
        else if (mSSL->getRoot() && mSSL->findList(std::stoi(textIn))!=mSSL->getRoot() && mSSL->findList(std::stoi(textIn))!=mSSL->getTail()) {
                DeleteValProcess = true;
                currentAnimatingNode = mSSL->getRoot();
                modeCur = DeleteType::Val;
                curline = curlinetmp = -1;
                idx = 0;    
            if(toggle.isStepByStep){
                shadedPos.node = nullptr;
                shadedPos.pos = startLinkedListPos;
                ListNode* cur = mSSL->getRoot();
                while(cur != mSSL->findList(std::stoi(textIn))){
                    idx++;
                    cur = cur->next;
                }   
                
                prevStep = toggle.isStepByStep;
            }
            else{
                shadedPos.node = mSSL->getRoot();
                shadedPos.pos = startLinkedListPos;
                while(currentAnimatingNode != mSSL->findList(std::stoi(textIn))){
                    idx++;
                    prevpos = shadedPos.pos;
                    nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel,st);
                    shadedPos.node = currentAnimatingNode;
                }   
            }
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Val,idx,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            while(!st.empty()) st.pop();
        }
        else if(mSSL->getRoot() && mSSL->findList(std::stoi(textIn))==mSSL->getRoot()){
            shadedPos.pos = {0,0};
            DeleteHeadProcess = true;
            ArrowLengthRender = EArrow.length;
            modeCur = DeleteType::Head;
            curline = curlinetmp = -1;
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Head,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            while(!st.empty()) st.pop();
            prevStep = toggle.isStepByStep;
        }
        else if(mSSL->getRoot() && mSSL->findList(std::stoi(textIn))==mSSL->getTail()){
            DeleteTailProcess = true;
            currentAnimatingNode = mSSL->getRoot();
            curline = curlinetmp = -1;
            if(toggle.isStepByStep){
                shadedPos.node = nullptr;
                shadedPos.pos = startLinkedListPos;
                modeCur = DeleteType::Tail;
                prevStep = toggle.isStepByStep;
            }
            else{
                shadedPos.node = mSSL->getRoot();
                shadedPos.pos = startLinkedListPos;
                while(currentAnimatingNode->next){
                    prevpos = shadedPos.pos; 
                    nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel,st);
                    shadedPos.node = currentAnimatingNode;
                }
            }
            command = {InsertType::None,InsertType::None,DeleteType::Val,DeleteType::Tail,1,std::stoi(textIn)};
            mSSL->pushStack(mSSL->getCommandUndo(),command);
            while(!st.empty()) st.pop();
        }
    }
    if(!DeleteHeadProcess && CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed && ((mSSL->getRoot() && cur == DeleteType::Head) || (mSSL->getRoot() && !mSSL->getRoot()->next))){
        shadedPos.pos = {0,0};
        DeleteHeadProcess = true;
        ArrowLengthRender = EArrow.length;
        curline = curlinetmp = -1;
        command = {InsertType::None,InsertType::None,DeleteType::Head,DeleteType::None,1,mSSL->getRoot()->data};
        mSSL->pushStack(mSSL->getCommandUndo(),command);
        if(!mSSL->getCommandUndo().empty()) std::cout << mSSL->getTop().val;
        else std::cout << "khong co";
        while(!st.empty()) st.pop();
        prevStep = toggle.isStepByStep;
        existVal = true;
    }
    if(!DeleteTailProcess && mSSL->getRoot() && mSSL->getRoot()->next && cur == DeleteType::Tail && CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed){
        DeleteTailProcess = true;
        currentAnimatingNode = mSSL->getRoot();
        curline = curlinetmp = -1;
        if(toggle.isStepByStep){
            shadedPos.node = nullptr;
            shadedPos.pos = startLinkedListPos;
            prevStep = toggle.isStepByStep;
            existVal = true;
        }
        else{
            shadedPos.node = mSSL->getRoot();
            shadedPos.pos = startLinkedListPos;
            while(currentAnimatingNode->next){
            prevpos = shadedPos.pos; 
            nodeNext(currentAnimatingNode,shadedPos.pos,framecntDel,st);
            shadedPos.node = currentAnimatingNode;
            }
        }
        command = {InsertType::None,InsertType::None,DeleteType::Tail,DeleteType::None,1,mSSL->getTail()->data};
        mSSL->pushStack(mSSL->getCommandUndo(),command);
        while(!st.empty()) st.pop();
    }
    SSL::command prev;
    if(!mSSL->getCommandUndo().empty()) prev = mSSL->getTop();
    if(UndoButton.isPressed() && prevStep && step && existVal && !mSSL->getCommandUndo().empty() && prev.curInsertType == InsertType::None && prev.modeInsertType == InsertType::None){ 
        textIn = tmpText;
        handleUndo();
    }
    if(RedoButton.isPressed() && (DeleteHeadProcess || DeleteTailProcess || DeleteValProcess)){
        handleRedo();
    }
}
// Find
Find::Find(SSL* s)
: mSSL(s), frameCounter(0), textIn(""), framecntFind(0), FindProcess(false), shadedPos({0,0}) {}
void Find::findAnimation(ListNode*& root) {
    float fraction = mSSL->getFraction();
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
    if(root == mSSL->getRoot() && framecntFind < (int)speedNode/(2*fraction)) curline = 1;
    if (shadedPos.pos.x + ARROW_LENGTH + NODE_SIZE > GetScreenWidth()-SCREEN_MARGIN && framecntFind >= (int)speedNode/(2*fraction) && framecntFind < (int)speedNode/fraction && root->next) {
        drawArrow2Node(shadedPos.pos,{ startLinkedListPos.x, shadedPos.pos.y + ROW_OFFSET },color::edgeRendered);
        curline = 2;
    } else if(shadedPos.pos.x + EArrow.length + NODE_SIZE < GetScreenWidth()-SCREEN_MARGIN && framecntFind >= (int)speedNode/(2*fraction) && framecntFind < (int)speedNode/fraction && root->next) {
        drawArrow(shadedPos.pos,{ shadedPos.pos.x+EArrow.length, shadedPos.pos.y},color::edgeRendered);
        curline = 2;
    }
    if (framecntFind >= (int)speedNode/fraction) {
        nodeNext(root,shadedPos.pos,framecntFind,st);
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
    float fraction = mSSL->getFraction();
    for(int i=0; i<codeLines.size(); i++){
        code[i].text = codeLines[i];
    }
    if(!mSSL->getRoot()) {
        curline = 0;
        curlinetmp = -1;
        return;
    }
    if(currentAnimatingNode && !currentAnimatingNode->next && framecntFind>0 && framecntFind<=(int)speedNode/(2*fraction)) {
        curline = 4;
        curlinetmp = 2;
    }
    if(curline == 4 && !currentAnimatingNode){
        curline = 5;
        curlinetmp = -1;
    }
}
void Find::handleUndo(){
    float fraction = mSSL->getFraction();
    mSSL->setPause(true);
    curline = -1, curlinetmp = -1;
    if(st.empty() && FindProcess && framecntFind >= (int)speedNode/(2*fraction) && framecntFind < (int)speedNode/fraction){
        framecntFind = 0;
    }
    if(!st.empty())
    {
        if(framecntFind >= (int)speedNode/(2*fraction) && framecntFind < (int)speedNode/fraction){
        framecntFind = 0;
    }
        else if(framecntFind < (int)speedNode/(2*fraction)){
        shadedPos.node = currentAnimatingNode = st.top().first;
        shadedPos.pos = st.top().second;
        framecntFind = (int)speedNode/(2*fraction)+1;
        st.pop();
    }
    }
}
void Find::handleRedo(){
    float fraction = mSSL->getFraction();
    mSSL->setPause(true);
    if(framecntFind < (int)speedNode/(2*fraction)){
        framecntFind = (int)speedNode/(2*fraction) + 1;
        curline = 2;
    }
    else if(framecntFind >= (int)speedNode/(2*fraction) && framecntFind < (int)speedNode/fraction){
        nodeNext(currentAnimatingNode,shadedPos.pos,framecntFind,st);
        curline = 3;
    }
}
void Find::draw() {
    drawButtons();
    if(mSSL->getToggle().isStepByStep) drawTextCode(curline,curlinetmp);
    else drawTextCode(-1,-1);
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
            shadedPos.pos = {0,0};
        }
        if(!FindProcess) drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL); 
        if (shadedPos.node && shadedPos.pos.x != 0 && shadedPos.pos.y != 0) {
            std::string str = std::to_string(shadedPos.node->data);
            drawNode(shadedPos.pos, str, NODE_SIZE,color::nodeRendered);
            drawTextDown("pointer",20,shadedPos.pos);
            if(frameCounter < 30 && !FindProcess && mSSL->getExistVal()){
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
    while(!st.empty()) st.pop();
}
void Find::handle(){
    IState::ToggleSwitch toggle = mSSL->getToggle();
    if(!FindProcess){
        toggle.Update(mouse);
        mSSL->setToggle(toggle);
    }
    if(!toggle.isStepByStep){
        while(!st.empty()) st.pop();
    }
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
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !FindProcess) {
        mSSL->setState(mSSL->getnotInMode());
        ResetFindState();
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getClear());
        ResetFindState();
        buttonVar::buttonGo.rect.x = buttonVar::buttonClear.rect.x+120, buttonVar::buttonGo.rect.y = buttonVar::buttonClear.rect.y;
    }
    int key = GetKeyPressed();
    if(key >= '0' && key <= '9' && !FindProcess) {
        textIn.push_back((char)key);
        while(!st.empty()) st.pop();
    }
    if(key == KEY_BACKSPACE && !textIn.empty() && !FindProcess) {
        textIn.pop_back();
    }
    if(!textIn.empty()) tmpText = textIn;
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if(!textIn.empty() && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed)) && !toggle.isStepByStep && mSSL->findList(std::stoi(textIn)) && !FindProcess){
        while(!st.empty()) st.pop();
        shadedPos.pos = startLinkedListPos;
        mSSL->setExistVal(mSSL->findList(std::stoi(textIn)));
        currentAnimatingNode = mSSL->getRoot();
        shadedPos.node = currentAnimatingNode;
        while (currentAnimatingNode != mSSL->getExistVal())
        {
            nodeNext(currentAnimatingNode,shadedPos.pos,framecntFind,st);
            shadedPos.node = currentAnimatingNode;
        }  
        textIn.clear(); 
        mSSL->setPause(false);      
    }  
    if(!textIn.empty() && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed)) && !toggle.isStepByStep && !mSSL->findList(std::stoi(textIn)) && !FindProcess){
        mSSL->setExistVal(mSSL->findList(std::stoi(textIn)));
        textIn.clear();
        mSSL->setPause(false);
    } 
    if(!textIn.empty() && ((key == KEY_ENTER) || (CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && mousePressed)) && toggle.isStepByStep && !FindProcess) {
        curline = 1;
        int x = std::stoi(textIn);
        mSSL->setExistVal(mSSL->findList(x));
        if(mSSL->getRoot()) {
            FindProcess = true;
            shadedPos.pos = startLinkedListPos;
            currentAnimatingNode = mSSL->getRoot();
            mSSL->setPause(false); 
    }
}
    if(UndoButton.isPressed()){
    FindProcess = true;
    textIn = tmpText;
    handleUndo();
}
    if(RedoButton.isPressed() && FindProcess){
    handleRedo();
}
}
//Clear
Clear::Clear(SSL* s) 
:mSSL(s), progress(0), clearProcess(false) {}
void Clear::ClearAnimation(){
    progress += deltaTime;
    float NodeRadiusRender = lerp(NODE_SIZE,0,progress);
    float FontSize = lerp(FontNode,0,progress);
    drawPos(node,NodeRadiusRender,FontSize);
    if(progress >= 1.0f){
        clearProcess = false;
        progress = 0.0f;
        node.clear();
        mSSL->delAllList();
    }
}
void Clear::draw(){
    drawTextCode(-1,-1);
    drawButtons();
    DrawButton(buttonVar::buttonGo.rect,buttonVar::buttonGo.text,buttonVar::buttonGo.buCol,SSLFont,22);
    if(clearProcess) ClearAnimation();
    else
    drawLinkedList(mSSL->getRoot(),startLinkedListPos,mSSL);
}
void Clear::handle(){
    handleButtonsHover();
    ToggleSwitch toggle = mSSL->getToggle();
    toggle.Update(GetMousePosition());
    mSSL->setToggle(toggle);
    if(CheckCollisionPointRec(mouse, buttonVar::buttonGo.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        handlePos(mSSL->getRoot(),startLinkedListPos,node);
        clearProcess = true;
        mSSL->setNumElement(0);
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonCreate.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !clearProcess) {
        mSSL->setState(mSSL->getCreate());
        mSSL->setExistVal(mSSL->getRoot());
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250;
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !clearProcess) {
        mSSL->setState(mSSL->getInsert());
        mSSL->setExistVal(mSSL->getRoot());
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250;
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !clearProcess) {
        mSSL->setState(mSSL->getDel());
        mSSL->setExistVal(mSSL->getRoot());
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250;
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !clearProcess) {
        mSSL->setState(mSSL->getFind());
        mSSL->setExistVal(mSSL->getRoot());
        buttonVar::buttonGo.rect.x = buttonVar::buttonF.rect.x+250, buttonVar::buttonGo.rect.y = buttonVar::buttonF.rect.y,60;
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !clearProcess) {
        mSSL->setState(mSSL->getnotInMode());
        mSSL->setExistVal(mSSL->getRoot());
    }
}