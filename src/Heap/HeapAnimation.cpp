#include "Heap.h"
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

void ButtonManager::DrawButtons(){
    // PUSH
    DrawButton(push);
    // REMOVE
    DrawButton(remove);
    // CLEAR
    DrawButton(clear);
    // TOP
    DrawButton(top);
    // INITIALIZE
    DrawButton(initialize);
    // PLAY
    // PAUSE
    
    if ( isPaused ){
        PlayButton.SetPosition(912.5f - PlayButton.width / 2, 650);
        PlayButton.Drawtexture();
        PauseButton.SetPosition(-1, -1); 
    } else {
        PauseButton.SetPosition(912.5f - PauseButton.width / 2, 650);
        PauseButton.Drawtexture();
        PlayButton.SetPosition(-1, -1);
    }

    UndoButton.SetPosition(912.5f - 50 - RedoButton.width, 650 + ( PlayButton.height - RedoButton.height ) / 2);
    UndoButton.Drawtexture();

    RedoButton.SetPosition(912.5f + 50, 650 + ( PlayButton.height - RedoButton.height ) / 2);
    RedoButton.Drawtexture();
    if ( RedoButton.CheckMouseCollision() && RedoButton.isPressed()){
        std::cout << "Redo button pressed!" << std::endl;
    }
}

void ButtonManager::HandleButtonsHover(){
    push.color = CheckCollisionPointRec(GetMousePosition(), push.rect) ? YELLOW : BLUE;
    remove.color = CheckCollisionPointRec(GetMousePosition(), remove.rect) ? YELLOW : BLUE;
    clear.color = CheckCollisionPointRec(GetMousePosition(), clear.rect) ? YELLOW : BLUE;
    top.color = CheckCollisionPointRec(GetMousePosition(), top.rect) ? YELLOW : BLUE;
    initialize.color = CheckCollisionPointRec(GetMousePosition(), initialize.rect) ? YELLOW : BLUE;
}

void ButtonManager::HandleButtonsClick(MinHeap* mHeap){
    if ( CheckCollisionPointRec(GetMousePosition(), push.rect ) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON ) ){
        mHeap->setState(mHeap->getPush());
        notFound = false;
        blinkTime = 0.0f;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), remove.rect ) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getRemove());
        blinkTime = 0.0f;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), clear.rect ) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getClear());
        notFound = false;
        blinkTime = 0.0f;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), top.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getTop());
        notFound = false;
        blinkTime = 0.0f;   
    }
    if ( CheckCollisionPointRec(GetMousePosition(), initialize.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getInitialize());
        notFound = false;
        blinkTime = 0.0f;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), random.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        int temp;
        if ( mHeap->mCurrent == mHeap->mInitialize )
            temp = rand() % 31;
        else temp = rand() % 999;
        strncpy(name, to_string(temp).c_str(), sizeof(name));
        blinkTime = 0.0f;
    }
}

void ButtonManager::DrawLoadFile(){
    DrawRectangleRounded( loadFile, 0.2f, 0, ORANGE );
    DrawTextEx(customFont, "Load file", {loadFile.x + 10, loadFile.y + 10}, 30, 0, BLACK); 
}

void ButtonManager::DrawInputBox( float x, float y ){

    // Vẽ ô nhập
    DrawRectangleRounded( {x + 210, y, 200, 50}, 0.2f, 0, ORANGE );
    DrawTextEx(customFont, name, {x + 220, y + 10}, 30, 0, BLACK); 

    // Handle ô nhập
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

    // Vẽ đường nhấp nháy
    DrawBlinkingLine(x, y);
}

void ButtonManager::DrawBlinkingLine(float x, float y){
    blinkTime += GetFrameTime();
    if ((int)(blinkTime * 2) % 2 == 0) { // Blink every 0.5 seconds
        Vector2 textSize = MeasureTextEx( customFont, name, fontSize, 0 );
        int cursorX = x + 10 + textSize.x + 210;
        DrawLine(cursorX, y + 10, cursorX, y + 40, BLACK); // Vertical blinking line
    }
}

void ButtonManager::DrawBlinkingText(string txt, float x, float y){
    blinkTime += GetFrameTime();
    if((int)(blinkTime) % 2 == 0){
        DrawTextEx( customFont, txt.c_str(), {x + 210, y - 30}, fontSize, 2, RED);
    }
}

