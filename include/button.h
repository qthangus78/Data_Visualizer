#pragma once 
#include "GlobalVar.h"

struct myTexture {
    Texture2D texture;  
    float x, y;
    const Color color[2] = {WHITE, BLUE};
    Color currentColor = WHITE;

    myTexture() {}
    myTexture(Texture2D _texture, float _x = 0.0, float _y = 0.0) {
        texture = _texture;
        x = _x;
        y = _y;
    }

    bool CheckMouseCollision() {
        return CheckCollisionPointRec(mouse, {x, y, (float)texture.width, (float)texture.height});
    }

    void processColor() {
        currentColor = color[CheckMouseCollision()];
    }

    void Drawtexture() {
        processColor();
        DrawTexture(texture, x, y, currentColor);
    }
};
extern myTexture BackButton; 

void display_title(const char *Title, int lastSlideID); 