#include "Heap_display.h"

void Heap_display::Display() {
    ClearBackground({192, 245, 242, 100});
    display_title("Min Heap", ScreenID::StartMenuScreen);
    if (heapInstance) {
        heapInstance->update();
        heapInstance->draw();
    }
}