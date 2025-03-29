#include "Graph_Display.h"


void RunGraphVisualization(GraphVisualizer &visualizer) {
    if (!visualizer.checkGraph()) return;
    visualizer.HandleMouseInteraction();
    visualizer.BalanceGraph();
    visualizer.Draw();

    // DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {900, 10}, 20, 1.0f, DARKGRAY);
    // DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {900, 40}, 20, 1.0f, DARKGRAY);
}

void RunGraphVisualization_MST(GraphVisualizer &visualizer) {
    if (!visualizer.checkGraph()) return;
    visualizer.HandleMouseInteraction();
    visualizer.BalanceGraph();
    visualizer.DrawMST();

    // DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {900, 10}, 20, 1.0f, DARKGRAY);
    // DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {900, 40}, 20, 1.0f, DARKGRAY);
}

void RunGraphVisualization_DIJKSTRA(GraphVisualizer &visualizer) {
    if (!visualizer.checkGraph()) return;
    visualizer.HandleMouseInteraction();
    visualizer.BalanceGraph();
    visualizer.DrawDIJKSTRA();

    // DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {900, 10}, 20, 1.0f, DARKGRAY);
    // DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {900, 40}, 20, 1.0f, DARKGRAY);
}


namespace Graph_display {
    GraphMenu Gmenu;
    GraphVisualizer myGraphVisualizer;

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", 1);
        DrawRectangleRec(GraphDisplayScreen, {238, 241, 218, 255}); // rgb(51, 145, 154)

        Gmenu.Handle(myGraphVisualizer);
        Gmenu.Draw();

        if (myGraphVisualizer.checkGraph()) {
            if (Gmenu.selectedOption == GraphMenu::MenuOption::CREATE) {
                RunGraphVisualization(myGraphVisualizer);
            }
            if (Gmenu.selectedOption == GraphMenu::MenuOption::MST_KRUSKAL) {
                RunGraphVisualization_MST(myGraphVisualizer);
            }
            if (Gmenu.selectedOption == GraphMenu::MenuOption::DIJKSTRA) {
                RunGraphVisualization_DIJKSTRA(myGraphVisualizer);
            }
        }
    }
}
