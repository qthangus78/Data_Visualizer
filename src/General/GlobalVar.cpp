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
myTexture BackButton;
myTexture PlayButton;
myTexture PauseButton;
myTexture ReplayButton;
myTexture UndoButton;
myTexture RedoButton;

ScreenID currentScreenID = ScreenID::StartScreen;

namespace color {
    Color buttonColor        = BLUE;
    Color buttonColorHovered = {204, 119, 34, 255};
    Color buttonFile         = GRAY;
    Color buttonFileHovered  = DARKGRAY;
    Color nodeNotInMode      = BLUE;
    Color nodeRendered       = {204, 119, 34, 255};
    Color edgeNotInMode      = BLACK;
    Color edgeRendered       = {204, 119, 34, 255};
}
namespace buttonVar {
    button buttonIns   = {{50.0f, 440.0f, button::sizeW, button::sizeH}, color::buttonColor, "Insert"};
    button buttonDel   = {{50.0f, 500.0f, button::sizeW, button::sizeH}, color::buttonColor, "Delete"};
    button buttonF     = {{50.0f, 560.0f, button::sizeW, button::sizeH}, color::buttonColor, "Find"};
    button buttonClear = {{50.0f, 620.0f, button::sizeW, button::sizeH}, color::buttonColor, "Clear"};
}
Edge EArrow = {100.0f, 3.0f, 20.0f};
Vector2 startLinkedListPos = {50,180};

void LoadFontResource() {
    customFont = LoadFont("../resources/fonts/Montserrat-Bold.ttf");
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
    
    cout << PauseButton.MainTexture.width << " " << PauseButton.MainTexture.height << endl;
}

void UnloadFontResource() {
    UnloadFont(customFont);
}

void UnloadButtonsResources() {
    UnloadTexture(BackButton.MainTexture);
    UnloadTexture(BackButton.SelectedTexture);
    UnloadTexture(PlayButton.MainTexture);
    UnloadTexture(PlayButton.SelectedTexture);
    UnloadTexture(PauseButton.MainTexture);
    UnloadTexture(PauseButton.SelectedTexture);
    UnloadTexture(ReplayButton.MainTexture);
    UnloadTexture(ReplayButton.SelectedTexture);
    UnloadTexture(UndoButton.MainTexture);
    UnloadTexture(UndoButton.SelectedTexture);
    UnloadTexture(RedoButton.MainTexture);
    UnloadTexture(RedoButton.SelectedTexture);
}