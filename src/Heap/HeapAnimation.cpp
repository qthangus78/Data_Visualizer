#include "Heap.h"
#include <cmath>
#include <cstring>
 
//-----------------------
//BUTTON MANAGER
//-----------------------
void ButtonManager::DrawButtons(){
    // PUSH
    DrawRectangleRounded ( push.rect, roundness, (int)segments, push.color );
    int pushX = push.rect.x + push.rect.width / 2 - MeasureText ( push.txt.c_str(), 20 ) / 2;
    int pushY = push.rect.y + push.rect.height / 2 - 20 / 2;
    DrawText ( push.txt.c_str(), pushX, pushY, 20, BLACK );
    // REMOVE
    DrawRectangleRounded ( remove.rect, roundness, (int)segments, remove.color );
    int removeX = remove.rect.x + remove.rect.width / 2 - MeasureText ( remove.txt.c_str(), 20 ) / 2;
    int removeY = remove.rect.y + remove.rect.height / 2 - 20 / 2;
    DrawText ( remove.txt.c_str(), removeX, removeY, 20, BLACK );
    // CLEAR
    DrawRectangleRounded ( clear.rect, roundness, (int)segments, clear.color );
    int clearX = clear.rect.x + clear.rect.width / 2 - MeasureText ( clear.txt.c_str(), 20 ) / 2;
    int clearY = clear.rect.y + clear.rect.height / 2 - 20 / 2;
    DrawText ( clear.txt.c_str(), clearX, clearY, 20, BLACK );
    // TOP
    DrawRectangleRounded ( top.rect, roundness, (int)segments, top.color );
    int topX = top.rect.x + top.rect.width / 2 - MeasureText ( top.txt.c_str(), 20 ) / 2;
    int topY = top.rect.y + top.rect.height / 2 - 20 / 2;
    DrawText ( top.txt.c_str(), topX, topY, 20, BLACK );
}

void ButtonManager::HandleButtons(MinHeap* mHeap){
    if ( CheckCollisionPointRec(GetMousePosition(), push.rect ) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON ) ){
        mHeap->setState(mHeap->getPush());
        notFound = false;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), remove.rect ) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getRemove());
    }
    if ( CheckCollisionPointRec(GetMousePosition(), clear.rect ) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getClear());
        notFound = false;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), top.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getTop());
        notFound = false;   
    }
    if ( CheckCollisionPointRec(GetMousePosition(), random.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        int temp = rand() % 999;
        strncpy(name, to_string(temp).c_str(), sizeof(name));
    }
}

void ButtonManager::DrawInputBox( int x, int y ){

    // Vẽ ô nhập
    DrawRectangleRounded( {(float)x,(float)y - 60, 200, 50}, roundness, (int)segments, Fade(MAROON, 0.2f) );
    DrawText(name, x + 10, y - 50, 20, BLACK); 

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

void ButtonManager::DrawBlinkingLine(int x, int y){
    blinkTime += GetFrameTime();
    if ((int)(blinkTime * 2) % 2 == 0) { // Blink every 0.5 seconds
        int cursorX = x + 10 + MeasureText(name, 20);
        DrawLine(cursorX, y - 50, cursorX, y - 30, BLACK); // Vertical blinking line
    }
}

void ButtonManager::DrawBlinkingText(string txt, int x, int y){
    blinkTime += GetFrameTime();
    if((int)blinkTime % 2 == 0){
        DrawText(txt.c_str(), x, y - 90, 20, RED);
    }
}

void ButtonManager::DrawRandom(int x, int y){
    // Tính toán tọa độ vẽ xúc xắc 
    x += ( 200 - dice.width * 0.25 - ( 50 - dice.height * 0.25 ) / 2 );
    y = y - 60 + ( 50 - dice.height * 0.25 ) / 2;

    // Lưu tọa độ vào button random để handle click 
    random.rect = { (float)x, (float)y, dice.height * 0.25f, dice.height * 0.25f };
    DrawTextureEx(dice, {(float)x, (float)y}, 0.0f, 0.25f, WHITE);
}

//-----------------------
//PUSH
//-----------------------
Push::Push(MinHeap* heap){ mHeap = heap; }

void Push::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.push.rect.x, buttons.push.rect.y);
    buttons.DrawRandom(buttons.push.rect.x, buttons.push.rect.y);

    DrawPartOfHeap(mHeap, animatingIdx, parentIdx, isAnimating, currentStep);
    if ( currentStep == 1 || currentStep == 3 )
        drawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]), 20,WHITE);
    if ( currentStep == 3 )
        drawNode(animatingPos2, std::to_string(mHeap->tree[parentIdx]), 20,WHITE);
    if ( currentStep == 4 ){
        currentStep == 0;
        isAnimating = false;
    }
}

