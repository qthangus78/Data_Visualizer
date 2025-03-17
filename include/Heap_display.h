#pragma once
#include "GlobalVar.h"
#include "TextBox.h"

namespace Heap_display {
    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Heap" , 1);
    }
}