#include "SLL_display.h"

void SLL_display::Display() {
    display_title("Singly Linked List", 1);
    if (sslInstance) {
        sslInstance->handle();
        sslInstance->draw();
    }
    ClearBackground({192, 245, 242, 100});
}
