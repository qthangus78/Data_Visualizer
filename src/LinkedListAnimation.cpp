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
    BeginDrawing();
    ClearBackground(WHITE);
    // Vẽ 3 nút
    drawButtons();
    // Vẽ danh sách
    drawLinkedList(mscreen->getRoot());
    EndDrawing();
}

void notInMode::handle(){
    handleButtonsHover();     
    handleButtonsClick(mscreen); 
}
//--------------------------------
// Insert
//--------------------------------
Insert::Insert(Screen* s)
    : mscreen(s), InsertProcess(false), frameCounter(0), framecntInsert(0), currentAnimatingNode(nullptr), textIn(""), posTail({800,50}) {
    fileInput = {{ buttonVar::buttonIns.rect.x + 270,
                buttonVar::buttonIns.rect.y,
                100, (float)button::sizeH },
                color::buttonFile, "Load File"};
}

void Insert::drawInputFile() {
    DrawRectangleRounded(fileInput.rect, 0.3f, 30, fileInput.buCol);
    std::string str = fileInput.text;
    Font font = GetFontDefault();
    float spacing = 2.0f;
    float sizeText = 18.0f;
    Vector2 textSize = MeasureTextEx(font, str.c_str(), sizeText, spacing);
    float cursorX = fileInput.rect.x + (fileInput.rect.width - textSize.x) / 2;
    float cursorY = fileInput.rect.y + (fileInput.rect.height - textSize.y) / 2;
    
    DrawTextEx(font, str.c_str(), {cursorX, cursorY}, sizeText, spacing, WHITE);
}

void Insert::handleInputFile(){
    Vector2 mousePos = mscreen->getMouse();
    std::ifstream fin;
    std::string pathFile;

    if (CheckCollisionPointRec(mousePos, fileInput.rect)) {
        fileInput.buCol = color::buttonFileHovered;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mscreen->delAllList();
            textIn.clear();
            const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
            if (filePath) {
                pathFile = filePath;
                fin.open(pathFile);
                if (fin.is_open()) {
                    mscreen->fileInput(fin);
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
    std::string tailStr = std::to_string(mscreen->getTail()->data);
    drawNode(posTail, tailStr, 35);
    // Vẽ danh sách liên kết không đổi (màu mặc định) và phần danh sách cập nhật (mode)
    drawPartofLinkedListNotColor(mscreen->getRoot(), mscreen->getTail(), mscreen);
    drawPartofLinkedList(mscreen->getRoot(), tmp);

    if(!tmp->next) {
        Vector2 startPos;
        int startPosVal;
        Vector2 endPos = posTail;
        // Xác định vị trí bắt đầu của animation dựa vào node đầu của phần cần cập nhật
        if(mscreen->getRoot()->next == tmp) {
            startPos = startLinkedListPos;
            startPosVal = mscreen->getRoot()->data;
        }
        else {
            startPos = mscreen->getPosEnd();
            startPosVal = mscreen->getprevTail()->data;
        }
        // Vẽ mũi tên nối startPos và posTail
        drawArrow(startPos, endPos, color::edgeNotInMode);
        // Vẽ lại node tail để đảm bảo hiển thị
        drawNode(posTail, tailStr, 35);
        std::string startStr = std::to_string(startPosVal);
        DrawCircleV(startPos, 39, color::nodeRendered);
        drawNode(startPos, startStr, 34);
        framecntInsert++;
        Vector2 targetPos = { startPos.x + EArrow.length + 35, startPos.y };
        if(targetPos.x > GetScreenWidth() - 50) {
            targetPos = { 50, mscreen->getPosEnd().y + 100 };
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
    BeginDrawing();
    ClearBackground(WHITE);
    drawButtons();
    drawInputFile();
    Rectangle inputRect = { buttonVar::buttonIns.rect.x + 130,
                            buttonVar::buttonIns.rect.y,
                            130, static_cast<float>(button::sizeH) };
    DrawRectangleRounded(inputRect, 0.3f, 30, color::buttonColorHovered);
    drawTextIn(textIn, inputRect, frameCounter);
    // Nếu có thông báo lỗi, hiển thị ở giữa màn hình
    if(!errorMessage.empty()) {
        Font font = GetFontDefault();
        Vector2 textSize = MeasureTextEx(font, errorMessage.c_str(), 50, 3);
        DrawTextEx(font, errorMessage.c_str(),
                   { (GetScreenWidth() - textSize.x) / 2, (GetScreenHeight() - textSize.y) / 2 },
                   50, 3, BLACK);
    }
    // Tính toán thông số cho hiệu ứng animation
    float subx = 0.0f, suby = 0.0f;
    const float deltax = 10.0f;
    float deltay = 0.0f;
    
    if(mscreen->getRoot()) {
        if(mscreen->getRoot()->next && !mscreen->getRoot()->next->next) {
            subx = static_cast<float>(800 - startLinkedListPos.x - EArrow.length - 35);
            suby = static_cast<float>(startLinkedListPos.y - 50);
        }
        else if(mscreen->getPosEnd().x + EArrow.length + 35 > GetScreenWidth()-50) {
            subx = static_cast<float>(800-startLinkedListPos.x);
            suby = static_cast<float>(mscreen->getPosEnd().y+50);
        }
        else {
            if(mscreen->getPosEnd().x + EArrow.length + 35 < 800){
            subx = static_cast<float>(800 - mscreen->getPosEnd().x - EArrow.length - 35);
            suby = static_cast<float>(mscreen->getPosEnd().y - 50);
            }
            else{
            subx = static_cast<float>(mscreen->getPosEnd().x + EArrow.length + 35 - 800);
            suby = static_cast<float>(mscreen->getPosEnd().y - 50);
            }
        }
        deltay = suby / (subx / deltax);
    }
    if(InsertProcess) {
        insertAnimation(currentAnimatingNode, posTail, deltax, deltay);
    }
    else {
        drawLinkedList(mscreen->getRoot());
    }
    EndDrawing();
}

void Insert::handle(){
    handleButtonsHover();
    handleInputFile();
    // Xử lý click 3 nút + clear text
    Vector2 mousePos = mscreen->getMouse();
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getnotInMode());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getDel());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getFind());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
        
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getClear());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
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
        if(mscreen->getRoot()){
            InsertProcess = true;
            currentAnimatingNode = mscreen->getRoot();
        }
        mscreen->insertList(std::stoi(textIn));
        textIn.clear();
    }
}
//--------------------------------
// Delete
//--------------------------------
Delete::Delete(Screen* s)
: mscreen(s), frameCounter(0), textIn(""), existVal(true) {}

