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

bool AnnouncementBox::HandleScrollingBar(float totalContentHeight, float maxContentHeight, float scrollY, float scrollHeight) {
    // Calculate scroll bar parameters
    bool needsScrollBar = totalContentHeight > maxContentHeight;
    float scrollThumbHeight = needsScrollBar ? (scrollHeight / totalContentHeight) * scrollHeight : 0;
    float scrollThumbY = scrollY + (scrollOffset / totalContentHeight) * scrollHeight;
    
    // Handle mouse wheel for scrolling
    if (needsScrollBar && CheckCollisionPointRec(GetMousePosition(), rect)) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            scrollOffset -= wheel * 20;
            scrollOffset = fmax(0, fmin(scrollOffset, totalContentHeight - maxContentHeight));
        }
    }
    
    // Handle scroll bar dragging
    static bool isDragging = false;
    static float dragOffset = 0;
    
    if (needsScrollBar) {
        Rectangle scrollBarRect = {rect.x + rect.width - 15, scrollY, 10, scrollHeight};
        Rectangle scrollThumbRect = {rect.x + rect.width - 15, scrollThumbY, 10, scrollThumbHeight};
        
        if (CheckCollisionPointRec(GetMousePosition(), scrollThumbRect)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isDragging = true;
                dragOffset = GetMousePosition().y - scrollThumbY;
            }
        }
        
        if (isDragging) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                float newThumbY = GetMousePosition().y - dragOffset;
                newThumbY = fmax(scrollY, fmin(newThumbY, scrollY + scrollHeight - scrollThumbHeight));
                scrollOffset = ((newThumbY - scrollY) / scrollHeight) * totalContentHeight;
            } else {
                isDragging = false;
            }
        }
        
        // Draw scroll bar
        DrawRectangleRec(scrollBarRect, Color{200, 200, 200, 100});
        DrawRectangleRec(scrollThumbRect, Color{150, 150, 150, 200});
    }
    
    return needsScrollBar;
}

void AnnouncementBox::Draw() {
    DrawRectangleRec(rect, backgroundColor);
    DrawRectangleLinesEx(rect, 1.0f, borderColor);
    
    // Draw title
    float titleY = rect.y + 10;
    Vector2 titlePos = {rect.x + rect.width / 2.0f - MeasureTextEx(customFont, title, titleFontSize, 1.0f).x / 2.0f, titleY};
    DrawTextEx(customFont, title, titlePos, 24.0f, 1.0f, textColor);
    
    // Calculate visible content height
    float codeHeight = content.size() * lineHeight;
    float infoHeight = infoLines.size() * lineHeight;
    float totalContentHeight = codeHeight + infoHeight + 50; // 50 for padding
    float maxContentHeight = rect.height - 60; // Account for title and padding
    
    // Handle scrolling bar
    float scrollY = rect.y + 50;
    float scrollHeight = maxContentHeight;
    bool needsScrollBar = HandleScrollingBar(totalContentHeight, maxContentHeight, scrollY, scrollHeight);
    
    // Apply scrolling to content
    BeginScissorMode(rect.x, rect.y + 50, rect.width - (needsScrollBar ? 20 : 0), maxContentHeight);
    
    float codeY = rect.y + 50 - scrollOffset;
    
    // Draw code with highlighting
    for (int i = 0; i < content.size(); i++) {
        float y = codeY + i * lineHeight;
        
        // Skip if not visible
        if (y + lineHeight < rect.y + 50 || y > rect.y + 50 + maxContentHeight) {
            continue;
        }
        
        Color bgColor = (i >= highlightStartLine && i <= highlightEndLine) ? highlightBgColor : backgroundColor;
        DrawRectangle(rect.x + 10, y, rect.width - 30, lineHeight, bgColor);
        DrawTextEx(customFont, content[i], (Vector2){rect.x + 15, y + 5}, contentFontSize, 1.0f, textColor);
    }
    
    // Draw divider line
    float lineY = codeY + content.size() * lineHeight + 10;
    DrawLine(rect.x + 20, lineY, rect.x + rect.width - 30, lineY, Color{100, 100, 100, 150});
    
    // Draw info section
    float infoY = lineY + 20;
    
    for (const auto& info : infoLines) {
        // Skip if not visible
        if (infoY + lineHeight < rect.y + 50 || infoY > rect.y + 50 + maxContentHeight) {
            infoY += lineHeight;
            continue;
        }
        
        char buffer[100];
        sprintf(buffer, "%s: %s", info.label, info.value);
        DrawTextEx(customFont, buffer, (Vector2){rect.x + 20, infoY}, infoFontSize, 1.0f, textColor);
        infoY += lineHeight;
    }
    
    EndScissorMode();
}

