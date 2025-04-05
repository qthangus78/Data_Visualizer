#include "graph.h"
#include <algorithm>
#include <random>
#include <fstream>

float magnitude(Vector2 V) {
    return sqrtf(V.x * V.x + V.y * V.y);
}

void Clamp(float &val, float Min, float Max) {
    if (val < Min) val = Min;
    if (val > Max) val = Max;
}

// Graph Implementation
Graph::Node::Node(int _val, float x, float y) : val(_val), Pos({x, y}), Forces({0.0f, 0.0f}) {}

Graph::Edge::Edge(int _u, int _v, int _w) : u(_u), v(_v), w(_w) {}

int Graph::Edge::other(int x) const {
    return u ^ v ^ x;
}

Graph::Graph(int _numNode, bool _isDirected, bool _isWeighted)
    : numNode(_numNode), numEdge(0), isDirected(_isDirected), isWeighted(_isWeighted) {
    Nodes.resize(numNode + 1);
    for (int i = 1; i <= numNode; ++i) Nodes[i].val = i;
    Adjacent_list.resize(numNode + 1);
}

void Graph::AddEdge(int u, int v, int w) {
    Adjacent_list[u].push_back(Edges.size());
    Adjacent_list[v].push_back(Edges.size());

    Edges.push_back(Edge(u, v, w));
    ++numEdge;
}

float Graph::EulerDist(int u, int v) const {
    float dx = Nodes[u].Pos.x - Nodes[v].Pos.x;
    float dy = Nodes[u].Pos.y - Nodes[v].Pos.y;
    return sqrtf(dx * dx + dy * dy);
}

float Graph::EulerDist(int u, Vector2 pos) const {
    float dx = Nodes[u].Pos.x - pos.x;
    float dy = Nodes[u].Pos.y - pos.y;
    return sqrtf(dx * dx + dy * dy);
}

bool GraphVisualizer::checkGraph() const {
    return (graph != nullptr);
}

Vector2 GraphVisualizer::UnitVector(int u, int v) const {
    Vector2 direction = {
        graph->Nodes[v].Pos.x - graph->Nodes[u].Pos.x,
        graph->Nodes[v].Pos.y - graph->Nodes[u].Pos.y
    };
    float dist = magnitude(direction);
    if (dist == 0.0f) return {0.0f, 0.0f};
    return {direction.x / dist, direction.y / dist};
}

Vector2 GraphVisualizer::RepulsiveForce(int u, int v) const {
    float dist = graph->EulerDist(u, v);
    if (dist == 0.0f) return {0.0f, 0.0f};
    Vector2 direction = UnitVector(v, u);
    float forceMagnitude = Crep / powf(dist, 1.5f);
    return {forceMagnitude * direction.x, forceMagnitude * direction.y};
}

Vector2 GraphVisualizer::SpringForce(int u, int v) const {
    float dist = graph->EulerDist(u, v);
    if (dist == 0.0f) return {0.0f, 0.0f};
    Vector2 direction = UnitVector(u, v);
    float forceMagnitude = (dist > IdealEdgeLength) ? Cspring * logf(dist / IdealEdgeLength) : 0.0f;
    return {forceMagnitude * direction.x, forceMagnitude * direction.y};
}

Vector2 GraphVisualizer::CenteringForce(int u) const {
    Vector2 center = {
        GraphDisplayScreen.x + GraphDisplayScreen.width / 2.0f,
        GraphDisplayScreen.y + GraphDisplayScreen.height / 2.0f
    };
    Vector2 direction = {center.x - graph->Nodes[u].Pos.x, center.y - graph->Nodes[u].Pos.y};
    float dist = magnitude(direction);
    if (dist == 0.0f) return {0.0f, 0.0f};
    return {Ccenter * direction.x, Ccenter * direction.y};
}

