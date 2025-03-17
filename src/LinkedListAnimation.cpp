#include "LinkedList.h"
#include "GlobalVar.h"
#include <raylib.h>
#include <string>
#include <algorithm>
#include <cmath>
#include "tinyfiledialogs.h"
//--------------------------------
// notInMode
//--------------------------------
void notInMode::draw(){
    ClearBackground(WHITE);
    // Vẽ 3 nút
    drawButtons();
    // Vẽ danh sách
    drawLinkedList(mSSL->getRoot());
}

void notInMode::handle(){
    handleButtonsHover();     
    handleButtonsClick(mSSL); 
}
//--------------------------------
// Insert
//--------------------------------
Insert::Insert(SSL* s)
    : mSSL(s), InsertProcess(false), frameCounter(0), framecntInsert(0), currentAnimatingNode(nullptr), textIn(""), posTail({800,50}) {
    fileInput = {{ buttonVar::buttonIns.rect.x + 270,buttonVar::buttonIns.rect.y,100, (float)button::sizeH },color::buttonFile, "Load File"};
}

void Insert::drawInputFile() {
    DrawRectangleRounded(fileInput.rect, 0.3f, 30, fileInput.buCol);
    std::string str = fileInput.text;
    float spacing = 2.0f;
    float sizeText = 18.0f;
    Vector2 textSize = MeasureTextEx(customFont, str.c_str(), sizeText, spacing);
    float cursorX = fileInput.rect.x + (fileInput.rect.width - textSize.x) / 2;
    float cursorY = fileInput.rect.y + (fileInput.rect.height - textSize.y) / 2;
    DrawTextEx(customFont, str.c_str(), {cursorX, cursorY}, sizeText, spacing, WHITE);
}

void Insert::handleInputFile(){
    std::ifstream fin;
    std::string pathFile;
    if (CheckCollisionPointRec(mouse, fileInput.rect)) {
        fileInput.buCol = color::buttonFileHovered;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mSSL->delAllList();
            textIn.clear();
            const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
            if (filePath) {
                pathFile = filePath;
                fin.open(pathFile);
                if (fin.is_open()) {
                    mSSL->fileInput(fin);
                    fin.close();
                } else {
                    errorMessage = "Cannot Open File";
                }
            }
        }
    } else {
        fileInput.buCol = color::buttonFile;
    }
}

void Insert::insertAnimation(ListNode*& tmp, Vector2& posTail, int deltax, int deltay) {
    std::string tailStr = std::to_string(mSSL->getTail()->data);
    drawNode(posTail, tailStr, 33);
    // Vẽ danh sách liên kết không đổi (màu mặc định) và phần danh sách cập nhật (mode)
    drawPartofLinkedListNotColor(mSSL->getRoot(), mSSL->getTail(), mSSL);
    drawPartofLinkedList(mSSL->getRoot(), tmp);

    if(!tmp->next) {
        Vector2 startPos;
        int startPosVal;
        Vector2 endPos = posTail;
        // Xác định vị trí bắt đầu của animation dựa vào node đầu của phần cần cập nhật
        if(mSSL->getRoot()->next == tmp) {
            startPos = startLinkedListPos;
            startPosVal = mSSL->getRoot()->data;
        }
        else {
            startPos = mSSL->getPosEnd();
            startPosVal = mSSL->getprevTail()->data;
        }
        // Vẽ mũi tên nối startPos và posTail
        drawArrow2Node(startPos, endPos, color::edgeNotInMode);
        // Vẽ lại node tail để đảm bảo hiển thị
        drawNode(posTail, tailStr, 33);
        std::string startStr = std::to_string(startPosVal);
        DrawCircleV(startPos, 34, color::nodeRendered);
        drawNode(startPos, startStr, 30);
        framecntInsert++;
        Vector2 targetPos = { startPos.x + EArrow.length + 33, startPos.y };
        if(targetPos.x > GetScreenWidth() - 50) {
            targetPos = { startLinkedListPos.x, mSSL->getPosEnd().y + 100 };
        }
        // Mỗi 2 frame, cập nhật posTail tiến dần về targetPos
        if(framecntInsert >= 2) {
            if(targetPos.x < posTail.x)
                posTail = { posTail.x - deltax, posTail.y + deltay };
            else
                posTail = { posTail.x + deltax, posTail.y + deltay };
            framecntInsert = 0;
        }
        if(fabs(posTail.x - targetPos.x) < 40 && fabs(posTail.y - targetPos.y) < 40) {
            InsertProcess = false;
            posTail = { 800, 50 };  
        }
    }
    else {
        framecntInsert++;
        if(framecntInsert >= 15 ) {
            framecntInsert = 0;
            tmp = tmp->next;
        }
    }
}

