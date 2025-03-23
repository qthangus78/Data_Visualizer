#include "Graph_Display.h"

namespace Graph_display {
    graph* G = nullptr;
    Graph_Menu Gmenu;

    void Handle(Graph_Menu menu, graph* &Graphs) {
        if (Graphs != nullptr) return;

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
            Graphs = GenerateRandomGraph(numNodes, menu.isDirected, menu.isWeighted);
        }
    }

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", 1);

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