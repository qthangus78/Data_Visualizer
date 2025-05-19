#include "Heap.h"
#include <cmath>
#include <string.h>
#include "tinyfiledialogs.h"

//-----------------------
//BUTTON MANAGER
//-----------------------

void ButtonManager::DrawButton(Button &button){
    Vector2 textSize = MeasureTextEx ( customFont, button.txt.c_str(), fontSize, 2 );
    DrawRectangleRoundedLinesEx( button.rect, 0.2f, 0, 2.0f, BLACK);
    DrawRectangleRounded ( button.rect, 0.2f, 0, button.color );
    float x = button.rect.x + button.rect.width / 2 - textSize.x / 2;
    float y = button.rect.y + button.rect.height / 2 - fontSize / 2;
    DrawTextEx( customFont, button.txt.c_str(), { x, y }, fontSize, 2, WHITE );
}

void ButtonManager::Update(){
    HandleButtonsHover();

    if ( !CheckCollisionPointRec(GetMousePosition(), Stepbystep.rect))
        if ( isStepbystep )
            Stepbystep.color = YELLOW;
        else
            Stepbystep.color = BLUE;

    speedButton.Init({500, 90});
    speedButton.Update();
    duration = 2 / speedButton.GetValue();

    if ( isPaused ){
        PlayButton.SetPosition(912.5f - PlayButton.width / 2, 650);
        PauseButton.SetPosition(2000, 2000); 
    } else {
        PauseButton.SetPosition(912.5f - PauseButton.width / 2, 650);
        PlayButton.SetPosition(2000, 2000); 
    }

    UndoButton.SetPosition(912.5f - 50 - RedoButton.width, 650 + ( PlayButton.height - RedoButton.height ) / 2);

    RedoButton.SetPosition(912.5f + 50, 650 + ( PlayButton.height - RedoButton.height ) / 2);

}

void ButtonManager::DrawButtons(){
    DrawBackground();
    DrawButton(push);
    DrawButton(remove);
    DrawButton(clear);
    DrawButton(top);
    DrawButton(initialize);
    DrawButton(search);
    DrawButton(Stepbystep);
    UndoButton.Drawtexture();
    RedoButton.Drawtexture();
    isPaused ? PlayButton.Drawtexture() : PauseButton.Drawtexture();
    speedButton.Draw();
}

void ButtonManager::setInputBox ( float x, float y ){
    pos = {x, y};
}

void ButtonManager::setTxt ( std::string a ){
    txt = a;
}

void ButtonManager::HandleButtonsHover(){
    push.color = CheckCollisionPointRec(GetMousePosition(), push.rect) ? YELLOW : BLUE;
    remove.color = CheckCollisionPointRec(GetMousePosition(), remove.rect) ? YELLOW : BLUE;
    clear.color = CheckCollisionPointRec(GetMousePosition(), clear.rect) ? YELLOW : BLUE;
    top.color = CheckCollisionPointRec(GetMousePosition(), top.rect) ? YELLOW : BLUE;
    initialize.color = CheckCollisionPointRec(GetMousePosition(), initialize.rect) ? YELLOW : BLUE;
    search.color = CheckCollisionPointRec(GetMousePosition(), search.rect) ? YELLOW : BLUE;
    Stepbystep.color = CheckCollisionPointRec(GetMousePosition(), Stepbystep.rect) ? YELLOW : BLUE;
}

void ButtonManager::HandleButtonsClick(MinHeap* mHeap){
    if ( IsMouseButtonPressed( MOUSE_LEFT_BUTTON ) ){
        if ( CheckCollisionPointRec(GetMousePosition(), push.rect ) ){
            mHeap->setState(mHeap->getPush());
        }
        if ( CheckCollisionPointRec(GetMousePosition(), remove.rect ) ){
            mHeap->setState(mHeap->getRemove());
        }
        if ( CheckCollisionPointRec(GetMousePosition(), clear.rect ) ){
            mHeap->setState(mHeap->getClear());
        }
        if ( CheckCollisionPointRec(GetMousePosition(), top.rect) ){
            mHeap->setState(mHeap->getTop());
        }
        if ( CheckCollisionPointRec(GetMousePosition(), initialize.rect) ){
            mHeap->setState(mHeap->getInitialize());
        }
        if ( CheckCollisionPointRec(GetMousePosition(), search.rect) ){
            mHeap->setState(mHeap->getSearch());
        }
        if ( CheckCollisionPointRec(GetMousePosition(), random.rect) ){
            int temp = rand() % ( mHeap->mCurrent == mHeap->mInitialize ? 31 : 999 );
            strncpy(name, to_string(temp).c_str(), sizeof(name));
        }
        if ( CheckCollisionPointRec(GetMousePosition(), Stepbystep.rect) ){
            isStepbystep = !isStepbystep;
        }
    }
}

