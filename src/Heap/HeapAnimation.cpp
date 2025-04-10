#include "Heap.h"
#include "tinyfiledialogs.h"

//-----------------------
//BUTTON MANAGER
//-----------------------
void ButtonManager::DrawButtons(){
    // PUSH
    Vector2 textSize = MeasureTextEx ( customFont, push.txt.c_str(), fontSize, 2 );
    DrawRectangleRounded ( push.rect, 0.2f, 0, push.color );
    float pushX = push.rect.x + push.rect.width / 2 - textSize.x / 2;
    float pushY = push.rect.y + push.rect.height / 2 - fontSize / 2;
    DrawTextEx( customFont, push.txt.c_str(), { pushX, pushY }, fontSize, 2, WHITE );
    // REMOVE
    textSize = MeasureTextEx ( customFont, remove.txt.c_str(), fontSize, 2 );
    DrawRectangleRounded ( remove.rect, 0.2f, 0, remove.color );
    float removeX = remove.rect.x + remove.rect.width / 2 - textSize.x / 2;
    float removeY = remove.rect.y + remove.rect.height / 2 - fontSize / 2;
    DrawTextEx( customFont, remove.txt.c_str(), { removeX, removeY }, fontSize, 2, WHITE );
    // CLEAR
    textSize = MeasureTextEx ( customFont, clear.txt.c_str(), fontSize, 2 );
    DrawRectangleRounded ( clear.rect, 0.2f, 0, clear.color );
    float clearX = clear.rect.x + clear.rect.width / 2 - textSize.x / 2;
    float clearY = clear.rect.y + clear.rect.height / 2 - fontSize / 2;
    DrawTextEx( customFont, clear.txt.c_str(), { clearX, clearY }, fontSize, 2, WHITE );
    // TOP
    textSize = MeasureTextEx ( customFont, top.txt.c_str(), fontSize, 2 );
    DrawRectangleRounded ( top.rect, 0.2f, 0, top.color );
    float topX = top.rect.x + top.rect.width / 2 - textSize.x / 2;
    float topY = top.rect.y + top.rect.height / 2 - fontSize / 2;
    DrawTextEx( customFont, top.txt.c_str(), { topX, topY }, fontSize, 2, WHITE );
    // INITIALIZE
    textSize = MeasureTextEx ( customFont, initialize.txt.c_str(), fontSize, 2 );
    DrawRectangleRounded ( initialize.rect, 0.2f, 0, initialize.color );
    float initializeX = initialize.rect.x + initialize.rect.width / 2 - textSize.x / 2;
    float initializeY = initialize.rect.y + initialize.rect.height / 2 - fontSize / 2;
    DrawTextEx ( customFont, initialize.txt.c_str(), {initializeX, initializeY}, fontSize, 2, WHITE );
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
    if ( CheckCollisionPointRec(GetMousePosition(), initialize.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        mHeap->setState(mHeap->getInitialize());
        notFound = false;
    }
    if ( CheckCollisionPointRec(GetMousePosition(), random.rect) && IsMouseButtonPressed ( MOUSE_LEFT_BUTTON )){
        int temp;
        if ( mHeap->mCurrent == mHeap->mInitialize )
            temp = rand() % 31;
        else temp = rand() % 999;
        strncpy(name, to_string(temp).c_str(), sizeof(name));
    }
}

void ButtonManager::DrawLoadFile(){
    DrawRectangleRounded( loadFile, 0.2f, 0, Fade(MAROON, 0.2f) );
    DrawTextEx(customFont, "Load file", {loadFile.x + 10, loadFile.y + 10}, 30, 0, BLACK); 
}

void ButtonManager::DrawInputBox( float x, float y ){

    // Vẽ ô nhập
    DrawRectangleRounded( {x, y - 60, 200, 50}, 0.2f, 0, Fade(MAROON, 0.2f) );
    DrawTextEx(customFont, name, {x + 10, y - 50}, 30, 0, BLACK); 

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
        Vector2 textSize = MeasureTextEx( customFont, name, fontSize, 0 );
        int cursorX = x + 10 + textSize.x;
        DrawLine(cursorX, y - 50, cursorX, y - 20, BLACK); // Vertical blinking line
    }
}

void ButtonManager::DrawBlinkingText(string txt, int x, int y){
    blinkTime += GetFrameTime();
    if((int)blinkTime % 2 == 0){
        DrawTextEx( customFont, txt.c_str(), {x, y - 90}, fontSize, 2, RED);
    }
}

void ButtonManager::DrawRandom(float x, float y){
    // Tính toán tọa độ vẽ xúc xắc 
    x += ( 200 - dice.width * 0.25 - ( 50 - dice.height * 0.25 ) / 2 );
    y = y - 60 + ( 50 - dice.height * 0.25 ) / 2;

    // Lưu tọa độ vào button random để handle click 
    random.rect = { x, y, dice.height * 0.25f, dice.height * 0.25f };
    DrawTextureEx(dice, {x, y}, 0.0f, 0.25f, WHITE);
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
        drawNode(animatingPos, std::to_string(mHeap->tree[animatingIdx]), nodeRadius);
    if ( currentStep == 3 )
        drawNode(animatingPos2, std::to_string(mHeap->tree[parentIdx]), nodeRadius);
    if ( currentStep == 4 ){
        currentStep == -1;
        isAnimating = false;
    }
}