void Delete::draw(){
    BeginDrawing();
    ClearBackground(WHITE);
    drawButtons();
    Rectangle inputRect = { buttonVar::buttonDel.rect.x + 130,
                            buttonVar::buttonDel.rect.y,
                            130, (float)button::sizeH };
    DrawRectangleRounded(inputRect, 0.3f, 30, color::buttonColorHovered);
    Font font = GetFontDefault();
    drawTextIn(textIn,inputRect,frameCounter);
    // Nếu không tồn tại value => "Value Not Found"
    if(!existVal && frameCounter < 30) {
        DrawTextEx(font, "Value Not Found",
                   {inputRect.x-3, inputRect.y-20}, 20, 2, RED);
    }
    // Vẽ danh sách
    drawLinkedList(mscreen->getRoot());
    EndDrawing();
}

void Delete::handle(){
    handleButtonsHover();
    // Xử lý click 3 nút
    Vector2 mousePos = mscreen->getMouse();
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getInsert());
        textIn.clear();
        existVal = true;
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getnotInMode());
        textIn.clear();
        existVal = true;
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getFind());
        textIn.clear();
        existVal = true;
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getClear());
        textIn.clear();
        existVal = true;
        mscreen->setExistVal(mscreen->getRoot());
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
        existVal = mscreen->delList(x);  // nếu false => Value Not Found
    }
}
//--------------------------------
// Find
//--------------------------------
Find::Find(Screen* s)
: mscreen(s), frameCounter(0), textIn(""), framecntFind(0), FindProcess(false), shadedPos({0,0}) {}

