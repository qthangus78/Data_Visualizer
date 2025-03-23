#include "TextBox.h"

void TextBox::ScaleProcess() {
    /// adjust :
        // scale
        // current_fontSize
        // textPos
        // scaledRect

        bool isHovered = CheckCollisionPointRec(mouse, rect);
        scale = isHovered ? scaleRate : 1.0f;

        current_fontSize = isHovered ? 1.0f * fontSize * scale : fontSize;
        Vector2 textSize = MeasureTextEx(customFont, text, fontSize, 1.5f);

        // adjust rect.width
        if (rect.width <= textSize.x + 20) 
            rect.width = textSize.x + 20;
        
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

void TextBox::Draw_TextBox() {
    ScaleProcess();
    DrawRectangleRounded(scaledRect, roundness, numSegment, rectColor);
    DrawTextEx(customFont, text, {textPos.x , textPos.y}, current_fontSize, 1.5, textColor);
}