void ButtonManager::DrawRandom(float x, float y){
    // Tính toán tọa độ vẽ xúc xắc 
    x += 210 + ( 200 - dice.width * 0.25 - ( 50 - dice.height * 0.25 ) / 2 );
    y = y + ( 50 - dice.height * 0.25 ) / 2;

    // Lưu tọa độ vào button random để handle click 
    random.rect = { x, y, dice.height * 0.25f, dice.height * 0.25f };
    DrawTextureEx(dice, {x, y}, 0.0f, 0.25f, WHITE);
}

void ButtonManager::DrawBackground(){
    Rectangle rect = { 5, 60, 420, screenHeight - 65 };
    DrawRectangleRounded( rect, 0.05f, 0, Fade(DARKGREEN, 0.8f) );
    Rectangle rect2 = { 430, 60, screenWidth - 435, screenHeight - 65 };
    DrawRectangleRounded( rect2, 0.05f, 0, Fade(BEIGE, 0.5f) );
    Rectangle rect3 = { 430, 600, screenWidth - 435, screenHeight - 605 };
    DrawRectangleRounded( rect3, 0.05f, 0, BEIGE );
}

//-----------------------
//PUSH
//-----------------------

PushState::PushState(){
    tree = std::vector<int>(0);
    heapNode = std::vector<HeapNode>(0);
    originHeapNode = std::vector<HeapNode>(0);
    targetHeapNode = std::vector<HeapNode>(0);
}

PushState::~PushState(){
    tree.clear();
    heapNode.clear();
    originHeapNode.clear();
    targetHeapNode.clear();
}

void Push::saveState(){
    PushState state;
    state.tree = mHeap->tree;
    state.heapNode = heapNode;
    state.originHeapNode = originHeapNode;
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
    state.isAnimating = isAnimating;
    undoStack.push(state);
    std::cout << "State saved!" << std::endl;
}

void Push::handleUndo(){
    if (undoStack.empty()) return;
    PushState prevState = undoStack.top();
    undoStack.pop();
    mHeap->tree = prevState.tree;
    heapNode = prevState.heapNode;
    originHeapNode = prevState.originHeapNode;
    targetHeapNode = prevState.targetHeapNode;
    animatingIdx = prevState.animatingIdx;
    parentIdx = prevState.parentIdx;
    animatingPos = prevState.animatingPos;
    targetPos = prevState.targetPos;
    originPos = prevState.originPos;
    animatingPos2 = prevState.animatingPos2;
    targetPos2 = prevState.targetPos2;
    originPos2 = prevState.originPos2;
    currentStep = prevState.currentStep;
    isAnimating = prevState.isAnimating;
    recalculateAllNodePos(mHeap);
}

Push::Push(MinHeap* heap){ mHeap = heap; }

void Push::draw(){
    buttons.DrawBackground();
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.push.rect.x, buttons.push.rect.y);
    buttons.DrawRandom(buttons.push.rect.x, buttons.push.rect.y);

    DrawPartOfHeap(mHeap, animatingIdx, parentIdx, isAnimating, currentStep);
    if ( currentStep == 1 || currentStep == 4 )
    drawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]), nodeRadius);
    if ( currentStep == 4 )
    drawNode(animatingPos2, std::to_string(mHeap->tree[parentIdx]), nodeRadius);

    drawBlinkingNode();
}

void Push::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        int value = atoi(buttons.name); 

        isAnimating = false;
        saveState();
        handleInsert(value);

        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }

    if ( isPaused && PlayButton.isPressed()){
        isPaused = !isPaused;
    }
    if ( !isPaused && PauseButton.isPressed()){
        isPaused = !isPaused;
    }
    if ( UndoButton.isPressed() ){
        handleUndo();
        isPaused = true;
    }
    if ( isPaused ) return;
    
    updateTreeStructure();
    updateInsert();
    handleBubbleUp();
    updateBubbleUp();
}

// Tiền xử lý cấu trúc cây, vị trí đưa nút từ vị trí ngẫu nhiên vào cây 
void Push::handleInsert(int val){
    // if ( isPaused ) return;

    
    // Thêm giá trị vào cây HeapHeap
    mHeap->tree.push_back(val);
    animatingIdx = mHeap->size() - 1;
    
    // Tính toán vị trí mục tiêu
    int x = animatingIdx - (1 << (int)log2(animatingIdx + 1)) + 1;
    int y = (int)log2(animatingIdx + 1);
    int height = (int)log2(mHeap->size()) + 1;
    targetPos = calculateNodePos({x, y}, height);
    
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

    saveState();
    
}