void ButtonManager::DrawLoadFile(){
    DrawRectangleRounded( loadFile, 0.2f, 0, ORANGE );
    DrawTextEx(customFont, "Load file", {loadFile.x + 10, loadFile.y + 10}, 30, 0, BLACK); 
}

void ButtonManager::DrawInputBox(){
    // Vẽ ô nhập
    DrawRectangleRounded( {pos.x + 210, pos.y, 200, 50}, 0.2f, 0, ORANGE );
    DrawTextEx(customFont, name, {pos.x + 220, pos.y + 10}, 30, 0, BLACK); 

    // Xử lý ô nhập
    int key = GetCharPressed();
    while ( key > 0 ){
        if ((key >= '0') && (key <= '9') && (letterCount < 3)){
            name[letterCount] = (char)key;
            name[letterCount+1] = '\0';
            letterCount++;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)){
        letterCount--;
        if ( letterCount < 0 )
            letterCount = 0;
        name[letterCount] = '\0';
    }

    // Vẽ đường nhấp nháy khi nhập 
    blinkTime += GetFrameTime();
    if ((int)(blinkTime * 2) % 2 == 0) { // Blink every 0.5 seconds
        Vector2 textSize = MeasureTextEx( customFont, name, fontSize, 0 );
        int cursorX = pos.x + 10 + textSize.x + 210;
        DrawLine(cursorX, pos.y + 10, cursorX, pos.y + 40, BLACK); // Vertical blinking line
    }

    // Vẽ nút ngẫu nhiên
    pos.x += 210 + ( 200 - dice.width * 0.25 - ( 50 - dice.height * 0.25 ) / 2 );
    pos.y = pos.y + ( 50 - dice.height * 0.25 ) / 2;

    random.rect = { pos.x, pos.y, dice.height * 0.25f, dice.height * 0.25f };
    DrawTextureEx(dice, {pos.x, pos.y}, 0.0f, 0.25f, WHITE);
}

void ButtonManager::DrawBlinkingText(){
    txtBlinkTime += GetFrameTime();
    if((int)(txtBlinkTime * 2) % 2 == 0){
        DrawTextEx( customFont, txt.c_str(), {pos.x + 210, pos.y - 30}, fontSize, 2, RED);
    }
}

void ButtonManager::DrawBackground(){
    Rectangle rect = { 5, 60, 420, screenHeight - 65 };
    DrawRectangleRounded( rect, 0.05f, 0, Fade(BEIGE, 0.5f) );
    Rectangle rect2 = { 430, 60, screenWidth - 435, screenHeight - 65 };
    DrawRectangleRounded( rect2, 0.05f, 0, Fade(BEIGE, 0.5f) );
    Rectangle rect3 = { 430, 600, screenWidth - 435, screenHeight - 605 };
    DrawRectangleRounded( rect3, 0.05f, 0, BEIGE );
}

//-----------------------
//PUSH
//-----------------------

State::State(){
    tree = std::vector<int>(0);
    heapNode = std::vector<HeapNode>(0);
    originHeapNode = std::vector<HeapNode>(0);
    targetHeapNode = std::vector<HeapNode>(0);
}

State::~State(){
    tree.clear();
    heapNode.clear();
    originHeapNode.clear();
    targetHeapNode.clear();
}

void Push::getState(State &state){
    beginLine = state.beginLine;
    endLine = state.endLine;

    mHeap->tree = state.tree;
    heapNode = state.heapNode;
    originHeapNode = state.originHeapNode;
    targetHeapNode = state.targetHeapNode;
    animatingIdx = state.animatingIdx;
    parentIdx = state.parentIdx;

    animatingPos = state.animatingPos;
    targetPos = state.targetPos;
    originPos = state.originPos;

    animatingPos2 = state.animatingPos2;
    targetPos2 = state.targetPos2;
    originPos2 = state.originPos2;

    currentStep = state.currentStep;
    isAnimating = state.isAnimating;

    blinkTime1 = 0.0f;
    blinkTime2 = 0.0f;
    for ( int i = 0; i < elapsedTime.size(); i++ ){
        elapsedTime[i] = 0.0f;
    }
}

void Push::saveState(){
    State state;
    state.beginLine = beginLine;
    state.endLine = endLine;
    state.tree = mHeap->tree;
    state.heapNode = heapNode;
    state.originHeapNode = currentStep == 0 ? targetHeapNode : originHeapNode;
    state.targetHeapNode = targetHeapNode;
    state.animatingIdx = animatingIdx;
    state.parentIdx = parentIdx;
    state.animatingPos = animatingPos;
    state.targetPos = targetPos;
    state.originPos = originPos;
    state.animatingPos2 = animatingPos2;
    state.targetPos2 = targetPos2;
    state.originPos2 = originPos2;
    state.currentStep = currentStep;
    state.isAnimating = !( currentStep == -1 );
    if ( currentStep == 4 )
        std::swap(state.animatingPos, state.animatingPos2);
    undoStack.push(state);
}

