#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "button.h"
#include "LinkedList.h"

namespace SLL_display {
    inline SSL* sslInstance = nullptr;  // Dùng inline để tránh lỗi link-time
    void Display();
}