void Push::update(){
    buttons.HandleButtons(mHeap);
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        int value = atoi(buttons.name); 

        currentStep = 0;
        isAnimating = false;
        handleInsert(value);

        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }
    updateInsert();
    handleBubbleUp();
    updateBubbleUp();
}

void Push::handleInsert(int val){
    // Thêm giá trị vào cây HeapHeap
    mHeap->tree.push_back(val);
    animatingIdx = mHeap->size() - 1;

    // Tính toán vị trí mục tiêu
    int x = animatingIdx - (1 << (int)log2(animatingIdx + 1)) + 1;
    int y = (int)log2(animatingIdx + 1);
    int height = (int)log2(mHeap->size()) + 1;
    targetPos = calculateNodePos({(float)x, (float)y}, 100, screenWidth / 2, height);

    // Khởi tạo vị trí ban đầu
    animatingPos = { screenWidth / 2, 600 };
    heapNode[animatingIdx] = {val, targetPos, true};

    if ( mHeap->size() == std::pow(2, height - 1))
        recalculateNodePos ( mHeap );

    currentStep = 1;
    isAnimating = true;
}

void Push::updateInsert(){
    if (currentStep != 1 || !isAnimating) return;

    Vector2 direction = Vector2Subtract(targetPos, animatingPos);
    float distance = Vector2Length(direction);

    if (distance > 5.0f) {
        Vector2 step = Vector2Scale(Vector2Normalize(direction), 5.0f);
        animatingPos = Vector2Add(animatingPos, step);
    } else {
        animatingPos = targetPos;
        isAnimating = false;
        currentStep = 2; // Chuyển sang bước bubble-up
    }
}

void Push::handleBubbleUp(){
    if ( currentStep != 2 || isAnimating ) return;

    parentIdx = mHeap->parent(animatingIdx);
    if ( parentIdx >= 0 && heapNode[parentIdx].val > heapNode[animatingIdx].val){
        targetPos = heapNode[parentIdx].pos;

        targetPos2 = heapNode[animatingIdx].pos;
        animatingPos2 = targetPos;

        isAnimating = true;
        currentStep = 3;

        std::swap(mHeap->tree[animatingIdx], mHeap->tree[parentIdx]);

        swapHeapNode(heapNode[animatingIdx], heapNode[parentIdx]);

        std::swap(animatingIdx, parentIdx);
    }
    else {
        animatingIdx = -1;
        parentIdx = -1;
        isAnimating = false;
        currentStep = 4;
    }
}

void Push::updateBubbleUp(){
    if ( currentStep != 3 || !isAnimating ) return;

    // Cập nhật vị trí của nút cha
    Vector2 direction2 = Vector2Subtract(targetPos2, animatingPos2);
    float distance2 = Vector2Length(direction2);

    if ( distance2 > 5.0f ){
        Vector2 step2 = Vector2Scale(Vector2Normalize(direction2), 5.0f);
        animatingPos2 = Vector2Add(animatingPos2, step2);
    }
    else
        animatingPos2 = targetPos2;
    
    // Cập nhật vị trí của nút con 
    Vector2 direction = Vector2Subtract(targetPos, animatingPos);
    float distance = Vector2Length(direction);

    if (distance > 5.0f) {
        Vector2 step = Vector2Scale(Vector2Normalize(direction), 5.0f);
        animatingPos = Vector2Add(animatingPos, step);
    }
    else {
        animatingPos = targetPos;
        // Về lại bước 2 kiểm tra bubble up
        isAnimating = false;
        currentStep = 2;
    }

}

void DrawPartOfHeap ( MinHeap* mHeap, int animatingIdx, int parentIdx, bool isAnimating, int currentStep ){
    for ( int i = 0; i < mHeap->size(); i++ ){
        int nodeRadius = 20;
        int height = (int)log2(mHeap->size()) + 1;
        int y = (int)log2(i + 1);

        if ( isAnimating ){
            if ( currentStep == 1 )
                if ( i != animatingIdx )
                    drawNode(heapNode[i].pos, to_string(heapNode[i].val), nodeRadius,WHITE);
            if ( currentStep == 3 )
                if ( i != animatingIdx && i != parentIdx )
                    drawNode(heapNode[i].pos, to_string(heapNode[i].val), nodeRadius,WHITE);
        }
        else
            drawNode(heapNode[i].pos, to_string(heapNode[i].val), nodeRadius,WHITE);

        if ( y < height - 1 ){
            int leftChild = 2*i + 1;
            if ( leftChild < mHeap->size() ){
                Vector2 arrowStart = calculateArrowPosition(heapNode[i].pos, heapNode[leftChild].pos, nodeRadius);
                Vector2 arrowEnd = calculateArrowPosition(heapNode[leftChild].pos, heapNode[i].pos, nodeRadius);
                drawArrow(arrowStart, arrowEnd, BLACK);
            }
            int rightChild = 2*i + 2;
            if ( rightChild < mHeap->size() ){
                Vector2 arrowStart = calculateArrowPosition(heapNode[i].pos, heapNode[rightChild].pos, nodeRadius);
                Vector2 arrowEnd = calculateArrowPosition(heapNode[rightChild].pos, heapNode[i].pos, nodeRadius);
                drawArrow(arrowStart, arrowEnd, BLACK);
            }
        }
    }
}