void Push::handleUndo(){
    if (undoStack.empty()) return;
    State prevState = undoStack.top();
    undoStack.pop();
    redoStack.push(prevState);
    getState(prevState);
}

void Push::handleRedo(){
    if(redoStack.empty()) return;
    State nextState = redoStack.top();
    redoStack.pop();
    undoStack.push(nextState);
    getState(nextState);
}

Push::Push(MinHeap* heap){ mHeap = heap; }

void setPseudoCode(char* title, std::vector<char*> content, int beginLine = -1, int endLine = -1){
    pseudoCode.SetTitle(title);
    pseudoCode.SetContent(content);
    pseudoCode.SetHighlightLines(beginLine, endLine);
}

void Push::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox();

    pseudoCode.Draw();

    DrawPartOfHeap(mHeap, animatingIdx, parentIdx, isAnimating, currentStep);

    switch ( currentStep ){
        case 1:
            DrawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]));
            break;
        case 2:
            if ( !isPaused )
                drawBlinkingNode( duration );
            if ( isUndoing || isRedoing )
                drawBlinkingNode( 3600.0f );
            break;
        case 4:
            DrawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]));
            DrawNode(animatingPos2, std::to_string(mHeap->tree[parentIdx]) );
            break;
        default:
            break;
    }
}

void Push::update(){
    buttons.setInputBox(buttons.push.rect.x, buttons.push.rect.y);
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        val = atoi(buttons.name); 

        clearState(undoStack, redoStack);

        isAnimating = false;
        saveState();
        handleInsert(val);

        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }

    if ( isPaused && PlayButton.isPressed()){
        isRedoing = false;
        isUndoing = false;
        isPaused = !isPaused;

        if ( isStepbystep ){
            if ( currentStep == 1 ){
                saveState();
                isAnimating = true;
                currentStep = 2;
            }
            else if (currentStep == 2) {
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                isAnimating = true;
                currentStep = 3;
            }
            else if (currentStep == 4) {
                saveState();
                isAnimating = true;
                currentStep = 2;
            }
        }
    }
    if ( !isPaused && PauseButton.isPressed()){
        isPaused = !isPaused;
    }
    if ( UndoButton.isPressed() ){
        isUndoing = true;
        isRedoing = false;
        isPaused = true;
        handleUndo();
    }
    if ( RedoButton.isPressed() ){
        isRedoing = true;
        isUndoing = false;
        isPaused = true;
        handleRedo();
    }

    setPseudoCode(yes.titleP, yes.contentP, beginLine, endLine);

    if ( isPaused ) return;

    switch(currentStep){
        case 0:
            updateTreeStructure();
            break;
        case 1:
            updateInsert();
            break;
        case 3:
            handleBubbleUp();
            break;
        case 4:
            updateBubbleUp();
            break;
        default:
            break;
    }
}

void Push::reset(){
    blinkTime = 0.0f;
    if ( !undoStack.empty() ){
        while ( undoStack.size() > 1 )
            undoStack.pop();
        State originState = undoStack.top();
        getState(originState);
        mHeap->push(val);
        recalculateAllNodePos(mHeap);
    }
    clearState(undoStack, redoStack);
}

// Tiền xử lý cấu trúc cây, vị trí đưa nút từ vị trí ngẫu nhiên vào cây 
void Push::handleInsert(int val){
    // Thêm giá trị vào cây HeapHeap
    mHeap->tree.push_back(val);
    animatingIdx = mHeap->size() - 1;
    
    // Tính toán vị trí mục tiêu
    int x = animatingIdx - (1 << (int)log2(animatingIdx + 1)) + 1;
    int y = (int)log2(animatingIdx + 1);
    int height = (int)log2(mHeap->size()) + 1;
    targetPos = calculateNodePos({(float)x, (float)y}, height);

    // Khởi tạo vị trí ban đầu
    animatingPos = { screenWidth / 2, 600 };
    originPos = animatingPos;
    heapNode[animatingIdx] = {val, targetPos, true};
    
    // Cập nhật vị trí các nút nếu cấu trúc cây thay đổi
    if ( mHeap->size() >= 4 && mHeap->size() == std::pow(2, height - 1)){
        int n = heapNode.size();
        for ( int i = 0; i < n; i++ ){
            originHeapNode[i] = heapNode[i];
        }
        recalculateAllNodePos ( mHeap );
        for ( int i = 0; i < n; i++ ){
            targetHeapNode[i] = heapNode[i];
        }
        currentStep = 0;
        isAnimating = true;
    }
    else{ 
        currentStep = 1;
        isAnimating = true;
    }
}