void GraphVisualizer::BalanceGraph() {
    if (iteration >= numIteration || convergent) return;

    for (int u = 1; u <= graph->numNode; ++u) {
        if (u == selectedNode) continue;
        graph->Nodes[u].Forces = {0.0f, 0.0f};

        for (int v = 1; v <= graph->numNode; ++v) {
            if (v == u) continue;
            Vector2 repulsiveF = RepulsiveForce(u, v);
            graph->Nodes[u].Forces.x += repulsiveF.x;
            graph->Nodes[u].Forces.y += repulsiveF.y;
        }

        for (const int &EdgeID : graph->Adjacent_list[u]) {
            int v = graph->Edges[EdgeID].other(u);
            if (v == u) continue;
            Vector2 springF = SpringForce(u, v);
            graph->Nodes[u].Forces.x += springF.x;
            graph->Nodes[u].Forces.y += springF.y;
        }

        Vector2 centerF = CenteringForce(u);
        graph->Nodes[u].Forces.x += centerF.x;
        graph->Nodes[u].Forces.y += centerF.y;
    }

    float totalForce = 0.0f;
    for (int u = 1; u <= graph->numNode; ++u) {
        if (u == selectedNode) continue;
        totalForce += magnitude(graph->Nodes[u].Forces);
        graph->Nodes[u].Pos.x += graph->Nodes[u].Forces.x * currentVeclocity;
        graph->Nodes[u].Pos.y += graph->Nodes[u].Forces.y * currentVeclocity;

        Clamp(graph->Nodes[u].Pos.x, GraphDisplayScreen.x + nodeRadius, GraphDisplayScreen.x + GraphDisplayScreen.width - nodeRadius);
        Clamp(graph->Nodes[u].Pos.y, GraphDisplayScreen.y + nodeRadius, GraphDisplayScreen.y + GraphDisplayScreen.height - nodeRadius);
    }

    currentVeclocity *= CoolingFactor;
    ++iteration;
    if (totalForce < epsilon) convergent = true;
}

void GraphVisualizer::HandleMouseInteraction() {
    mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        selectedNode = -1;
        for (int u = 1; u <= graph->numNode; ++u) {
            float dist = graph->EulerDist(u, mouse);
            if (dist <= nodeRadius) {
                selectedNode = u;
                break;
            }
        }
    }

    if (selectedNode != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        graph->Nodes[selectedNode].Pos = mouse;
        Clamp(graph->Nodes[selectedNode].Pos.x, GraphDisplayScreen.x + nodeRadius, GraphDisplayScreen.x + GraphDisplayScreen.width - nodeRadius);
        Clamp(graph->Nodes[selectedNode].Pos.y, GraphDisplayScreen.y + nodeRadius, GraphDisplayScreen.y + GraphDisplayScreen.height - nodeRadius);
        graph->Nodes[selectedNode].Forces = {0.0f, 0.0f};
        initEadesFactor();
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        selectedNode = -1;
    }
}

void GraphVisualizer::DrawEdge(const Graph::Edge& edge, int special) const {
    int u = edge.u;
    int v = edge.v;

    bool uVisible = CheckCollisionPointRec(graph->Nodes[u].Pos, GraphDisplayScreen);
    bool vVisible = CheckCollisionPointRec(graph->Nodes[v].Pos, GraphDisplayScreen);
    if (!uVisible || !vVisible) return;

    Vector2 start = graph->Nodes[u].Pos;
    Vector2 end = graph->Nodes[v].Pos;

    Vector2 direction = {end.x - start.x, end.y - start.y};
    float dist = magnitude(direction);
    if (dist > 0) {
        direction = {direction.x / dist, direction.y / dist};
        start = {start.x + direction.x * nodeRadius, start.y + direction.y * nodeRadius};
        end = {end.x - direction.x * nodeRadius, end.y - direction.y * nodeRadius};
    }

    Color edgeColor;
    float thickness;
    switch (special) {
        case 2:  // Currently being visited
            edgeColor = RED;
            thickness = 3.0f;
            break;
        case 1:  // Part of Dijkstra tree
            edgeColor = BLUE;
            thickness = 2.0f;
            break;
        default: // Normal edge
            edgeColor = BLACK;
            thickness = 1.0f;
    }

    if (graph->isDirected) {
        if (dist > 0) {
            DrawLineEx(start, end, thickness, edgeColor);
            Vector2 arrow1 = {end.x - direction.x * 10 - direction.y * 5, end.y - direction.y * 10 + direction.x * 5};
            Vector2 arrow2 = {end.x - direction.x * 10 + direction.y * 5, end.y - direction.y * 10 - direction.x * 5};
            DrawLineEx(end, arrow1, thickness, edgeColor);
            DrawLineEx(end, arrow2, thickness, edgeColor);
        }
    } else {
        DrawLineEx(start, end, thickness, edgeColor);
    }

    if (graph->isWeighted) {
        Vector2 midPoint = {(start.x + end.x) / 2.0f, (start.y + end.y) / 2.0f};
        const char* weightText = TextFormat("%d", edge.w);
        float weightFontSize = nodeRadius * 1.2f;
        Vector2 textSize = MeasureTextEx(customFont, weightText, weightFontSize, 1.0f);

        Vector2 perpendicular = {-direction.y, direction.x};
        if (perpendicular.y > 0) {
            perpendicular.x *= -1.0f;
            perpendicular.y *= -1.0f;
        }
        float offset = 10.0f;
        Vector2 textPos = {
            midPoint.x + perpendicular.x * offset - textSize.x / 2.0f,
            midPoint.y + perpendicular.y * offset - textSize.y / 2.0f
        };

        Color textColor;
        switch (special) {
            case 2:
                textColor = RED;
                break;
            case 1:
                textColor = BLUE;
                break;
            default:
                textColor = Color{138, 178, 166, 255};
        }
        DrawTextEx(customFont, weightText, textPos, weightFontSize * (special ? 1.2f : 1.0f), 1.0f, textColor);
    }
}

