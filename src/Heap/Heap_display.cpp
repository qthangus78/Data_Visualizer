#include "Heap_display.h"

void Heap_display::Display() {
    ClearBackground(theme.getTheme());
    display_title("Min Heap", ScreenID::StartMenuScreen);
    if (heapInstance) {
        heapInstance->update();
        heapInstance->draw();
    }
}