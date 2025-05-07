#include "AVLTree_display.h"

void AVLTree_display::Display() {
    display_title("AVL Tree", ScreenID::StartMenuScreen);
    if (avlInstance) {
        avlInstance->handle();
        avlInstance->draw();
    }
    ClearBackground({192, 245, 242, 100});
}