void GraphVisualizer::DrawNode(int u, bool special) const {
    Color nodeColor;
    if (u == selectedNode) {
        nodeColor = Color{172, 211, 168, 255};
    } else if (special) {
        nodeColor = Color{165, 91, 75, 255};
    } else {
        nodeColor = Color{62, 63, 91, 255};
    }
    DrawCircleV(graph->Nodes[u].Pos, nodeRadius * (special ? 1.2f : 1.0f), nodeColor);

    const char* nodeText = TextFormat("%d", graph->Nodes[u].val);
    float nodeFontSize = nodeRadius * (special ? 1.4f : 1.2f);
    Vector2 textSize = MeasureTextEx(customFont, nodeText, nodeFontSize, 1.0f);
    DrawTextEx(customFont, nodeText,
               {graph->Nodes[u].Pos.x - textSize.x / 2, graph->Nodes[u].Pos.y - textSize.y / 2},
               nodeFontSize, 1.0f, WHITE);
}

void GraphVisualizer::Draw() const {
    for (const auto& edge : graph->Edges) {
        DrawEdge(edge, 0);
    }
    for (int u = 1; u <= graph->numNode; ++u) {
        DrawNode(u);
    }
}

void GraphVisualizer::DrawMST() const {
    std::vector<int> MST_edges = GraphAlgorithms::getMST(graph);
    for (int i = 0; i < graph->Edges.size(); ++i) {
        int special = 0;
        for (int id : MST_edges) {
            if (i == id) {
                special = 1;
                break;
            }
        }
        DrawEdge(graph->Edges[i], special);
    }
    for (int u = 1; u <= graph->numNode; ++u) {
        DrawNode(u);
    }
}

void GraphVisualizer::initEadesFactor() {
    iteration = 0;
    currentVeclocity = Veclocity;
    convergent = false;
}


// GraphAlgorithms Implementation
namespace GraphAlgorithms {
    int* par = nullptr;

    int root(int x, int* par) {
        if (par[x] < 0) return x;
        return par[x] = root(par[x], par);
    }

    bool unite(int u, int v, int* par) {
        u = root(u, par);
        v = root(v, par);
        if (u == v) return false;
        if (-par[u] < -par[v]) std::swap(u, v);
        par[u] += par[v];
        par[v] = u;
        return true;
    }

    std::vector<int> getMST(const Graph* G) {
        if (!G) return std::vector<int>();

        par = new int[G->numNode + 1];
        for (int i = 1; i <= G->numNode; ++i) par[i] = -1;

        std::vector<int> MST_Edges;
        std::vector<int> edgesList;
        for (int i = 0; i < G->Edges.size(); ++i) edgesList.push_back(i);
        std::sort(edgesList.begin(), edgesList.end(), [&](const int &id1, const int &id2) {
            return G->Edges[id1].w < G->Edges[id2].w;
        });

        for (const int &id : edgesList) {
            int u = G->Edges[id].u;
            int v = G->Edges[id].v;
            if (unite(u, v, par))
                MST_Edges.push_back(id);
        }
        delete[] par;
        return MST_Edges;
    }

