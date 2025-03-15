#pragma once
#include "Global.h"
#include "TextBox.h"

namespace ShortestP_display {
    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Shortest Path", 1);
    }
}