#ifndef GRAPH_H
#define GRAPH_H

#include "raylib.h"
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

    graph(int _numNode);

    void AddEdge(int u, int v, int w);
    float EulerDist(int u, int v);
    Vector2 UnitVector(int u, int v);
    Vector2 RepulsiveForce(int u, int v);
    Vector2 SpringForce(int u, int v);
    Vector2 CenteringForce(int u);  // New: Centering force
    void BalanceGraph();
    void Draw();

    // Graph properties
    int numNode, numEdge;
    vector<node> Nodes;
    vector<std::vector<int>> Adjacent_list;
    vector<edge> Edges;

    // Eades algorithm parameters
    float Crep = 10.0f;          // Increased repulsion constant (C3)
    float Cspring = 2.0f;        // Spring constant (C1)
    float IdealEdgeLength = 50.0f;  // Will be scaled dynamically (C2)
    float Ccenter = 0.001f;      // Centering force constant
    float Veclocity = 0.1f;      // Initial velocity (C4)
    float currentVeclocity = Veclocity;
    float CoolingFactor = 0.98f; // Slower cooling for more spreading
    float epsilon = 0.005f;      // Stricter convergence criterion
    int iteration = 0;
    int numIteration = 200;      // More iterations for better distribution
    bool convergent = false;

    // Visualization parameters
    float nodeRadius = 10.0f;
    // float screenWidth = 1000.0f;
    // float screenHeight = 700.0f;
};

graph* GenerateRandomGraph();
void initGraph(graph* G);

#endif