#include "Heap.h"

// BUTTON MANAGER
void ButtonManager::DrawButtons(){
    DrawRectangleRounded ( push.rect, roundness, (int)segments, push.color );
    int pushX = push.rect.x + push.rect.width / 2 - MeasureText ( push.txt.c_str(), 20 ) / 2;
    int pushY = push.rect.y + push.rect.height / 2 - 20 / 2;
    DrawText ( push.txt.c_str(), pushX, pushY, 20, BLACK );

    DrawRectangleRounded ( remove.rect, roundness, (int)segments, remove.color );
    int removeX = remove.rect.x + remove.rect.width / 2 - MeasureText ( remove.txt.c_str(), 20 ) / 2;
    int removeY = remove.rect.y + remove.rect.height / 2 - 20 / 2;
    DrawText ( remove.txt.c_str(), removeX, removeY, 20, BLACK );

    DrawRectangleRounded ( clear.rect, roundness, (int)segments, clear.color );
    int clearX = clear.rect.x + clear.rect.width / 2 - MeasureText ( clear.txt.c_str(), 20 ) / 2;
    int clearY = clear.rect.y + clear.rect.height / 2 - 20 / 2;
    DrawText ( clear.txt.c_str(), clearX, clearY, 20, BLACK );
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
}

void ButtonManager::DrawInputBox( int x, int y ){
    DrawRectangleRounded( {x, y - 60, 200, 50}, roundness, (int)segments, Fade(MAROON, 0.2f) );
    DrawText(name, x + 10, y - 50, 20, BLACK); // Display the current input text
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

    // Draw blinking cursor
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

// PUSH
Push::Push(MinHeap* heap){ mHeap = heap; }

void Push::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.push.rect.x, buttons.push.rect.y);
    drawHeap(mHeap->tree);
    drawAnimate();
}

void Push::update(){
    buttons.HandleButtons(mHeap);
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') { // Handle "Enter" key to push the input value
        int value = atoi(buttons.name); // Convert input to integer
        pushAnimate(value);
        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }
    updateAnimate();
}

// REMOVE
Remove::Remove(MinHeap* heap) { mHeap = heap; }

void Remove::draw(){
    buttons.DrawButtons();
    buttons.DrawInputBox(buttons.remove.rect.x, buttons.remove.rect.y);
    drawHeap(mHeap->tree);

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
        int idx = mHeap->search(value);

        if ( idx != -1 ){
            mHeap->remove(idx);
            buttons.notFound = false;
        }
        else {
            buttons.notFound = true;
        }

        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }
}

// CLEAR
ClearH::ClearH(MinHeap* heap){ mHeap = heap; }

void ClearH::draw(){
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
}

void ClearH::update(){
    buttons.HandleButtons(mHeap);
    mHeap->clear();
}

// ANIMATION
void Push::pushAnimate(int val){
    mHeap->tree.push_back(val);
    animatingIdx = mHeap->size() - 1;
    animatingPos = {screenWidth / 2, 600};
    int x = heapNode[animatingIdx-1].xy.x + 1;
    int y = heapNode[animatingIdx-1].xy.y;
    if ( x >= std::pow(2, y) ){
        x = 0;
        y += 1;
    }
    targetPos = calculateNodePos({x, y}, 100, screenWidth / 2, (int)log2(mHeap->size()) + 1);
    isAnimating = true;
}

void Push::updateAnimate(){
    if (!isAnimating) return;

    // Move the node toward its target position
    Vector2 direction = Vector2Subtract(targetPos, animatingPos);
    float distance = Vector2Length(direction);
    if (distance > 2.0f) { // Continue moving if not close enough
        Vector2 step = Vector2Scale(Vector2Normalize(direction), 5.0f); // Move by a small step
        animatingPos = Vector2Add(animatingPos, step);
    } else {
        // Node has reached its target position
        animatingPos = targetPos;

        // Perform the bubble-up process
        int parentIdx = mHeap->parent(animatingIdx);
        if (parentIdx >= 0 && mHeap->tree[animatingIdx] < mHeap->tree[parentIdx]) {
            // Swap with parent and update target position
            std::swap(mHeap->tree[animatingIdx], mHeap->tree[parentIdx]);
            animatingIdx = parentIdx;
            targetPos = calculateNodePos({animatingIdx, (int)log2(animatingIdx + 1)}, 100, screenWidth / 2, (int)log2(mHeap->size()) + 1);
        } else {
            // Animation is complete
            isAnimating = false;
        }
    }
}

void Push::drawAnimate() {
    if (!isAnimating) return;

    // Draw the animated node
    DrawCircle(animatingPos.x, animatingPos.y, 20, BLUE);
    DrawText(std::to_string(mHeap->tree[animatingIdx]).c_str(), animatingPos.x - 10, animatingPos.y - 10, 20, WHITE);
}

void MinHeap::draw(){ if ( mCurrent ) mCurrent->draw(); }
void MinHeap::update(){ if ( mCurrent ) mCurrent->update(); }