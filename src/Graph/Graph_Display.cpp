#include "Graph_Display.h"

namespace Graph_display {
    graph* G = nullptr;
    Graph_Menu Gmenu;

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", 1);
        DrawRectangleRec(GraphDisplayScreen, {238, 241, 218, 255}); // rgb(51, 145, 154)

        Gmenu.Handle(G);
        Gmenu.Draw();

        if (G && Gmenu.selectedOption == Graph_Menu::CREATE) {
            RunGraphVisualization(G);
        }
        if (G && Gmenu.selectedOption == Graph_Menu::MST_KRUSKAL) {
            RunGraphVisualization_MST(G);
        }
        if (G && Gmenu.selectedOption == Graph_Menu::DIJKSTRA) {
            RunGraphVisualization_DIJKSTRA(G);
        }
    }
}
