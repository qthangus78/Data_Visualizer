#include "SLL_display.h"

void SLL_display::Display() {
    display_title("Singly Linked List", ScreenID::StartMenuScreen);
    if (sslInstance) {
        sslInstance->handle();
        sslInstance->draw();
    }
    ClearBackground(theme.getTheme());
}