// Step 0: Cập nhật vị trí các nút trong cây
void Push::updateTreeStructure(){
    bool allFinished = true;
    int n = heapNode.size();
    for ( int i = 0; i < n; i++ ){
        bool nodeAnimating = true;
        updateNodePos( heapNode[i].pos, targetHeapNode[i].pos, originHeapNode[i].pos, duration, nodeAnimating, i );
        if ( nodeAnimating ) {
            allFinished = false;
        }
    }
    if ( allFinished ){
        saveState();
        currentStep = 1;
    }
}

// Step 1: Cập nhật vị trí nút đang thêm vào
void Push::updateInsert(){
    beginLine = 0;
    endLine = 1;
    updateNodePos ( animatingPos, targetPos, originPos, duration, isAnimating );
    
    if ( !isAnimating ){
        if ( isStepbystep ){
            isPaused = true;
        }
        else {
            saveState();
            isAnimating = true;
            currentStep = 2;
        }
    }
}

// Step 2: Vẽ nhấp nháy nút đang thêm vào và nút cha của nó
void Push::drawBlinkingNode( float duration ){

    DrawBlinkingNode(heapNode[animatingIdx].pos, heapNode[animatingIdx].val, blinkTime1);
    parentIdx = mHeap->parent(animatingIdx);
    if ( parentIdx >= 0 ){
        DrawBlinkingNode(heapNode[parentIdx].pos, heapNode[parentIdx].val, blinkTime2);
    }
    if ( animatingIdx == 0 ){
        beginLine = 2;
        endLine = 2;
    }
    else{
        beginLine = 4;
        endLine = 4;
    }

    if ( blinkTime1 > duration ){
        if ( isStepbystep )
            isPaused = true;
        else{
            saveState();
            blinkTime1 = 0.0f;
            blinkTime2 = 0.0f;
            isAnimating = false;
            currentStep = 3;
        }
    }
}

// Step 3: Kiểm tra xem có cần hoán đổi vị trí với nút cha không
void Push::handleBubbleUp(){
    if ( parentIdx >= 0 && heapNode[parentIdx].val > heapNode[animatingIdx].val){
        originPos = animatingPos;
        targetPos = heapNode[parentIdx].pos;
        
        targetPos2 = heapNode[animatingIdx].pos;
        animatingPos2 = targetPos;
        originPos2 = animatingPos2;
        
        std::swap(mHeap->tree[animatingIdx], mHeap->tree[parentIdx]);
        
        swapHeapNode(heapNode[animatingIdx], heapNode[parentIdx]);
        
        std::swap(animatingIdx, parentIdx);

        isAnimating = true;
        currentStep = 4;
    }
    else {
        animatingIdx = -1;
        parentIdx = -1;
        isAnimating = false;
        currentStep = -1;
        if ( parentIdx < 0 ){
            beginLine = -1;
            endLine = -1;
        }
        else{
            beginLine = 8;
            endLine = 8;
        }
        recalculateAllNodePos ( mHeap );
    }
}

// Step 4: Cập nhật vị trí nút đang thêm vào và nút cha của nó
void Push::updateBubbleUp(){
    beginLine = 5;
    endLine = 6;
    updateNodePos ( animatingPos, targetPos, originPos, duration, isAnimating );
    
    updateNodePos ( animatingPos2, targetPos2, originPos2, duration, isAnimating );
    
    if ( !isAnimating ){
        animatingPos = targetPos;
        animatingPos2 = targetPos2;
        if ( isStepbystep ){
            isPaused = true;
        }
        else {
            saveState();
            isAnimating = true;
            currentStep = 2;
        }
    }
}

//-----------------------
//REMOVE
//-----------------------
Remove::Remove(MinHeap* heap) { mHeap = heap; }

void Remove::saveState(){
    State state;
    state.blinkingStep = blinkingStep;
    state.beginLine = beginLine;
    state.endLine = endLine;
    state.tree = mHeap->tree;
    state.heapNode = heapNode;
    state.originHeapNode = originHeapNode;
    state.targetHeapNode = targetHeapNode;
    state.animatingIdx = animatingIdx;
    state.parentIdx = childIdx;
    state.animatingPos = animatingPos;
    state.targetPos = targetPos;
    state.originPos = originPos;
    state.animatingPos2 = animatingPos2;
    state.targetPos2 = targetPos2;
    state.originPos2 = originPos2;
    state.currentStep = currentStep;
    state.isAnimating = ( currentStep != -1 ); // 0 1 2 4 
    if ( currentStep == 4 )
        std::swap(state.animatingPos, state.animatingPos2);
    undoStack.push(state);
}