void Insert::draw() {
    ClearBackground(WHITE);
    drawButtons();
    drawInputFile();
    Rectangle inputRect = { buttonVar::buttonIns.rect.x + 130,buttonVar::buttonIns.rect.y,130, static_cast<float>(button::sizeH) };
    DrawRectangleRounded(inputRect, 0.3f, 30, color::buttonColorHovered);
    drawTextIn(textIn, inputRect, frameCounter);
    // Nếu có thông báo lỗi, hiển thị ở giữa màn hình
    if(!errorMessage.empty()) {
        Vector2 textSize = MeasureTextEx(customFont, errorMessage.c_str(), 50, 3);
        DrawTextEx(customFont, errorMessage.c_str(),{ (GetScreenWidth() - textSize.x) / 2, (GetScreenHeight() - textSize.y) / 2 }, 50, 3, BLACK);
    }
    // Tính toán thông số cho hiệu ứng animation
    float subx = 0.0f, suby = 0.0f;
    const float deltax = 10.0f;
    float deltay = 0.0f;
    if(mSSL->getRoot()) {
        if(mSSL->getRoot()->next && !mSSL->getRoot()->next->next) {
            subx = static_cast<float>(800 - startLinkedListPos.x - EArrow.length - 35);
            suby = static_cast<float>(startLinkedListPos.y - 50);
        }
        else if(mSSL->getPosEnd().x + EArrow.length + 35 > GetScreenWidth()-50) {
            subx = static_cast<float>(800-startLinkedListPos.x);
            suby = static_cast<float>(mSSL->getPosEnd().y+50);
        }
        else {
            if(mSSL->getPosEnd().x + EArrow.length + 35 < 800){
            subx = static_cast<float>(800 - mSSL->getPosEnd().x - EArrow.length - 35);
            suby = static_cast<float>(mSSL->getPosEnd().y - 50);
            }
            else{
            subx = static_cast<float>(mSSL->getPosEnd().x + EArrow.length + 35 - 800);
            suby = static_cast<float>(mSSL->getPosEnd().y - 50);
            }
        }
        deltay = suby / (subx / deltax);
    }
    if(InsertProcess) {
        insertAnimation(currentAnimatingNode, posTail, deltax, deltay);
    }
    else {
        drawLinkedList(mSSL->getRoot());
    }
}

void Insert::handle(){
    handleButtonsHover();
    handleInputFile();
    // Xử lý click 3 nút + clear text
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getnotInMode());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getDel());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getFind());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
        
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getClear());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
    }
    // Nhập số
    int key = GetKeyPressed();
    if(key >= '0' && key <= '9') {
        textIn.push_back((char)key);
    }
    if(key == KEY_BACKSPACE && !textIn.empty()) {
        textIn.pop_back();
    }
    if(key == KEY_ENTER && !textIn.empty()) {
        if(mSSL->getRoot()){
            InsertProcess = true;
            currentAnimatingNode = mSSL->getRoot();
        }
        mSSL->insertList(std::stoi(textIn));
        textIn.clear();
    }
}
//--------------------------------
// Delete
//--------------------------------
Delete::Delete(SSL* s)
: mSSL(s), frameCounter(0), textIn(""), existVal(true) {}

void Delete::draw(){
    ClearBackground(WHITE);
    drawButtons();
    Rectangle inputRect = { buttonVar::buttonDel.rect.x + 130,buttonVar::buttonDel.rect.y,130, (float)button::sizeH };
    DrawRectangleRounded(inputRect, 0.3f, 30, color::buttonColorHovered);
    drawTextIn(textIn,inputRect,frameCounter);
    // Nếu không tồn tại value => "Value Not Found"
    if(!existVal && frameCounter < 30) {
        DrawTextEx(customFont, "Value Not Found", {inputRect.x-3, inputRect.y-20}, 20, 2, RED);
    }
    drawLinkedList(mSSL->getRoot());
}

