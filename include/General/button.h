#pragma once 
#include <raylib.h>
#include <string>

enum class ScreenID {
    StartScreen,
    StartMenuScreen,
    SettingScreen,
    SLLScreen,
    HeapScreen,
    TrieScreen,
    GraphScreen,
    AVLScreen
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

class MusicTexture {
private:
    Music music;
    bool isMusic;
    Rectangle recSound;
    myTexture checkMusic_button;
    myTexture checkMusic_activated;
    std::string str;
public:
    void init();
    void unload();
    void draw();
    void update();
};

class ThemeTexture {
private:
    Color theme;
    bool lightOrdark;
    Rectangle recTheme;
    myTexture checkTheme_button;
    myTexture checkTheme_activated;
    std::string str;
public:
    void init();
    void draw();
    void update();
    Color getTheme() const { return theme; } 
};
    

void display_title(const char *Title, ScreenID lastScreenID);
bool IsResourcesLoaded(); // Add function to check if resources are loaded


