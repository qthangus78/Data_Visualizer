#pragma once
#include "GlobalVar.h"
#include "graph.h"

float displayVeclocity;

struct DijkstraStepResult {
    enum Action { INIT, SELECT_U, RELAX_EDGE, DONE };
    Action action;
    int u, v;
};

class DijkstraVisualizer {
public:
    Graph* graph;
    vector<int> distances;
    vector<bool> processed;
    int current_u, current_neighbor_index;
    bool first_step;
        
    DijkstraVisualizer(Graph* _graph); 
    DijkstraStepResult step(); 

private:
    int selectNextU();
    std::vector<char*> code = {
        "for each vertex v in Graph:",
        "    distance[v] := infinity",
        "    previous[v] := undefined",
        "distance[source] := 0",
        "Q := the set of all vertices in Graph",
        "while Q is not empty:",
        "    u := vertex in Q with smallest distance[]",
        "    remove u from Q",
        "    for each neighbor v of u:",
        "        alt := distance[u] + length(u, v)",
        "        if alt < distance[v]:",
        "            distance[v] := alt",
        "            previous[v] := u"
    };
};


struct KruskalStepResult {
    enum Action { INIT, CONSIDER_EDGE, ADD_EDGE, REJECT_EDGE, DONE };
    Action action;
    int u, v; // Vertices of the current edge
};