void Find::findAnimation(ListNode*& root) {
    if (!root) {
        FindProcess = false;
        shadedPos = { {0, 0}, nullptr };
        return;
    }
    drawLinkedList(mscreen->getRoot());
    if (shadedPos.pos.x + EArrow.length + 35 > GetScreenWidth() - 50 && framecntFind>=30 && root->next) {
        drawArrow(shadedPos.pos,{ startLinkedListPos.x, shadedPos.pos.y + 100 },color::edgeRendered);
        std::string str = std::to_string(root->next->data);
        drawNode({ startLinkedListPos.x, shadedPos.pos.y + 100 }, str,35);
    } else if(shadedPos.pos.x + EArrow.length + 35 < GetScreenWidth() - 50 && framecntFind>=30 && root->next) {
      drawArrow(shadedPos.pos,{ shadedPos.pos.x+EArrow.length, shadedPos.pos.y},color::edgeRendered);
    }
    if (framecntFind >= 60) {
        root = root->next;
        framecntFind = 0;
        if (shadedPos.pos.x + EArrow.length + 35 > GetScreenWidth() - 50) {
            if(framecntFind>=30)drawArrow(shadedPos.pos,{ startLinkedListPos.x, shadedPos.pos.y + 100 },color::edgeRendered);
            shadedPos.pos = { startLinkedListPos.x, shadedPos.pos.y + 100 };
        } else {
            if(framecntFind>=30)drawArrow(shadedPos.pos,{ shadedPos.pos.x+EArrow.length + 35, shadedPos.pos.y},color::edgeRendered);
            shadedPos.pos.x += EArrow.length + 35;
        }
    }
    if (!root) { 
        FindProcess = false;
        shadedPos = { {0, 0}, nullptr };
        return;
    }
    framecntFind++;
    shadedPos.node = root;
    if (root == mscreen->getExistVal()) {
        FindProcess = false; 
        return;
    }
}

void Find::draw() {
    BeginDrawing();
    ClearBackground(WHITE);
    drawButtons();
    Rectangle inputRect = { buttonVar::buttonF.rect.x + 130,
                            buttonVar::buttonF.rect.y,
                            130, (float)button::sizeH };
    DrawRectangleRounded(inputRect, 0.3f, 30, color::buttonColorHovered);
    Font font = GetFontDefault();
    drawTextIn(textIn, inputRect, frameCounter);
    // Nếu danh sách rỗng, hiển thị thông báo
    if (!mscreen->getRoot() && frameCounter < 30) {
        DrawTextEx(font, "Empty List",
                   { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
    }
    else {
        // Nếu đang trong quá trình tìm kiếm, gọi animation tìm kiếm
        if (FindProcess) findAnimation(currentAnimatingNode);
        if(!FindProcess) drawLinkedList(mscreen->getRoot());
        if (shadedPos.pos.x != 0 || shadedPos.pos.y != 0) {
            DrawCircleV(shadedPos.pos, 39, color::nodeRendered);
            if (shadedPos.node) {
                std::string str = std::to_string(shadedPos.node->data);
                drawNode(shadedPos.pos, str, 34);
                if(frameCounter < 30 && !FindProcess)DrawTextEx(font, "Value Found",
                    { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
            }
        }
        // Nếu không tìm thấy giá trị và không còn animation, hiển thị thông báo "Value Not Found"
        else if (mscreen->getRoot() && !mscreen->getExistVal() && frameCounter < 30 && !FindProcess) {
            DrawTextEx(font, "Value Not Found",
                       { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
        }
}
    EndDrawing();
}

void Find::handle(){
    handleButtonsHover();
    // Xử lý click 3 nút
    Vector2 mousePos = mscreen->getMouse();
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getInsert());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
        shadedPos = {0,0};
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getDel());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
        shadedPos = {0,0};
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getnotInMode());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
        shadedPos = {0,0};
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getClear());
        textIn.clear();
        mscreen->setExistVal(mscreen->getRoot());
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
        mscreen->setExistVal(mscreen->findList(x)); // nullptr nếu không có
        if(mscreen->getRoot()) {
            FindProcess = true;
            shadedPos.pos = startLinkedListPos;
            currentAnimatingNode = mscreen->getRoot();
        };
    }
}

Clear::Clear(Screen* s) 
:mscreen(s) {}

void Clear::draw(){
    BeginDrawing();
    ClearBackground(WHITE);
    drawButtons();
    EndDrawing();
}

void Clear::handle(){
    mscreen->delAllList();
    handleButtonsHover();
    Vector2 mousePos = mscreen->getMouse();
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getInsert());
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getDel());
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getFind());
        mscreen->setExistVal(mscreen->getRoot());
    }
    if(CheckCollisionPointRec(mousePos, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mscreen->setState(mscreen->getnotInMode());
        mscreen->setExistVal(mscreen->getRoot());
    }
}