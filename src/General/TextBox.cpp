#include "TextBox.h"
#include <cstring>

void TextBox::ScaleProcess() {
    /// adjust :
        // scale
        // current_fontSize
        // textPos
        // scaledRect

        bool isHovered = CheckCollisionPointRec(mouse, rect);
        scale = isHovered ? scaleRate : 1.0f;

        current_fontSize = isHovered ? 1.0f * fontSize * scale : fontSize;
        Vector2 textSize = MeasureTextEx(customFont, text, current_fontSize, 1.5f);

        // adjust rect.width
        if (rect.width <= textSize.x + 20) {
            rect.width = textSize.x + 20;
        }

        scaledRect = { 
            rect.x - (rect.width * (scale - 1.0f)) / 2, 
            rect.y - (rect.height * (scale - 1.0f)) / 2,
            rect.width * scale,
            rect.height * scale
        };

        
        textPos = {
            scaledRect.x + (scaledRect.width - textSize.x) / 2,
            scaledRect.y + (scaledRect.height - textSize.y) / 2
        };
}

void TextBox::MakeHover() {
    current_fontSize = 1.0f * fontSize * scaleRate; 
    Vector2 textSize = MeasureTextEx(customFont, text, current_fontSize, 1.5f);

    // adjust rect.width
    if (rect.width <= textSize.x + 20) 
        rect.width = textSize.x + 20;
    
    scaledRect = { 
        rect.x - (rect.width * (scaleRate - 1.0f)) / 2, 
        rect.y - (rect.height * (scaleRate - 1.0f)) / 2,
        rect.width * scaleRate,
        rect.height * scaleRate
    };

    
    textPos = {
        scaledRect.x + (scaledRect.width - textSize.x) / 2,
        scaledRect.y + (scaledRect.height - textSize.y) / 2
    };
}

void TextBox::Draw_TextBox(bool makeHover) {
    if (makeHover) MakeHover();
    else ScaleProcess();

    DrawRectangleRounded(scaledRect, roundness, numSegment, rectColor);
    DrawTextEx(customFont, text, {textPos.x , textPos.y}, current_fontSize, 1.5f, textColor);
}

void TextBox::Draw_TextBox2() {
    DrawRectangleRounded(rect, roundness, numSegment, rectColor);
    Vector2 textSize = MeasureTextEx(customFont, text, fontSize, 1.5f);
    textPos = {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + (rect.height - textSize.y) / 2
    };
    DrawTextEx(customFont, text, {textPos.x , textPos.y}, fontSize, 1.5f, textColor);
}

void TextBox::Draw_BlinkingLine() {
    if (std::strlen(text) == 0) {
        Vector2 textSize = MeasureTextEx(customFont, "a", current_fontSize, 1.5f);
        DrawLine(scaledRect.x + scaledRect.width / 2, scaledRect.y + scaledRect.height / 2 - textSize.y / 2, 
                 scaledRect.x + scaledRect.width / 2, scaledRect.y + scaledRect.height / 2 + textSize.y / 2, 
                 BLACK);
        return;
    }
    Vector2 textSize = MeasureTextEx(customFont, text, current_fontSize, 1.5f);
    DrawLine(textPos.x + textSize.x, textPos.y, textPos.x + textSize.x, textPos.y + textSize.y, BLACK);
}