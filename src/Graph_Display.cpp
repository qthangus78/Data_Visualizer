#include "Graph_Display.h"

void RunGraphVisualization(graph* G) {
    if (!G) return;
    initEadesFactor(G);
    G->HandleMouseInteraction();
    G->BalanceGraph();
    G->Draw();

    // Display info
    DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {10, 40}, 20, 1.0f, DARKGRAY);
    DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {10, 70}, 20, 1.0f, DARKGRAY);
    if (G->convergent) {
        DrawText("Converged!", 10, 100, 20, GREEN);
    }
}

namespace Graph_display {
    graph* G = nullptr;

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", 1);

        if (!G) {
            G = GenerateRandomGraph(10, 1, 1);
        }
        RunGraphVisualization(G);
    }
}