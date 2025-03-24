#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "graph.h"
#include "button.h"
#include "Graph_Menu.h"

namespace Graph_display {
    extern graph* G;
    extern Graph_Menu Gmenu;

    void Handle(Graph_Menu &menu, graph* &Graphs);
    void Display();
}