void Remove::getState(State &state){
    blinkingStep = state.blinkingStep;
    beginLine = state.beginLine;
    endLine = state.endLine;

    mHeap->tree = state.tree;
    heapNode = state.heapNode;
    originHeapNode = state.originHeapNode;
    targetHeapNode = state.targetHeapNode;
    animatingIdx = state.animatingIdx;
    childIdx = state.parentIdx;

    animatingPos = state.animatingPos;
    targetPos = state.targetPos;
    originPos = state.originPos;

    animatingPos2 = state.animatingPos2;
    targetPos2 = state.targetPos2;
    originPos2 = state.originPos2;

    currentStep = state.currentStep;
    isAnimating = state.isAnimating;

    blinkTime1 = 0.0f;
    blinkTime2 = 0.0f;
    for ( int i = 0; i < elapsedTime.size(); i++ ){
        elapsedTime[i] = 0.0f;
    }
}

void Remove::handleUndo(){
    if (undoStack.empty()) return;
    State prevState = undoStack.top();
    undoStack.pop();
    redoStack.push(prevState);
    getState(prevState);
}

void Remove::handleRedo(){
    if(redoStack.empty()) return;
    State nextState = redoStack.top();
    redoStack.pop();
    undoStack.push(nextState);
    getState(nextState);
}

void Remove::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox();
    pseudoCode.Draw();

    DrawPartOfHeap(mHeap, animatingIdx, childIdx, isAnimating, currentStep);
    if ( currentStep == 0 || currentStep == 4 )
        DrawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]));
    if ( currentStep == 2 ){
        if ( !isPaused )
            drawBlinkingNode( duration );
        if ( isUndoing || isRedoing )
            drawBlinkingNode( 3600.0f );
    }
    if ( currentStep == 4 )
        DrawNode(animatingPos2, std::to_string(mHeap->tree[childIdx]));

    // Tìm không thấy hoặc cây trống
    if (buttons.notFound) {
        std::string txt;
        if ( mHeap->tree.empty() )
            buttons.setTxt ( "Empty tree!" );
        else
            buttons.setTxt ( "Value not found!" );
        buttons.setInputBox(buttons.remove.rect.x, buttons.remove.rect.y);
        buttons.DrawBlinkingText();
    }
        
}
    
void Remove::update(){  
    buttons.setInputBox(buttons.remove.rect.x, buttons.remove.rect.y);
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);
    if ( IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0' ){
        val = atoi(buttons.name);
        animatingIdx = mHeap->search(val);
        
        clearState(undoStack, redoStack);

        if ( animatingIdx != -1 ){
            saveState();
            handleRemove(animatingIdx);
            buttons.notFound = false;
        }
        else {
            buttons.notFound = true;
        }
        
        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }
    
    if ( isPaused && PlayButton.isPressed()){
        isRedoing = false;
        isUndoing = false;
        isPaused = !isPaused;
        
        if ( currentStep == 0 ){
            saveState();
            currentStep = 2; 
            blinkingStep = 0;
            isAnimating = false;
        }
        
        if ( currentStep == 1 ){
            saveState();
            isAnimating = true;
            currentStep = 2;
            blinkingStep = 0;
            std::swap(tempIdx, animatingIdx);
        }
        
        else if ( currentStep == 2 ){
            if ( blinkingStep == 0 ){
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                blinkingStep = childIdx != animatingIdx ? 1 : 2;
            }
            else if ( blinkingStep == 1 ){
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                isAnimating = false;
                currentStep = 3;
            }
            else if ( blinkingStep == 2 ){
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                blinkingStep = -1;
                isAnimating = false;
                currentStep = 3;
            }
            
        }
        
        else if ( currentStep == 4 ){
            saveState();
            isAnimating = true;
            blinkingStep = 0;
            currentStep = 2;
        }
        
        
    }
    if ( !isPaused && PauseButton.isPressed()){
        isPaused = !isPaused;
    }
    if ( UndoButton.isPressed() ){
        isUndoing = true;
        isRedoing = false;
        isPaused = true;
        handleUndo();
    }
    if ( RedoButton.isPressed() ){
        isRedoing = true;
        isUndoing = false;
        isPaused = true;
        handleRedo();
    }
    
    setPseudoCode(yes.titleR, yes.contentR, beginLine, endLine);
        
    if ( isPaused ) return;
        
    switch ( currentStep ){
        case 0:
            updateRemove();
            break;
        case 1:
            updateTreeStructure();
            break;
        case 2:
            break;
        case 3:
            handleBubbleDown();
            break;
        case 4:
            updateBubbleDown();
            break;
        default:
            break;
    }
}

void Remove::reset(){
    buttons.notFound = false;
    blinkTime = 0.0f;
    txtBlinkTime = 0.0f;
    // if ( !undoStack.empty() ){
    //     while ( undoStack.size() > 1 )
    //         undoStack.pop();
    //     State originState = undoStack.top();
    //     getState(originState);
    //     mHeap->remove(val);
    //     recalculateAllNodePos(mHeap);
    // }
    clearState(undoStack, redoStack);
}

