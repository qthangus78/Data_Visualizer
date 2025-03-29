#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "graph.h"
#include "button.h"
#include "Graph_Menu.h"

void RunGraphVisualization(Graph* G);
void RunGraphVisualization_MST(Graph* G);
void RunGraphVisualization_DIJKSTRA(Graph* G);

namespace Graph_display {
    extern GraphMenu Gmenu;
    extern GraphVisualizer myGraphVisualizer;
    void Display();
}