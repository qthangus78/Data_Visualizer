#include "Heap_display.h"

void Heap_display::Display() {
    display_title("Min Heap", 1);
    if (heapInstance) {
        heapInstance->update();
        heapInstance->draw();
    }
    ClearBackground({192, 245, 242, 100});
}