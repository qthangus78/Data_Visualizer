#pragma once

#include "GlobalVar.h"
using namespace std;
#include <vector>
#include <queue>

class Graph {
public:
    struct Node {
        int val;
        Vector2 Pos;
        Vector2 Forces;
        Node(int _val = 0, float x = 0.0f, float y = 0.0f);
    };

    struct Edge {
        int u, v, w;
        Edge(int _u = 0, int _v = 0, int _w = 0);
        int other(int x) const;
    };

    Graph(int _numNode, bool _isDirected = false, bool _isWeighted = true);

    void AddEdge(int u, int v, int w);
    float EulerDist(int u, int v) const;
    float EulerDist(int u, Vector2 pos) const;

    // Graph properties
    int numNode, numEdge;
    std::vector<Node> Nodes;
    std::vector<std::vector<int>> Adjacent_list;
    std::vector<Edge> Edges;
    bool isDirected;
    bool isWeighted;

    // Add a member to store the source node for Dijkstra's algorithm
    int dijkstraSource = -1;
};

struct DijkstraStepResult {
    enum Action { INIT, SELECT_U, RELAX_EDGE, DONE };
    Action action;
    int u, v;
};

class GraphVisualizer {
public:
    GraphVisualizer(){}
    
    Graph* graph = nullptr;

    bool checkGraph() const;
    void BalanceGraph();
    void HandleMouseInteraction();
    void Draw() const;
    void DrawMST() const;
    void DrawDIJKSTRA() const;
    void initEadesFactor();

    // Add Dijkstra visualization methods
    void initDijkstra();
    void DrawDIJKSTRA_StepByStep();


    // Add MST visualization methods
    void DrawMST_StepByStep();
    void initKruskal();
    bool isKruskalInited() const; 

private:
    int selectedNode = -1;
    float nodeRadius = 15.0f;

    // Eades algorithm parameters
    float Crep = 1000.0f;
    float Cspring = 1.0f;
    float IdealEdgeLength = 150.0f;
    float Ccenter = 0.005f;
    float Veclocity = 0.89f;
    float currentVeclocity = Veclocity;
    float CoolingFactor = 0.95f;
    float epsilon = 0.01f;
    int iteration = 0;
    int numIteration = 200;
    bool convergent = false;

    Vector2 UnitVector(int u, int v) const;
    Vector2 RepulsiveForce(int u, int v) const;
    Vector2 SpringForce(int u, int v) const;
    Vector2 CenteringForce(int u) const;
    void DrawEdge(const Graph::Edge& edge, int special = 0) const;
    void DrawNode(int u, bool special = false) const;
    void DrawDijkstraPseudoCode();  // Add this line

    // Add Dijkstra visualization properties
    int selectNextDijkstraU();
    void GetDijkstraStep();
    struct DijkstraVisualizerData {
        vector<int> distances;
        vector<bool> processed;
        int current_u = -1;
        int current_neighbor_index = 0;
        bool first_step = true;
        vector<char*> codes;

        DijkstraStepResult step_result = {DijkstraStepResult::INIT};
        float elapsed_time = 0.0f;
        const float step_interval = 1.0f;
        bool is_paused = false; 
        bool inited = false;
        DijkstraVisualizerData();
    } dijkstra_data;

   

    // Add Kruskal visualization properties
    void GetKruskalStep();
    int findSet(int v);
    bool unionSets(int a, int b);
    struct KruskalVisualizerData {
        vector<int> parent;
        vector<pair<int, int>> edges;  // {weight, edge_index}
        int current_edge_index = -1;
        bool first_step = true;
        vector<bool> in_mst;
        vector<char*> codes;

        float elapsed_time = 0.0f;
        const float step_interval = 1.0f;
        bool is_paused = false;
        bool inited = false;
        KruskalVisualizerData();
    } kruskal_data;


    // Add announcement box for algorithms
    AnnouncementBox algorithmBox;
    void UpdateDijkstraInfoLines();
};

namespace GraphAlgorithms {
    std::vector<int> getMST(const Graph* G);
    std::vector<int> getDIJKSTRA(const Graph* G, int src);
}


Graph* GenerateRandomGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted);
Graph* GenerateRandomConnectedGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted);
void Handle_InputFile(const char* filePath, Graph* &G);
