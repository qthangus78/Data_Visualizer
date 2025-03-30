#ifndef GRAPH_H
#define GRAPH_H

#include "GlobalVar.h"
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

    Graph(int _numNode, bool _isDirected = false, bool _isWeighted = false);

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
private:
    int selectedNode = -1;
    float nodeRadius = 15.0f;

    // Eades algorithm parameters
    float Crep = 1000.0f;
    float Cspring = 1.5f;
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
    void DrawEdge(const Graph::Edge& edge, bool special = false) const;
    void DrawNode(int u) const;
};

namespace GraphAlgorithms {
    std::vector<int> getMST(const Graph* G);
    std::vector<int> getDIJKSTRA(const Graph* G, int src);
}

Graph* GenerateRandomGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted);
Graph* GenerateRandomConnectedGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted);
void Handle_InputFile(const char* filePath, Graph* &G);

#endif