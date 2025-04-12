#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "graph.h"
#include "button.h"
#include "Graph_Menu.h"

void RunGraphVisualization(GraphVisualizer &GV);
void RunGraphVisualization_MST(GraphVisualizer &GV);
void RunGraphVisualization_DIJKSTRA(GraphVisualizer &GV);

namespace Graph_display {
    extern GraphMenu Gmenu;
    extern GraphVisualizer myGraphVisualizer;

    extern void SetUpTextureButtons();
    extern void DrawTextureButtons();
    extern void Display();
}