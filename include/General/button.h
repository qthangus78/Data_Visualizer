#pragma once 
#include <raylib.h>

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

    myTexture();
    void LoadTextureResources(const char *mainTexturePath, const char *selectedTexturePath);
    void Drawtexture();
    bool CheckMouseCollision();
};

void display_title(const char *Title, ScreenID lastScreenID);
bool IsResourcesLoaded(); // Add function to check if resources are loaded