void Delete::handle(){
    handleButtonsHover();
    // Xử lý click 3 nút 
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getInsert());
        textIn.clear();
        existVal = true;
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getnotInMode());
        textIn.clear();
        existVal = true;
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getFind());
        textIn.clear();
        existVal = true;
        mSSL->setExistVal(mSSL->getRoot());
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getClear());
        textIn.clear();
        existVal = true;
        mSSL->setExistVal(mSSL->getRoot());
    }
    // Xử lý input
    int key = GetKeyPressed();
    if(key >= '0' && key <= '9') {
        textIn.push_back((char)key);
    }
    if(key == KEY_BACKSPACE && !textIn.empty()) {
        textIn.pop_back();
    }
    if(key == KEY_ENTER && !textIn.empty()) {
        int x = std::stoi(textIn);
        textIn.clear();
        existVal = mSSL->delList(x);  // nếu false => Value Not Found
    }
}
//--------------------------------
// Find
//--------------------------------
Find::Find(SSL* s)
: mSSL(s), frameCounter(0), textIn(""), framecntFind(0), FindProcess(false), shadedPos({0,0}) {}
void Find::findAnimation(ListNode*& root) {
    if (!root) {
        FindProcess = false;
        shadedPos = { {0, 0}, nullptr };
        return;
    }
    drawLinkedList(mSSL->getRoot());
    if (shadedPos.pos.x + EArrow.length + 33 > GetScreenWidth() - 50 && framecntFind>=30 && root->next) {
        drawArrow2Node(shadedPos.pos,{ startLinkedListPos.x, shadedPos.pos.y + 100 },color::edgeRendered);
        std::string str = std::to_string(root->next->data);
        drawNode({ startLinkedListPos.x, shadedPos.pos.y + 100 }, str,33);
    } else if(shadedPos.pos.x + EArrow.length + 33 < GetScreenWidth() - 50 && framecntFind>=30 && root->next) {
      drawArrow(shadedPos.pos,{ shadedPos.pos.x+EArrow.length, shadedPos.pos.y},color::edgeRendered);
    }
    if (framecntFind >= 60) {
        root = root->next;
        framecntFind = 0;
        if (shadedPos.pos.x + EArrow.length + 33 > GetScreenWidth() - 50) {
            shadedPos.pos = { startLinkedListPos.x, shadedPos.pos.y + 100 };
        } else {
            shadedPos.pos.x += EArrow.length + 33;
        }
    }
    if (!root) { 
        FindProcess = false;
        shadedPos = { {0, 0}, nullptr };
        return;
    }
    framecntFind++;
    shadedPos.node = root;
    if (root == mSSL->getExistVal()) {
        FindProcess = false; 
        return;
    }
}

void Find::draw() {
    ClearBackground(WHITE);
    drawButtons();
    Rectangle inputRect = { buttonVar::buttonF.rect.x + 130,buttonVar::buttonF.rect.y,130, (float)button::sizeH };
    DrawRectangleRounded(inputRect, 0.3f, 30, color::buttonColorHovered);
    drawTextIn(textIn, inputRect, frameCounter);
    // Nếu danh sách rỗng, hiển thị thông báo
    if (!mSSL->getRoot() && frameCounter < 30) {
        DrawTextEx(customFont, "Empty List", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
    }
    else {  
        // Nếu đang trong quá trình tìm kiếm, gọi animation tìm kiếm
        if (FindProcess) findAnimation(currentAnimatingNode);
        if(!FindProcess) drawLinkedList(mSSL->getRoot());
        if (shadedPos.pos.x != 0 || shadedPos.pos.y != 0) {
            DrawCircleV(shadedPos.pos, 34, color::nodeRendered);
            if (shadedPos.node) {
                std::string str = std::to_string(shadedPos.node->data);
                drawNode(shadedPos.pos, str, 30);
                if(frameCounter < 30 && !FindProcess){
                    DrawTextEx(customFont, "Value Found", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);}
            }
        }
        else if (mSSL->getRoot() && !mSSL->getExistVal() && frameCounter < 30 && !FindProcess) {
            DrawTextEx(customFont, "Value Not Found",{ inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
        } 
}
}

void Find::handle(){
    handleButtonsHover();
    // Xử lý click 3 nút
    if(CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getInsert());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
        shadedPos = {0,0};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getDel());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
        shadedPos = {0,0};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getnotInMode());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
        shadedPos = {0,0};
    }
    if(CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mSSL->setState(mSSL->getClear());
        textIn.clear();
        mSSL->setExistVal(mSSL->getRoot());
        shadedPos = {0,0};
    }
    // Nhập số
    int key = GetKeyPressed();
    if(key >= '0' && key <= '9') {
        textIn.push_back((char)key);
    }
    if(key == KEY_BACKSPACE && !textIn.empty()) {
        textIn.pop_back();
    }
    if(key == KEY_ENTER && !textIn.empty()) {
        int x = std::stoi(textIn);
        textIn.clear();
        mSSL->setExistVal(mSSL->findList(x)); // nullptr nếu không có
        if(mSSL->getRoot()) {
            FindProcess = true;
            shadedPos.pos = startLinkedListPos;
            currentAnimatingNode = mSSL->getRoot();
        };
    }
}

Clear::Clear(SSL* s) 
:mSSL(s) {}

void Clear::draw(){
    ClearBackground(WHITE);
    drawButtons();
}

void Clear::handle(){
    mSSL->delAllList();
    handleButtonsHover();
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