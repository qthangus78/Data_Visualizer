#ifndef GRAPH_H
#define GRAPH_H

#include "GlobalVar.h"  

class graph {
public:
    struct node {
        int val;
        Vector2 Pos;
        Vector2 Forces;
        node(int _val = 0, float x = 0.0f, float y = 0.0f);
    };

    struct edge {
        int u, v, w;
        edge(int _u = 0, int _v = 0, int _w = 0);
        int other(int x);
    };

    graph(int _numNode, bool _isDirected = false, bool _isWeighted = false);

    void AddEdge(int u, int v, int w);
    float EulerDist(int u, int v);
    float EulerDist(int u, Vector2 pos);
    Vector2 UnitVector(int u, int v);
    Vector2 RepulsiveForce(int u, int v);
    Vector2 SpringForce(int u, int v);
    Vector2 CenteringForce(int u);
    void BalanceGraph();
    void DrawEdge(const edge& edge, bool special = 0);
    void DrawNode(int u); 
    void Draw();
    void DrawMST();
    void DrawDIJKSTRA(int src, int dest);
    void HandleMouseInteraction();

    // Graph properties
    int numNode, numEdge;
    std::vector<node> Nodes;
    std::vector<std::vector<int>> Adjacent_list;
    std::vector<edge> Edges;

    // Graph settings
    bool isDirected;   // True for directed, false for undirected
    bool isWeighted;   // True for weighted, false for unweighted
    pair<int, int> DIJKSTRA_parameters; /// source and dest for Dijkstra

    // Eades algorithm parameters
    float Crep = 100.0f;
    float Cspring = 1.5f;
    float IdealEdgeLength = 50.0f;
    float Ccenter = 0.005f;
    float Veclocity = 0.89f;
    float currentVeclocity = Veclocity;
    float CoolingFactor = 0.95f;
    float epsilon = 0.01f;
    int iteration = 0;
    int numIteration = 200;
    bool convergent = false;

    // Visualization parameters
    float nodeRadius = 15.0f;

    // Mouse interaction
    int selectedNode = -1;  // Index of the node being dragged (-1 if none)
    Rectangle DisplayScreen = {400, 60, 600, 600};
};

graph* GenerateRandomGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted);
graph* GenerateRandomConnectedGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted);
void initEadesFactor(graph* G);
void RunGraphVisualization(graph* G);
void RunGraphVisualization_MST(graph* G);
void RunGraphVisualization_DIJKSTRA(graph* G);
std::vector<int> getMST(graph *G);
std::vector<int> getDIJKSTRA(graph *G, int src, int dest);
void Handle_InputFile(const char* filePath, graph* &G);

#endif