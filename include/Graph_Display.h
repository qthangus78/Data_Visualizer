#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "graph.h"
#include "button.h"

void RunGraphVisualization(graph* G);

namespace Graph_display {
    extern graph* G;
    void Display();
}