// Step 0: Cập nhật vị trí các nút trong cây
void Push::updateTreeStructure(){
    // if ( isPaused ) return;

    if ( currentStep != 0 || !isAnimating ) return;
    
    
    bool allFinished = true;
    int n = heapNode.size();
    for ( int i = 0; i < n; i++ ){
        bool nodeAnimating = true;
        updateNodePos( heapNode[i].pos, targetHeapNode[i].pos, originHeapNode[i].pos, 2.0f, nodeAnimating, i );
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
    // if ( isPaused ) return;

    if (currentStep != 1 || !isAnimating) return;
    
    
    updateNodePos ( animatingPos, targetPos, originPos, 1.0f, isAnimating );
    
    if ( !isAnimating ){
        saveState();
        isAnimating = true;
        currentStep = 2;
    }
}

// Step 2: Vẽ nhấp nháy nút đang thêm vào và nút cha của nó
void Push::drawBlinkingNode(){
    // if ( isPaused ) return;

    if ( currentStep != 2 || !isAnimating ) return;

    
    DrawBlinkingNode(heapNode[animatingIdx].pos, heapNode[animatingIdx].val, blinkTime1);
    parentIdx = mHeap->parent(animatingIdx);
    if ( parentIdx >= 0 )
        DrawBlinkingNode(heapNode[parentIdx].pos, heapNode[parentIdx].val, blinkTime2);

    if ( blinkTime1 > 1.5f ){
        saveState();
        blinkTime1 = 0.0f;
        blinkTime2 = 0.0f;
        isAnimating = false;
        currentStep = 3;
    }
}

// Step 3: Kiểm tra xem có cần hoán đổi vị trí với nút cha không
void Push::handleBubbleUp(){
    // if ( isPaused ) return;

    if ( currentStep != 3 || isAnimating ) return;

    
    // parentIdx = mHeap->parent(animatingIdx);
    if ( parentIdx >= 0 && heapNode[parentIdx].val > heapNode[animatingIdx].val){
        originPos = animatingPos;
        targetPos = heapNode[parentIdx].pos;
        
        targetPos2 = heapNode[animatingIdx].pos;
        animatingPos2 = targetPos;
        originPos2 = animatingPos2;
        
        isAnimating = true;
        currentStep = 4;
        saveState();
        
        std::swap(mHeap->tree[animatingIdx], mHeap->tree[parentIdx]);
        
        swapHeapNode(heapNode[animatingIdx], heapNode[parentIdx]);
        
        std::swap(animatingIdx, parentIdx);
    }
    else {
        animatingIdx = -1;
        parentIdx = -1;
        isAnimating = false;
        currentStep = -1;
        while ( !undoStack.empty())
            undoStack.pop();
    }
}

// Step 4: Cập nhật vị trí nút đang thêm vào và nút cha của nó
void Push::updateBubbleUp(){
    // if ( isPaused ) return;

    if ( currentStep != 4 || !isAnimating ) return;

    
    updateNodePos ( animatingPos, targetPos, originPos, 1.0f, isAnimating );
    
    updateNodePos ( animatingPos2, targetPos2, originPos2, 1.0f, isAnimating );
    
    if ( !isAnimating ){
        saveState();
        isAnimating = true;
        currentStep = 2;
    }
}

//-----------------------
//REMOVE
//-----------------------
Remove::Remove(MinHeap* heap) { mHeap = heap; }

void Remove::draw(){
    buttons.DrawBackground();
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.remove.rect.x, buttons.remove.rect.y);
    buttons.DrawRandom(buttons.remove.rect.x, buttons.remove.rect.y);

    DrawPartOfHeap(mHeap, animatingIdx, childIdx, isAnimating, currentStep);
    if ( currentStep == 0 || currentStep == 3 )
        drawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]), nodeRadius);
    if ( currentStep == 3 )
        drawNode(animatingPos2, std::to_string(mHeap->tree[childIdx]), nodeRadius);
    
    // Tìm không thấy hoặc cây trống
    if (buttons.notFound) {
        std::string txt;
        if ( mHeap->tree.empty() )
            txt = "Empty tree!";
        else
            txt = "Value not found!";
        buttons.DrawBlinkingText(txt, buttons.remove.rect.x, buttons.remove.rect.y );
    }
}

void Remove::update(){  
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
    if ( IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0' ){
        int value = atoi(buttons.name);
        animatingIdx = mHeap->search(value);

        if ( animatingIdx != -1 ){
            handleRemove(animatingIdx);
            buttons.notFound = false;
        }
        else {
            buttons.notFound = true;
        }
        
        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }
    updateTreeStructure();
    updateRemove();
    handleBubbleDown();
    updateBubbleDown();
}

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


