#include "Graph_Display.h"

Rectangle GraphDisplayScreen = {350, 60, 650, 600};
namespace Graph_display {
    graph* G = nullptr;
    Graph_Menu Gmenu;

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", 1);
        DrawRectangleRec(GraphDisplayScreen, {238, 241, 218, 255}); // rgb(238, 241, 218)

        Gmenu.Handle();
        Gmenu.MakeGraph(G);
        Gmenu.Draw();

        if (G && Gmenu.selectedOption == Graph_Menu::CREATE) {
            RunGraphVisualization(G);
        }
        if (G && Gmenu.selectedOption == Graph_Menu::MST_KRUSKAL) {
            RunGraphVisualization_MST(G);
            // vector<int> tmp = getMST(G);
            // for (int i : tmp) cout << i << ' '; cout << endl;
        }

    }
}
