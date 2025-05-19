#include "AVLTree_display.h"

void AVLTree_display::Display() {
    if (avlInstance) {
        avlInstance->handle();
        avlInstance->draw();
    }
    display_title("AVL Tree", ScreenID::StartMenuScreen);
    ClearBackground(theme.getTheme());
}