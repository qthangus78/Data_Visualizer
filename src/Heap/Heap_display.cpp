#include "Heap_display.h"

void Heap_display::Display() {
    display_title("Min Heap", ScreenID::StartMenuScreen);
    if (heapInstance) {
        heapInstance->update();
        heapInstance->draw();
    }
    ClearBackground({192, 245, 242, 100});
}