// Tiền xử lý nội tại trong cây 
void Remove::handleRemove(int idx){
    int i = mHeap->size() - 1;
    animatingIdx = idx;

    animatingPos = heapNode[i].pos;
    targetPos = heapNode[idx].pos;
    originPos = animatingPos;

    std::swap(mHeap->tree[idx], mHeap->tree[i]);

    swapHeapNode(heapNode[idx], heapNode[i]);

    heapNode[i] = {0, {0, 0}, false};
    mHeap->tree.pop_back();

    currentStep = 0;
    isAnimating = true;
}

// STEP 0: Thay đổi vị trí rễ cây và nút cần xóaxóa
void Remove::updateRemove(){
    beginLine = 0;
    endLine = 0;
    updateNodePos ( animatingPos, targetPos, originPos, duration, isAnimating );
    
    if ( !isAnimating ){
        saveState();
        // Cập nhật vị trí các nút nếu cấu trúc cây thay đổi
        int height = (int)log2(mHeap->size()) + 1;
        if ( mHeap->size() >= 3 && mHeap->size() == std::pow(2, height) - 1){
            int n = heapNode.size();
            for ( int i = 0; i < n; i++ ){
                originHeapNode[i] = heapNode[i];
            }
            recalculateAllNodePos ( mHeap );
            for ( int i = 0; i < n; i++ ){
                targetHeapNode[i] = heapNode[i];
            }
            currentStep = 1;
            isAnimating = true;
            std::swap(tempIdx, animatingIdx);
        }
        else{ 
            if ( isStepbystep )
                isPaused = true;
            else {
                currentStep = 2; 
                blinkingStep = 0;
                isAnimating = true;
            }
        }
    }
}

// STEP 1: Cập nhật cấu trúc cây
void Remove::updateTreeStructure(){
    beginLine = -1;
    endLine = -1;
    bool allFinished = true;
    int n = heapNode.size();
    for ( int i = 0; i < n; i++ ){
        bool nodeAnimating = true;
        updateNodePos( heapNode[i].pos, targetHeapNode[i].pos, originHeapNode[i].pos, duration, nodeAnimating, i );
        if ( nodeAnimating ) {
            allFinished = false;
        }
    }
    if ( allFinished ){
        if ( isStepbystep ){
            isPaused = true;
        }
        else {
            saveState();
            currentStep = 2;
            isAnimating = true;
            blinkingStep = 0;
            std::swap(tempIdx, animatingIdx);
        }
    }
}

// STEP 2: Vẽ nút nhấp nháy 
void Remove::drawBlinkingNode(float duration){
    int n = mHeap->size();
    int l = mHeap->left(animatingIdx);
    int r = mHeap->right(animatingIdx);
    childIdx = animatingIdx;
    if ( l < n && heapNode[l].val < heapNode[childIdx].val )
        childIdx = l;
    if ( r < n && heapNode[r].val < heapNode[childIdx].val )
        childIdx = r;
    
    if ( blinkingStep == 0 ){
        if ( l < n ){
            beginLine = endLine = 3;
            DrawBlinkingNode(heapNode[l].pos, heapNode[l].val, blinkTime1);
        }
        if ( r < n ){
            beginLine = endLine = 6;
            DrawBlinkingNode(heapNode[r].pos, heapNode[r].val, blinkTime2);
        }
        if ( l < n && r < n ){
            beginLine = 3;
            endLine = 6;
        }
        if ( l >= n && r >= n ){
            beginLine = endLine = -1;
            currentStep = 3;
        }
        if ( blinkTime1 > duration || blinkTime2 > duration ){
            if ( isStepbystep ){
                isPaused = true;
            }
            else {
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                blinkingStep = childIdx != animatingIdx ? 1 : 2;
            }
        }
    }
    else if ( blinkingStep == 1 ){
        if ( childIdx == l ){
            beginLine = 4;
            endLine = 4;
        }
        else if ( childIdx == r ){
            beginLine = 6;
            endLine = 6;
        }
        DrawBlinkingNode(heapNode[animatingIdx].pos, heapNode[animatingIdx].val, blinkTime1);
        DrawBlinkingNode(heapNode[childIdx].pos, heapNode[childIdx].val, blinkTime2);
        if ( blinkTime1 > duration ){
            if ( isStepbystep )
                isPaused = true;
            else {
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                isAnimating = false;
                currentStep = 3;
            }
        }
    }
    else if ( blinkingStep == 2 ){
        beginLine = 7;
        endLine = 7;
        DrawBlinkingNode(heapNode[animatingIdx].pos, heapNode[animatingIdx].val, blinkTime1);
        if ( blinkTime1 > duration ){
            if ( isStepbystep )
                isPaused = true;
            else{
                saveState();
                blinkTime1 = 0.0f;
                blinkTime2 = 0.0f;
                blinkingStep = -1;
                isAnimating = false;
                currentStep = 3;
            }
        }
    }
}

