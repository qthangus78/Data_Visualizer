#pragma once 
#include <raylib.h>
#include <bits/stdc++.h>

// using namespace std;

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

void display_title(const char *Title, ScreenID lastScreenID);
bool IsResourcesLoaded(); // Add function to check if resources are loaded