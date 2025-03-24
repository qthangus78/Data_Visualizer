#include "Graph_Display.h"

Rectangle GraphDisplayScreen = {350, 60, 650, 600};
namespace Graph_display {
    graph* G = nullptr;
    Graph_Menu Gmenu;

    void Handle(Graph_Menu &menu, graph* &Graphs) {
        if (menu.confirmPressed && menu.selectedOption == Graph_Menu::CREATE) {
            int numNodes = 0;
            if (menu.randomSelected) {
                std::random_device rd;
                std::mt19937 gen(rd());
                numNodes = 10 + (gen() % 10);
            } else {
                try {
                    numNodes = std::stoi(menu.nodesInput);
                } catch (...) {
                    numNodes = 10;
                }
            }
            if (Graphs) {
                delete Graphs;
            }
            Graphs = GenerateRandomGraph(numNodes, menu.isDirected, menu.isWeighted);

            menu.confirmPressed = false;
            menu.nodesInput = "";
            menu.edgesInput = "";
            menu.randomSelected = false;
            menu.nodesBoxActive = false;
            menu.edgesBoxActive = false;
            menu.selectedOption = Graph_Menu::CREATE;

            // Reset textbox texts
            menu.nodesBox.text = "";
            menu.edgesBox.text = "";
        }
    }

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", 1);
        DrawRectangleRec(GraphDisplayScreen, {238, 241, 218, 255}); // rgb(238, 241, 218)

        Gmenu.Handle();
        Handle(Gmenu, G);
        Gmenu.Draw();
        RunGraphVisualization(G);
    }
}

// if (confirmPressed && selectedOption == CREATE) {
    //     int numNodes = 0;
    //     if (randomSelected) {
    //         std::random_device rd;
    //         std::mt19937 gen(rd());
    //         numNodes = 10 + (gen() % 10);
    //     } else {
    //         try {
    //             numNodes = std::stoi(nodesInput);
    //         } catch (...) {
    //             numNodes = 10;
    //         }
    //     }

    //     // // Generate the graph with the selected settings
    //     graph* G = GenerateRandomGraph(numNodes, isDirected, isWeighted);
    //     RunGraphVisualization(G);