// STEP 3
void Remove::handleBubbleDown(){
    if ( childIdx < mHeap->size() && childIdx != animatingIdx ){

        targetPos = heapNode[childIdx].pos;
        animatingPos = heapNode[animatingIdx].pos;
        
        animatingPos2 = targetPos;
        targetPos2 = animatingPos;

        originPos = animatingPos;
        originPos2 = animatingPos2;

        isAnimating = true;
        currentStep = 4;

        // Cập nhật giá trị trong mảng
        std::swap(mHeap->tree[animatingIdx], mHeap->tree[childIdx]);

        // Cập nhật vị trí trong mảng 
        swapHeapNode(heapNode[animatingIdx], heapNode[childIdx]);

        std::swap(animatingIdx, childIdx);
    }
    else {
        beginLine = -1;
        endLine = -1;
        animatingIdx = -1;
        childIdx = -1;
        isAnimating = false;
        currentStep = -1;
    }
}

// STEP 4
void Remove::updateBubbleDown(){
    beginLine = 8;
    endLine = 9;
    // Cập nhật vị trí nút con
    updateNodePos(animatingPos2, targetPos2, originPos2, duration, isAnimating );
    // Cập nhật vị trí nút cha
    updateNodePos(animatingPos, targetPos, originPos, duration, isAnimating ); 

    if ( !isAnimating ){
        if ( isStepbystep ){
            isPaused = true;
        }
        else {
            saveState();
            blinkingStep = 0;
            currentStep = 2;
            isAnimating = true;
        }
    }
}

//-----------------------
//CLEAR
//-----------------------
ClearH::ClearH(MinHeap* heap){ mHeap = heap; }

void ClearH::draw(){
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
    pseudoCode.Draw();
}

void ClearH::update(){
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);
    mHeap->clear();
    for ( int i = 0; i < 31; i++ ){
        heapNode[i].val = 0;
        heapNode[i].exist = false;
        heapNode[i].pos = {0, 0};
    }

    setPseudoCode(yes.titleC, yes.contentC);
}

void ClearH::reset(){
    blinkTime = 0.0f;
}

//-----------------------
//TOP
//-----------------------
Top::Top(MinHeap* heap){ mHeap = heap; }

void Top::draw(){
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
    if ( mHeap->tree.empty() ){
        buttons.setTxt( "Empty tree!" );
        buttons.setInputBox(buttons.top.rect.x, buttons.top.rect.y + 40);
        buttons.DrawBlinkingText();
    }
    else
        DrawBlinkingNode(heapNode[0].pos, mHeap->top(), blinkTime);
    pseudoCode.Draw();
}

void Top::update(){
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);

    setPseudoCode(yes.titleT, yes.contentT);
}

void Top::reset(){
    blinkTime = 0.0f;
    txtBlinkTime = 0.0f;
}

//-----------------------
//INITIALIZE
//-----------------------
Initialize::Initialize(MinHeap* heap){ mHeap = heap; }

void Initialize::handleInputFile(){
    const char* filePath = tinyfd_openFileDialog("Open File", NULL, 0, NULL, NULL, 0);
    if (filePath){
        std::ifstream fin(filePath);
        if (fin.is_open()){
            mHeap->tree.clear();
            for ( int i = 0; i < 31; i++ ){
                heapNode[i].val = 0;
                heapNode[i].exist = false;
                heapNode[i].pos = {0, 0};
            }
            int value;
            while (fin >> value){
                mHeap->push(value);
            }
            fin.close();
        }
        filePath = nullptr;
    }
}

void Initialize::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox();
    buttons.DrawLoadFile();
    drawHeap(mHeap->tree);
    pseudoCode.Draw();
}

void Initialize::update(){
    buttons.setInputBox(buttons.initialize.rect.x, buttons.initialize.rect.y);
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        mHeap->tree.clear();
        for ( int i = 0; i < 31; i++ ){
            heapNode[i].val = 0;
            heapNode[i].exist = false;
            heapNode[i].pos = {0, 0};
        }
        int value = atoi(buttons.name); 
        for ( int i = 0; i < value; i++ ){
            int temp = rand() % 999;
            mHeap->push(temp);
        }
        buttons.name[0] = '\0';
        buttons.letterCount = 0;
        recalculateAllNodePos(mHeap);
    }
    if ( CheckCollisionPointRec(GetMousePosition(), buttons.loadFile) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        handleInputFile();
        recalculateAllNodePos(mHeap);
    }
    setPseudoCode(yes.titleI, yes.contentI);
}