    std::vector<int> getDIJKSTRA(const Graph* G, int src) {
        if (!G || G->numNode <= 0) {
            std::cout << "Invalid graph\n";
            return std::vector<int>();
        }

        if (src < 1 || src > G->numNode) {
            std::cout << "Invalid source vertex: src=" << src << ", numNode=" << G->numNode << "\n";
            return std::vector<int>();
        }

        std::vector<int> dist(G->numNode + 1, INT_MAX);
        std::vector<int> pre(G->numNode + 1, -1);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> MinHeap;

        dist[src] = 0;
        MinHeap.push({0, src});

        while (!MinHeap.empty()) {
            int d = MinHeap.top().first;
            int u = MinHeap.top().second;
            MinHeap.pop();

            if (d > dist[u]) continue;

            for (const int& id : G->Adjacent_list[u]) {
                if (G->isDirected && u != G->Edges[id].u) continue; // Skip if the edge is not directed from u
                int v = G->Edges[id].other(u);
                int weight = G->Edges[id].w;

                if (weight < 0) {
                    std::cout << "Negative edge weight detected: " << weight << ". Dijkstra's algorithm cannot handle negative weights.\n";
                    return std::vector<int>();
                }

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pre[v] = id;
                    MinHeap.push({dist[v], v});
                }
            }
        }

        // Collect all edges used in the shortest paths
        std::vector<int> PathEdges;
        for (int v = 1; v <= G->numNode; ++v) {
            if (v != src && pre[v] != -1) {
                PathEdges.push_back(pre[v]);
            }
        }

        return PathEdges;
    }
}

// Graph Generation Functions
std::random_device rd;
std::mt19937 gen(rd());

Graph* GenerateRandomGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted) {
    if (numNodes < 0 || numEdges < 0) return nullptr;

    int maxEdges = isDirected ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
    int minEdges = 0;

    if (numEdges < minEdges) {
        printf("Number of edges cannot be negative. Setting numEdges to a random value.\n");
        numEdges = gen() % numNodes;
    }
    if (numEdges > maxEdges) {
        printf("Number of edges (%d) exceeds maximum possible edges (%d). Setting numEdges to %d.\n", numEdges, maxEdges, maxEdges);
        numEdges = maxEdges;
    }

    Graph* myGraph = new Graph(numNodes, isDirected, isWeighted);

    std::uniform_real_distribution<float> disX(GraphDisplayScreen.x + myGraph->Nodes[1].Pos.x,
                                              GraphDisplayScreen.x + GraphDisplayScreen.width - myGraph->Nodes[1].Pos.x);
    std::uniform_real_distribution<float> disY(GraphDisplayScreen.y + myGraph->Nodes[1].Pos.y,
                                              GraphDisplayScreen.y + GraphDisplayScreen.height - myGraph->Nodes[1].Pos.y);
    for (int i = 1; i <= numNodes; ++i) {
        myGraph->Nodes[i].Pos = {disX(gen), disY(gen)};
    }

    if (numEdges > 0) {
        std::vector<std::pair<int, int>> possibleEdges;
        for (int u = 1; u <= numNodes; ++u) {
            for (int v = (isDirected ? 1 : u + 1); v <= numNodes; ++v) {
                if (u == v) continue;
                possibleEdges.push_back({u, v});
            }
        }

        std::shuffle(possibleEdges.begin(), possibleEdges.end(), gen);

        int edgesAdded = 0;
        for (const auto& edge : possibleEdges) {
            if (edgesAdded >= numEdges) break;
            int u = edge.first;
            int v = edge.second;

            bool edgeExists = false;
            for (const int &EdgeID : myGraph->Adjacent_list[u]) {
                int otherNode = myGraph->Edges[EdgeID].other(u);
                if (otherNode == v) {
                    edgeExists = true;
                    break;
                }
            }
            if (edgeExists) continue;

            int weight = 1 + (gen() % (numNodes + numEdges));
            myGraph->AddEdge(u, v, weight);
            edgesAdded++;
        }
    }

    return myGraph;
}

Graph* GenerateRandomConnectedGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted) {
    if (numNodes <= 0 || numEdges < 0) return nullptr;

    int maxEdges = isDirected ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
    int minEdges = numNodes - 1;

    if (numEdges < minEdges) numEdges = minEdges;
    if (numEdges > maxEdges) numEdges = maxEdges;

    Graph* myGraph = new Graph(numNodes, isDirected, isWeighted);

    std::uniform_real_distribution<float> disX(GraphDisplayScreen.x + myGraph->Nodes[1].Pos.x,
                                              GraphDisplayScreen.x + GraphDisplayScreen.width - myGraph->Nodes[1].Pos.x);
    std::uniform_real_distribution<float> disY(GraphDisplayScreen.y + myGraph->Nodes[1].Pos.y,
                                              GraphDisplayScreen.y + GraphDisplayScreen.height - myGraph->Nodes[1].Pos.y);
    for (int i = 1; i <= numNodes; ++i) {
        myGraph->Nodes[i].val = i;
        myGraph->Nodes[i].Pos = {disX(gen), disY(gen)};
    }

    std::vector<int> nodes(numNodes);
    for (int i = 0; i < numNodes; ++i) nodes[i] = i + 1;
    std::shuffle(nodes.begin(), nodes.end(), gen);

    std::vector<bool> inTree(numNodes + 1, false);
    inTree[nodes[0]] = true;
    int edgesAdded = 0;

    for (int i = 1; i < numNodes; ++i) {
        int v = nodes[i];
        std::vector<int> treeNodes;
        for (int j = 1; j <= numNodes; ++j) {
            if (inTree[j]) treeNodes.push_back(j);
        }
        std::uniform_int_distribution<> dis(0, treeNodes.size() - 1);
        int u = treeNodes[dis(gen)];

        int weight = (gen() % 10);
        myGraph->AddEdge(u, v, weight);
        inTree[v] = true;
        edgesAdded++;
    }

    int remainingEdges = numEdges - edgesAdded;
    if (remainingEdges > 0) {
        std::vector<std::pair<int, int>> possibleEdges;
        for (int u = 1; u <= numNodes; ++u) {
            for (int v = (isDirected ? 1 : u + 1); v <= numNodes; ++v) {
                if (u == v) continue;
                bool edgeExists = false;
                for (const int &EdgeID : myGraph->Adjacent_list[u]) {
                    int otherNode = myGraph->Edges[EdgeID].other(u);
                    if (otherNode == v) {
                        edgeExists = true;
                        break;
                    }
                }
                if (!edgeExists) {
                    possibleEdges.push_back({u, v});
                }
            }
        }

        std::shuffle(possibleEdges.begin(), possibleEdges.end(), gen);

        for (const auto& edge : possibleEdges) {
            if (remainingEdges <= 0) break;
            int u = edge.first;
            int v = edge.second;

            int weight = isWeighted ? (1 + (gen() % 10)) : 1;
            myGraph->AddEdge(u, v, weight);
            remainingEdges--;
        }
    }

    return myGraph;
}


void Handle_InputFile(const char* filePath, Graph* &G) {
    std::fstream fin(filePath);
    if (!fin.is_open()) {
        std::cout << "Cannot Open File\n";
        return;
    }
    if (G) delete G;

    int numNodes, numEdges;
    fin >> numNodes >> numEdges;
    G = new Graph(numNodes, false, true);

    std::uniform_real_distribution<float> disX(GraphDisplayScreen.x + G->Nodes[1].Pos.x,
                                              GraphDisplayScreen.x + GraphDisplayScreen.width - G->Nodes[1].Pos.x);
    std::uniform_real_distribution<float> disY(GraphDisplayScreen.y + G->Nodes[1].Pos.y,
                                              GraphDisplayScreen.y + GraphDisplayScreen.height - G->Nodes[1].Pos.y);
    for (int i = 1; i <= numNodes; ++i) {
        G->Nodes[i].Pos = {disX(gen), disY(gen)};
    }

    for (int i = 1; i <= numEdges; ++i) {
        int u, v, w;
        fin >> u >> v >> w;
        G->AddEdge(u, v, w);
    }

    fin.close();
}
