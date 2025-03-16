#include "GlobalVar.h"
#include <raylib.h>
//--------------------------------
// Khai báo biến toàn cục
//--------------------------------
namespace color {
    Color buttonColor        = BLUE;
    Color buttonColorHovered = {204, 119, 34, 255};
    Color buttonFile         = GRAY;
    Color buttonFileHovered  = DARKGRAY;
    Color nodeNotInMode      = BLUE;
    Color nodeRendered       = {204, 119, 34, 255};
    Color edgeNotInMode      = BLACK;
    Color edgeRendered       = {204, 119, 34, 255};
}
namespace buttonVar {
    button buttonIns   = {{50.0f, 440.0f, button::sizeW, button::sizeH}, color::buttonColor, "Insert"};
    button buttonDel   = {{50.0f, 500.0f, button::sizeW, button::sizeH}, color::buttonColor, "Delete"};
    button buttonF     = {{50.0f, 560.0f, button::sizeW, button::sizeH}, color::buttonColor, "Find"};
    button buttonClear = {{50.0f, 620.0f, button::sizeW, button::sizeH}, color::buttonColor, "Clear"};
}
Edge EArrow = {100.0f, 5.0f, 20.0f};

Vector2 startLinkedListPos = {50,180};