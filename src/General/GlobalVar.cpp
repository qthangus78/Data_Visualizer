#include "GlobalVar.h"

//--------------------------------
// Khai báo biến toàn cục
//--------------------------------

const float screenWidth = 1400;
const float screenHeight = 750;
const Vector2 GraphDisplayScreenSize = {600.0f, 600.0f};
Rectangle GraphDisplayScreen = {450, 
                                float(screenHeight) - GraphDisplayScreenSize.y - 60, //padding
                                GraphDisplayScreenSize.x, 
                                GraphDisplayScreenSize.y};


const int FPS = 60;
Vector2 mouse;
bool WindowClose = 0;
Font customFont;
Font SSLFont;
Font textCodeFont;
myTexture BackButton;
myTexture PlayButton;
myTexture PauseButton;
myTexture ReplayButton;
myTexture UndoButton;
myTexture RedoButton;
myTexture addButton;
myTexture minusButton;
myTexture repeatButton;

Texture2D customTexture;
Texture2D dice;

ScreenID currentScreenID = ScreenID::StartScreen;

namespace color {
    Color buttonColor        = {72, 166, 167,255};
    Color buttonColorHovered = {0, 106, 113,255};
    Color buttonFile         = {242, 239, 231, 255};
    Color buttonFileHovered  = GRAY;
    Color nodeNotInMode      = {51, 51, 51, 255};
    Color nodeRendered       = {255, 215, 0, 255};
    Color edgeNotInMode      = BLACK;
    Color edgeRendered       = {255, 215, 0, 255};
}
namespace buttonVar {
    button buttonCreate = {{40.0f, 430.0f, button::sizeW, button::sizeH}, color::buttonColor, "Create"};
    button buttonIns    = {{40.0f, 480.0f, button::sizeW, button::sizeH}, color::buttonColor, "Insert"};
    button buttonDel    = {{40.0f, 530.0f, button::sizeW, button::sizeH}, color::buttonColor, "Delete"};
    button buttonF      = {{40.0f, 580.0f, button::sizeW, button::sizeH}, color::buttonColor, "Find"};
    button buttonClear  = {{40.0f, 630.0f, button::sizeW, button::sizeH}, color::buttonColor, "Clear"};
    button buttonGo     = {{290.0f, 580.0f, 60,40}, color::buttonFile, "Go"};
}
Edge EArrow = {100.0f, 3.0f, 20.0f};
Vector2 startLinkedListPos = {50,170};

void LoadFontResource() {
    customFont = LoadFont("../resources/fonts/Montserrat-Bold.ttf");
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);
    SSLFont = LoadFont("../resources/fonts/PublicSans-Bold.ttf");
    SetTextureFilter(SSLFont.texture, TEXTURE_FILTER_BILINEAR);
    textCodeFont = LoadFont("../resources/fonts/PublicSans-Regular.ttf");
    SetTextureFilter(textCodeFont.texture, TEXTURE_FILTER_BILINEAR);
}

void LoadButtonsResources() {
    BackButton.LoadTextureResources("../resources/images/back-normal.png",
         "../resources/images/back-selected.png");

    PlayButton.LoadTextureResources("../resources/images/play-normal.png",
         "../resources/images/play-selected.png");

    PauseButton.LoadTextureResources("../resources/images/pause-normal.png",
         "../resources/images/pause-selected.png"); 

    ReplayButton.LoadTextureResources("../resources/images/replay-normal.png",
         "../resources/images/replay-selected.png");

    UndoButton.LoadTextureResources("../resources/images/Rotate-Left.png",
        "../resources/images/Rotate-Left-Select.png");

    RedoButton.LoadTextureResources("../resources/images/Rotate-Right.png",
        "../resources/images/Rotate-Right-Select.png");
    customTexture = LoadTexture("../resources/images/back-normal.png");
    addButton.LoadTextureResources("../resources/images/add-circle.png",
        "../resources/images/add-circle-selected.png");
    minusButton.LoadTextureResources("../resources/images/minus-circle.png",
        "../resources/images/minus-circle-selected.png");
    dice = LoadTexture("../src/Heap/dice.png");
    
    repeatButton.LoadTextureResources("../resources/images/Repeat-normal.png",
        "../resources/images/Repeat-selected.png");
}

void UnloadFontResource() {
    UnloadFont(customFont);
    UnloadFont(SSLFont);
    UnloadFont(textCodeFont);
}
