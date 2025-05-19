#include "button.h"
#include "TextBox.h"
#include <vector>
#include <iostream>
#include <string.h>

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

void SpeedButtonSpinner::Init(Vector2 pos) {
    position = pos;
    Vector2 leftCenter = { position.x - 44, position.y - 7 };
    Vector2 rightCenter = { position.x + 44, position.y - 7 };
    addButton.SetPosition(rightCenter.x, rightCenter.y);
    minusButton.SetPosition(leftCenter.x, leftCenter.y);
}

void SpeedButtonSpinner::Update() {
    if (minusButton.isPressed()) value = std::max(minValue, value - step);
    if (addButton.isPressed()) value = std::min(maxValue, value + step);
}

void SpeedButtonSpinner::Draw() const {
    addButton.Drawtexture();
    minusButton.Drawtexture();
    char text[10];
    snprintf(text, sizeof(text), "x%.1f", value);
    Vector2 textSize = MeasureTextEx(customFont, text, 24, 1);
    Vector2 textPos = { position.x + 20 - textSize.x / 2, position.y + 15 - textSize.y / 2 };
    DrawTextEx(customFont, text, textPos, 24, 1, BLACK);
}

float SpeedButtonSpinner::GetValue() const {
    return value;
}

void MusicTexture::init() {
    music = LoadMusicStream("../resources/music/music.mp3");
    PlayMusicStream(music);
    checkMusic_button.LoadTextureResources("../resources/images/checkbox-normal.png",
                                           "../resources/images/checkbox-selected.png");
    checkMusic_activated.LoadTextureResources("../resources/images/checkbox-activated.png",
                                              "../resources/images/checkbox-activated.png");
    recSound = {(screenWidth - (screenWidth / 4)) / 2,
                (screenHeight - (screenHeight / 5)) / 3.4f,
                screenWidth / 4,
                screenHeight / 5};
    str = "SOUND: OFF";  
    checkMusic_button.SetPosition(recSound.x + recSound.width * 3 / 4, recSound.y + recSound.height / 2 - 10);
    checkMusic_activated.SetPosition(recSound.x + recSound.width * 3 / 4, recSound.y + recSound.height / 2 - 10);
}
void MusicTexture::draw() {
    DrawRectangleRounded(recSound, 0.5f, 16, {242, 239, 231, 255});
    if (isMusic) 
        checkMusic_activated.Drawtexture();
    else 
        checkMusic_button.Drawtexture();
    Vector2 textWidth = MeasureTextEx(SSLFont, str.c_str(), 22, 2);
    float textX = recSound.x + (recSound.width - textWidth.x) / 2;
    float textY = recSound.y + (recSound.height - textWidth.y) / 2;
    DrawTextEx(SSLFont, str.c_str(), {textX, textY + 3}, 22, 2, {219, 154, 0, 255});
}
void MusicTexture::update() {
    if (checkMusic_button.isPressed()) {
        isMusic = !isMusic;
        if (isMusic) {
            ResumeMusicStream(music); 
        } else {
            PauseMusicStream(music);   
        }
    }
    if (isMusic) {
        UpdateMusicStream(music);
    }
    std::string state = (isMusic) ? "ON" : "OFF";
    str = "SOUND: " + state;
}
void MusicTexture::unload() {
    UnloadMusicStream(music);
}

void ThemeTexture::init() {
    theme = lightMode; 
    lightOrdark = true;
    checkTheme_button.LoadTextureResources("../resources/images/checkbox-normal.png",
                                           "../resources/images/checkbox-selected.png");
    checkTheme_activated.LoadTextureResources("../resources/images/checkbox-activated.png",
                                              "../resources/images/checkbox-activated.png");
    recTheme = {
        (screenWidth - (screenWidth / 4)) / 2,
        (screenHeight - screenHeight / 5.0f) / 1.4f,
        screenWidth / 4,
        screenHeight / 5
    };
    str = "THEME: LIGHT";
    checkTheme_button.SetPosition(recTheme.x + recTheme.width * 3 / 4, recTheme.y + recTheme.height / 2 - 10);
    checkTheme_activated.SetPosition(recTheme.x + recTheme.width * 3 / 4, recTheme.y + recTheme.height / 2 - 10);
}

void ThemeTexture::draw() {
    DrawRectangleRounded(recTheme, 0.5f, 16, {242, 239, 231, 255});
    if (lightOrdark)
        checkTheme_button.Drawtexture();
    else
        checkTheme_activated.Drawtexture();
    Vector2 textWidth = MeasureTextEx(SSLFont, str.c_str(), 22, 2);
    float textX = recTheme.x + (recTheme.width - textWidth.x) / 2;
    float textY = recTheme.y + (recTheme.height - textWidth.y) / 2;
    DrawTextEx(SSLFont, str.c_str(), {textX, textY + 3}, 22, 2, {219, 154, 0, 255});
}

void ThemeTexture::update() {
    if (checkTheme_button.isPressed()) {
        lightOrdark = !lightOrdark;
        if (lightOrdark) {
            theme = lightMode;
        } else {
            theme = darkMode;
        }
        std::string state = (lightOrdark) ? "LIGHT" : "DARK";
        str = "THEME: " + state;
    }
}