void Initialize::reset(){
    blinkTime = 0.0f;
}

//-----------------------
//SEARCH
//-----------------------
Search::Search(MinHeap* heap){ mHeap = heap; }

void Search::saveState(){
    State state;
    state.beginLine = beginLine;
    state.endLine = endLine;
    state.currentStep = currentStep;
    state.animatingIdx = animatingIdx;
    state.parentIdx = targetIdx;
    undoStack.push(state);
}

void Search::getState(State &state){
    beginLine = state.beginLine;
    endLine = state.endLine;
    currentStep = state.currentStep;
    animatingIdx = state.animatingIdx;
    targetIdx = state.parentIdx;
}

void Search::handleRedo(){
    if(redoStack.empty()) return;
    State nextState = redoStack.top();
    redoStack.pop();
    undoStack.push(nextState);
    getState(nextState);
}

void Search::handleUndo(){
    if (undoStack.empty()) return;
    State prevState = undoStack.top();
    undoStack.pop();
    redoStack.push(prevState);
    getState(prevState);
}

void Search::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox();
    drawHeap(mHeap->tree);
    pseudoCode.Draw();

    switch ( currentStep ){
        case 0:
            if ( targetIdx == -1 ){
                targetIdx = mHeap->size() - 1;
            }

            if ( animatingIdx <= targetIdx ){
                DrawBlinkingNode(heapNode[animatingIdx].pos, heapNode[animatingIdx].val, blinkTime);
                
                if ( isPaused )
                    return;

                beginLine = endLine = ( blinkTime > duration / 2 ) ? 1 : 0;

                if ( blinkTime > duration ){
                    if ( isStepbystep )
                        isPaused = true;
                    else {
                        saveState();
                        // std::cout << "State " << animatingIdx << " saved" << std::endl;
                        blinkTime = 0.0f;
                        animatingIdx++;
                    }
                }
                
            }
            else {
                currentStep = 1;
                animatingIdx = targetIdx;
            }
            break;
        case 1:
            if ( valueFound ){
                buttons.setTxt ( "Value found!" );
                buttons.DrawBlinkingText();
                DrawBlinkingNode(heapNode[animatingIdx].pos, heapNode[animatingIdx].val, blinkTime);
                beginLine = 2;
                endLine = 2;
            }
            else{
                buttons.setTxt(( mHeap->tree.empty() ) ? "Empty tree!" : "Value not found!");
                buttons.DrawBlinkingText();
                beginLine = 3;
                endLine = 3;
            }
            break;
        default:
            beginLine = -1;
            endLine = -1;
            break;
        }
    }
    
void Search::update(){
    buttons.setInputBox(buttons.search.rect.x, buttons.search.rect.y);
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        int value = atoi(buttons.name); 
        targetIdx = mHeap->search(value);
        
        valueFound = ( targetIdx != -1 );
        animatingIdx = 0;
        currentStep = 0;
        blinkTime = 0.0f;
        
        buttons.name[0] = '\0';
        buttons.letterCount = 0;

        clearState(undoStack, redoStack);
    }

    if ( isPaused && PlayButton.isPressed()){
        isRedoing = false;
        isUndoing = false;
        isPaused = !isPaused;

        if ( isStepbystep ){
            if ( currentStep == 0 ){
                saveState();
                blinkTime = 0.0f;
                animatingIdx++;
            }
        }
    }
    if ( !isPaused && PauseButton.isPressed()){
        isPaused = !isPaused;
    }
    if ( UndoButton.isPressed() ){
        isUndoing = true;
        isRedoing = false;
        isPaused = true;
        handleUndo();
    }
    if ( RedoButton.isPressed() ){
        isRedoing = true;
        isUndoing = false;
        isPaused = true;
        handleRedo();
    }

    setPseudoCode(yes.titleS, yes.contentS, beginLine, endLine);
}

void Search::reset(){
    valueFound = false;
    buttons.notFound = false;
    currentStep = -1;
}

//-----------------------
//WAITING
//-----------------------
Waiting::Waiting(MinHeap* heap){ mHeap = heap; }  

void Waiting::draw(){
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
}

void Waiting::update(){
    buttons.Update();
    buttons.HandleButtonsClick(mHeap);
    if ( isPaused && PlayButton.isPressed()){
        isPaused = !isPaused;
    }
    if ( !isPaused && PauseButton.isPressed()){
        isPaused = !isPaused;
    }
}

void Waiting::reset(){

}

void MinHeap::draw(){ if ( mCurrent ) mCurrent->draw(); }
void MinHeap::update(){ if ( mCurrent ) mCurrent->update(); }
void MinHeap::reset(){ if ( mCurrent ) mCurrent->reset(); }