//-----------------------
//REMOVE
//-----------------------
Remove::Remove(MinHeap* heap) { mHeap = heap; }

void Remove::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.remove.rect.x, buttons.remove.rect.y);
    buttons.DrawRandom(buttons.remove.rect.x, buttons.remove.rect.y);

    DrawPartOfHeap(mHeap, animatingIdx, childIdx, isAnimating, currentStep);
    if ( currentStep == 1 || currentStep == 3 )
        drawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]), 20, WHITE);
    if ( currentStep == 3 )
        drawNode(animatingPos2, std::to_string(mHeap->tree[childIdx]), 20, WHITE);

    
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
    buttons.HandleButtons(mHeap);
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
    updateRemove();
    handleBubbleDown();
    updateBubbleDown();
}

void Remove::handleRemove(int idx){
    int i = mHeap->size() - 1;
    animatingIdx = idx;

    animatingPos = heapNode[i].pos;
    targetPos = heapNode[idx].pos;

    std::swap(mHeap->tree[idx], mHeap->tree[i]);
    swapHeapNode(heapNode[idx], heapNode[i]);
    heapNode[i] = {0, {0, 0}, false};
    mHeap->tree.pop_back();

    recalculateNodePos ( mHeap );

    currentStep = 1;
    isAnimating = true;
}


void Remove::updateRemove(){
    if (currentStep != 1 || !isAnimating) return;
    
    Vector2 direction = Vector2Subtract(targetPos, animatingPos);
    float distance = Vector2Length(direction);
    
    if (distance > 5.0f) {
        Vector2 step = Vector2Scale(Vector2Normalize(direction), 5.0f);
        animatingPos = Vector2Add(animatingPos, step);
    } else {
        animatingPos = targetPos;
        isAnimating = false;
        currentStep = 2; // Chuyển sang bước bubble-up
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
    
            targetPos2 = heapNode[animatingIdx].pos;
            animatingPos2 = targetPos;
    
            isAnimating = true;
            currentStep = 3;
    
            std::swap(mHeap->tree[animatingIdx], mHeap->tree[childIdx]);
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

    // Cập nhật vị trí của nút con
    Vector2 direction2 = Vector2Subtract(targetPos2, animatingPos2);
    float distance2 = Vector2Length(direction2);

    if ( distance2 > 5.0f ){
        Vector2 step2 = Vector2Scale(Vector2Normalize(direction2), 5.0f);
        animatingPos2 = Vector2Add(animatingPos2, step2);
    }
    else
        animatingPos2 = targetPos2;
    
    // Cập nhật vị trí của nút cha
    Vector2 direction = Vector2Subtract(targetPos, animatingPos);
    float distance = Vector2Length(direction);

    if (distance > 5.0f) {
        Vector2 step = Vector2Scale(Vector2Normalize(direction), 5.0f);
        animatingPos = Vector2Add(animatingPos, step);
    }
    else {
        animatingPos = targetPos;
        // Về lại bước 2 kiểm tra bubble up
        isAnimating = false;
        currentStep = 2;
    }

}

//-----------------------
//CLEAR
//-----------------------
ClearH::ClearH(MinHeap* heap){ mHeap = heap; }

void ClearH::draw(){
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
}

void ClearH::update(){
    buttons.HandleButtons(mHeap);
    mHeap->clear();
    heapNode = std::vector<HeapNode>(31, {0, {0, 0}, false});
}

//-----------------------
//TOP
//-----------------------
Top::Top(MinHeap* heap){ mHeap = heap; }

void Top::draw(){
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
    if ( !mHeap->tree.empty() )
        DrawBlinkingTop();
}

void Top::update(){
    buttons.HandleButtons(mHeap);
    mHeap->top();
}

void Top::DrawBlinkingTop(){
    Vector2 pos = calculateNodePos({0, 0}, 100, screenWidth / 2, (int)log2(mHeap->size())+1);
    blinkTime += GetFrameTime();
    if ((int)blinkTime % 2 == 0) { // Blink every 1 seconds
        DrawCircle( pos.x, pos.y, 20, YELLOW);
        string val = to_string(mHeap->top());
        Vector2 textSize = MeasureTextEx(customFont, val.c_str(), 23, 2);
        DrawTextEx(customFont, val.c_str() ,{pos.x - textSize.x / 2, pos.y - textSize.y / 2}, 22, 2, BLACK);
    }
}

void MinHeap::draw(){ if ( mCurrent ) mCurrent->draw(); }
void MinHeap::update(){ if ( mCurrent ) mCurrent->update(); }
