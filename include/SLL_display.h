#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "button.h"

namespace SLL_display {
    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Singly Linked List", 1);
    }
}