#pragma once 
#include <raylib.h>
#include <bits/stdc++.h>
using namespace std;

enum class ScreenID {
    StartScreen,
    StartMenuScreen,
    SettingScreen,
    SLLScreen,
    HeapScreen,
    AVLTreeScreen,
    GraphScreen
};
class myTexture {
public:
    Texture2D MainTexture;
    Texture2D SelectedTexture;

    float x, y;
    float width;
    float height;
    bool isLoaded = false;
    Color selectedColor = WHITE;

    myTexture(); // Constructor
    ~myTexture(); // Destructor
    void LoadTextureResources(const char *mainTexturePath, const char *selectedTexturePath); // Load textures
    
    void Drawtexture(); // Draw the texture
    bool CheckMouseCollision(); // Check if the mouse is over the texture
    void SetPosition(float xPos, float yPos); // Set position
    bool isPressed(); // Check if the button is pressed
};

class AnnouncementBox {
public:
    Rectangle rect;
    AnnouncementBox();
    AnnouncementBox(Rectangle rect, const char* title);
    
    void SetTitle(const char* title);
    void SetContent(const std::vector<char*>& content);
    void SetHighlightLines(int startLine, int endLine);
    
    void AddInfoLine(const char* label, const char* value);
    void ClearInfoLines();
    
    void Draw();
    
private:
    const char* title;
    vector<char*> content;
    int highlightStartLine;
    int highlightEndLine;
    
    struct InfoLine {
        const char* label;
        const char* value;
    };
    std::vector<InfoLine> infoLines;
    
    Color backgroundColor;
    Color borderColor;
    Color titleColor;
    Color textColor;
    Color highlightColor;
    Color highlightBgColor;
    
    float titleFontSize;
    float contentFontSize;
    float infoFontSize;
    float lineHeight;
    float indentation;
};

void display_title(const char *Title, ScreenID lastScreenID);
bool IsResourcesLoaded(); // Add function to check if resources are loaded

struct SpeedButtonSpinner {
    float value = 1.0f;
    const float minValue = 0.5f;
    const float maxValue = 4.0f;
    const float step = 0.5f;
    Vector2 position;

    void Init(Vector2 pos);   
    void Update();
    void Draw() const;
    float GetValue() const;
};