void Remove::updateRemove(){
    if (currentStep != 0 || !isAnimating) return;

    updateNodePos ( animatingPos, targetPos, originPos, duration, isAnimating );
    
    if ( !isAnimating ){
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
            currentStep = 2; 
            isAnimating = false;
        }
    }

}

void Remove::updateTreeStructure(){
    if ( currentStep != 1 || !isAnimating ) return;
        bool allFinished = true;
        int n = heapNode.size();
        for ( int i = 0; i < n; i++ ){
            bool nodeAnimating = true;
            updateNodePos( heapNode[i].pos, targetHeapNode[i].pos, originHeapNode[i].pos, 1.0f, nodeAnimating, i );
            if ( nodeAnimating ) {
                allFinished = false;
            }
        }
    if ( allFinished ){
        currentStep = 2;
        isAnimating = false;
        std::swap(tempIdx, animatingIdx);
    }
}

void Remove::handleBubbleDown(){
    if ( currentStep != 2 || isAnimating ) return;

    int l = mHeap->left(animatingIdx);
    int r = mHeap->right(animatingIdx);
    childIdx = animatingIdx;
    if ( childIdx < mHeap->size() ){
        if ( l < mHeap->size() && heapNode[l].val < heapNode[childIdx].val )
            childIdx = l;
        if ( r < mHeap->size() && heapNode[r].val < heapNode[childIdx].val )
            childIdx = r;
        if ( childIdx != animatingIdx ){

            targetPos = heapNode[childIdx].pos;
            animatingPos = heapNode[animatingIdx].pos;
            
            animatingPos2 = targetPos;
            targetPos2 = animatingPos;

            originPos = animatingPos;
            originPos2 = animatingPos2;
    
            isAnimating = true;
            currentStep = 3;
    
            // Cập nhật giá trị trong mảng
            std::swap(mHeap->tree[animatingIdx], mHeap->tree[childIdx]);

            // Cập nhật vị trí trong mảng 
            swapHeapNode(heapNode[animatingIdx], heapNode[childIdx]);

            std::swap(animatingIdx, childIdx);
        }
        else {
            animatingIdx = -1;
            childIdx = -1;
            isAnimating = false;
            currentStep = 4;
        }
    }
    else{
        animatingIdx = -1;
        childIdx = -1;
        isAnimating = false;
        currentStep = 4;
    }
}

void Remove::updateBubbleDown(){
    if ( currentStep != 3 || !isAnimating ) return;

    // Cập nhật vị trí nút con
    updateNodePos(animatingPos2, targetPos2, originPos2, duration, isAnimating );
    // Cập nhật vị trí nút cha
    updateNodePos(animatingPos, targetPos, originPos, duration, isAnimating ); 

    if ( !isAnimating )
        currentStep = 2;
}

//-----------------------
//CLEAR
//-----------------------
ClearH::ClearH(MinHeap* heap){ mHeap = heap; }

void ClearH::draw(){
    buttons.DrawBackground();
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
}

void ClearH::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
    mHeap->clear();
    heapNode = std::vector<HeapNode>(31, {0, {0, 0}, false});
}

//-----------------------
//TOP
//-----------------------
Top::Top(MinHeap* heap){ mHeap = heap; }

void Top::draw(){
    buttons.DrawBackground();
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
    if ( !mHeap->tree.empty() )
        DrawBlinkingNode(heapNode[0].pos, mHeap->top(), blinkTime);
    else
        buttons.DrawBlinkingText("Empty tree!", buttons.top.rect.x, buttons.top.rect.y + 40 );
}

void Top::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
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
            heapNode.resize(31, {0, {0, 0}, false});
            int value;
            while (fin >> value){
                mHeap->push(value);
            }
            fin.close();
        }
    }
    mHeap->setState(mHeap->getWaiting());
}

void Initialize::draw(){
    buttons.DrawBackground();
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.initialize.rect.x, buttons.initialize.rect.y);
    buttons.DrawRandom(buttons.initialize.rect.x, buttons.initialize.rect.y);
    buttons.DrawLoadFile();
    drawHeap(mHeap->tree);
}

void Initialize::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        mHeap->tree.clear();
        heapNode = std::vector<HeapNode>(31, {0, {0, 0}, false});
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
    // handleInputFile();
}

//-----------------------
//WAITING
//-----------------------
Waiting::Waiting(MinHeap* heap){ mHeap = heap; }  

void Waiting::draw(){
    buttons.DrawBackground();
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
}

void Waiting::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
}

void MinHeap::draw(){ if ( mCurrent ) mCurrent->draw(); }
void MinHeap::update(){ if ( mCurrent ) mCurrent->update(); }