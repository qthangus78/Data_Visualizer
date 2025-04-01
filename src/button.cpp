#include "button.h"
#include "TextBox.h"

void display_title(const char *Title, int lastSlideID) {
    TextBox title;
    title.text = Title;
    title.fontSize = 25;
    title.scaleRate = 1.0f; /// not scale
    title.rectColor = WHITE;
    title.textColor = BLACK;

    title.rect.width = MeasureText(title.text, title.fontSize);
    title.rect.height = 40;
    title.rect.x = screenWidth / 2 - title.rect.width / 2;
    title.rect.y = 15;
    title.numSegment = 50;
    title.roundness = 5.0f;
    title.Draw_TextBox();
   

    myTexture BackButton (
        customTexture,
        title.rect.x - 40 - 10,
        title.rect.y
    );

    BackButton.Drawtexture();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && BackButton.CheckMouseCollision()) {
        currentSlideID = lastSlideID;
    }
}
