#include "button.h"
#include "TextBox.h"
#include <vector>

myTexture::myTexture() {
    x = 0;
    y = 0;
    width = 40;
    height = 40;
    isLoaded = false; 
}

myTexture::~myTexture() {
    UnloadTexture(MainTexture);
    UnloadTexture(SelectedTexture);
}

void myTexture::LoadTextureResources(const char *mainTexturePath, const char *selectedTexturePath) {
    MainTexture = LoadTexture(mainTexturePath);
    SelectedTexture = LoadTexture(selectedTexturePath);
    
    width = MainTexture.width;
    height = MainTexture.height;
    
    if (MainTexture.id == 0 || SelectedTexture.id == 0) {
        // Handle error: texture not loaded
        std::cerr << "Error loading textures!" << std::endl;
    } else {
        isLoaded = true; // Set the flag to true if textures are loaded successfully
    }
}

bool myTexture::isPressed() {
    return CheckMouseCollision() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void myTexture::Drawtexture() {    
    bool selected = CheckMouseCollision();
    if (selected) {
        DrawTexture(SelectedTexture, x, y, selectedColor);
    } else {
        DrawTexture(MainTexture, x, y, WHITE);
    }
}

bool myTexture::CheckMouseCollision() {
    return CheckCollisionPointRec(mouse, {x, y, width, height});
}

void myTexture::SetPosition(float xPos, float yPos) {
    x = xPos;
    y = yPos;
}

void display_title(const char *Title, ScreenID lastScreenID) {
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
   
    BackButton.x = title.rect.x - 40 - 10;
    BackButton.y = title.rect.y;
    BackButton.Drawtexture();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && BackButton.CheckMouseCollision()) {
        currentScreenID = lastScreenID;
    }
}

bool IsResourcesLoaded() {
    return BackButton.isLoaded && PlayButton.isLoaded && PauseButton.isLoaded && ReplayButton.isLoaded
           && UndoButton.isLoaded && RedoButton.isLoaded;
}

AnnouncementBox::AnnouncementBox() 
    : rect({0, 0, 300, 500})
    , title("")
    , highlightStartLine(-1)
    , highlightEndLine(-1)
    , backgroundColor({240, 240, 240, 230})
    , borderColor({50, 50, 50, 255})
    , titleColor(BLACK)
    , textColor(BLACK)
    , highlightColor({244, 67, 54, 255})
    , highlightBgColor({255, 235, 238, 150})
    , titleFontSize(20.0f)
    , contentFontSize(16.0f)
    , infoFontSize(16.0f)
    , lineHeight(25.0f)
    , indentation(15.0f)
{
}

AnnouncementBox::AnnouncementBox(Rectangle rect, const char* title)
    : rect(rect)
    , title(title)
    , highlightStartLine(-1)
    , highlightEndLine(-1)
    , backgroundColor({240, 240, 240, 230})
    , borderColor({50, 50, 50, 255})
    , titleColor(BLACK)
    , textColor(BLACK)
    , highlightColor({244, 67, 54, 255})
    , highlightBgColor({255, 235, 238, 150})
    , titleFontSize(20.0f)
    , contentFontSize(16.0f)
    , infoFontSize(16.0f)
    , lineHeight(25.0f)
    , indentation(15.0f)
{
}

void AnnouncementBox::SetTitle(const char* newTitle) {
    title = newTitle;
}

void AnnouncementBox::SetContent(const std::vector<char*>& newContent) {
    content = newContent;
}

void AnnouncementBox::SetHighlightLines(int startLine, int endLine) {
    highlightStartLine = startLine;
    highlightEndLine = endLine;
}

void AnnouncementBox::AddInfoLine(const char* label, const char* value) {
    infoLines.push_back({label, value});
}

void AnnouncementBox::ClearInfoLines() {
    infoLines.clear();
}

void AnnouncementBox::Draw() {
    // Draw box background
    DrawRectangleRec(rect, backgroundColor);
    DrawRectangleLinesEx(rect, 2, borderColor);
    
    // Draw title
    Vector2 titleSize = MeasureTextEx(customFont, title, titleFontSize, 1.0f);
    Vector2 titlePos = {
        rect.x + (rect.width - titleSize.x) / 2,
        rect.y + 15
    };
    DrawTextEx(customFont, title, titlePos, titleFontSize, 1.0f, titleColor);
    
    // Draw horizontal line below title
    DrawLineEx(
        (Vector2){rect.x + 10, rect.y + 45},
        (Vector2){rect.x + rect.width - 10, rect.y + 45},
        2, borderColor
    );
    
    // Draw content
    float startY = rect.y + 60;
    
    for (int i = 0; i < content.size(); i++) {
        bool isHighlighted = (i >= highlightStartLine && i <= highlightEndLine);
        Color lineColor = isHighlighted ? highlightColor : textColor;
        
        // Draw highlight background if line is highlighted
        if (isHighlighted) {
            Vector2 textSize = MeasureTextEx(customFont, content[i], contentFontSize, 1.0f);
            DrawRectangle(
                rect.x + 15 + indentation,
                startY + i * lineHeight - 2,
                textSize.x + 10,
                lineHeight,
                highlightBgColor
            );
        }
        
        DrawTextEx(
            customFont, 
            content[i], 
            (Vector2){rect.x + 20 + indentation, startY + i * lineHeight}, 
            contentFontSize, 
            1.0f, 
            lineColor
        );
    }
    
    // Draw additional info
    if (!infoLines.empty()) {
        float infoY = startY + content.size() * lineHeight + 20;
        
        // Draw separator line
        DrawLineEx(
            (Vector2){rect.x + 10, infoY - 10},
            (Vector2){rect.x + rect.width - 10, infoY - 10},
            1, Color{100, 100, 100, 150}
        );
        
        DrawTextEx(customFont, "Current state:", (Vector2){rect.x + 20, infoY}, 18.0f, 1.0f, textColor);
        infoY += 30;
        
        for (const auto& info : infoLines) {
            char buffer[100];
            sprintf(buffer, "%s: %s", info.label, info.value);
            DrawTextEx(customFont, buffer, (Vector2){rect.x + 20, infoY}, infoFontSize, 1.0f, textColor);
            infoY += 25;
        }
    }
}