void Push::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
    if (IsKeyPressed(KEY_ENTER) && buttons.name[0] != '\0') {
        int value = atoi(buttons.name); 

        isAnimating = false;
        handleInsert(value);

        buttons.name[0] = '\0';
        buttons.letterCount = 0;
    }
    updateTreeStructure( currentStep, isAnimating );
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
    targetPos = calculateNodePos({x, y}, height);

    // Khởi tạo vị trí ban đầu
    animatingPos = { screenWidth / 2, 600 };
    originPos = animatingPos;
    heapNode[animatingIdx] = {val, targetPos, true};

    // Cập nhật vị trí các nút nếu cấu trúc cây thay đổi
    if ( mHeap->size() >= 4 && mHeap->size() == std::pow(2, height - 1)){
        // animatingIdx = -1;
        parentIdx = -1;
        int n = heapNode.size();
        for ( int i = 0; i < n; i++ ){
            originHeapNode[i] = heapNode[i];
        }
        recalculateAllNodePos ( mHeap );
        for ( int i = 0; i < n; i++ ){
            targetHeapNode[i] = heapNode[i];
        }
        currentStep = 0;
    }
    else{ currentStep = 1; }
    isAnimating = true;

}

void Push::updateInsert(){
    if (currentStep != 1 || !isAnimating) return;

    updateNodePos ( animatingPos, targetPos, originPos, 0.5f, isAnimating );

    if ( !isAnimating )
        currentStep = 2;

    // Vector2 direction = Vector2Subtract(targetPos, animatingPos);
    // float distance = Vector2Length(direction);

    // if (distance > 5.0f) {
    //     Vector2 step = Vector2Scale(Vector2Normalize(direction), 5.0f);
    //     animatingPos = Vector2Add(animatingPos, step);
    // } else {
    //     animatingPos = targetPos;
    //     isAnimating = false;
    //     currentStep = 2; // Chuyển sang bước bubble-up
    // }
}

void Push::handleBubbleUp(){
    if ( currentStep != 2 || isAnimating ) return;

    parentIdx = mHeap->parent(animatingIdx);
    if ( parentIdx >= 0 && heapNode[parentIdx].val > heapNode[animatingIdx].val){
        originPos = animatingPos;
        targetPos = heapNode[parentIdx].pos;

        targetPos2 = heapNode[animatingIdx].pos;
        animatingPos2 = targetPos;
        originPos2 = animatingPos2;

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

    updateNodePos ( animatingPos, targetPos, originPos, 0.5f, isAnimating );

    updateNodePos ( animatingPos2, targetPos2, originPos2, 0.5f, isAnimating );

    if ( !isAnimating )
        currentStep = 2;
}

void DrawPartOfHeap ( MinHeap* mHeap, int animatingIdx, int parentIdx, bool isAnimating, int currentStep ){
    for ( int i = 0; i < mHeap->size(); i++ ){
        int height = (int)log2(mHeap->size()) + 1;
        int y = (int)log2(i + 1);

        if ( isAnimating ){
            if ( currentStep == 0 || currentStep == 1 )
                if ( i != animatingIdx )
                    drawNode(heapNode[i].pos, to_string(heapNode[i].val), nodeRadius);
            if ( currentStep == 3 )
                if ( i != animatingIdx && i != parentIdx )
                    drawNode(heapNode[i].pos, to_string(heapNode[i].val), nodeRadius);
        }
        else
            drawNode(heapNode[i].pos, to_string(heapNode[i].val), nodeRadius);

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
    updateTreeStructure( currentStep, isAnimating );
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
        currentStep = 0;
        std::cout << "Updating tree structure...\n";
    }
    else{ currentStep = 1; }

    isAnimating = true;
}


void Remove::updateRemove(){
    if (currentStep != 1 || !isAnimating) return;

    updateNodePos ( animatingPos, targetPos, originPos, duration, isAnimating );
    
    if ( !isAnimating ){
        animatingIdx = childIdx;
        currentStep = 2;
        isAnimating = false;
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
    updateNodePos(animatingPos2, targetPos2, originPos2, duration, isAnimating );\
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
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
    if ( !mHeap->tree.empty() )
        DrawBlinkingTop();
}

void Top::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
    mHeap->top();
}

void Top::DrawBlinkingTop(){
    Vector2 pos = calculateNodePos({0, 0}, (int)log2(mHeap->size())+1);
    blinkTime += GetFrameTime();
    if ((int)blinkTime % 2 == 0) { // Blink every 1 seconds
        DrawCircle( pos.x, pos.y, 20, YELLOW);
        string val = to_string(mHeap->top());
        Vector2 textSize = MeasureTextEx(customFont, val.c_str(), 23, 2);
        DrawTextEx(customFont, val.c_str() ,{pos.x - textSize.x / 2, pos.y - textSize.y / 2}, 22, 2, BLACK);
    }
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
        int value = atoi(buttons.name); 
        for ( int i = 0; i < value; i++ ){
            mHeap->push(rand() % 999);
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
    buttons.DrawButtons();
    drawHeap(mHeap->tree);
}

void Waiting::update(){
    buttons.HandleButtonsClick(mHeap);
    buttons.HandleButtonsHover();
}

void MinHeap::draw(){ if ( mCurrent ) mCurrent->draw(); }
void MinHeap::update(){ if ( mCurrent ) mCurrent->update(); }