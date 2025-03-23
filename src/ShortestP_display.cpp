#include "ShortestP_display.h"

namespace ShortestP_display {
    graph* G = nullptr;

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Shortest Path", 1);

        if (!G) {
            G = GenerateRandomGraph();
            initEadesFactor(G);
        }

        G->HandleMouseInteraction();
        G->BalanceGraph();
        G->Draw();

        // Display info
        // DrawText(("Iteration: " + std::to_string(G->iteration)).c_str(), 10, 10, 20, DARKGRAY);
        DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {10, 40}, 20, 1.0f, DARKGRAY);
        DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {10, 70}, 20, 1.0f, DARKGRAY);
        if (G->convergent) {
            DrawText("Converged!", 10, 100, 20, GREEN);
        }
    }
}