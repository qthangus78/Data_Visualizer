#include "graph.h"

float magnitude(Vector2 V) {
    return sqrtf(V.x * V.x + V.y * V.y);
}

void Clamp(float &val, float Min, float Max) {
    if (val < Min) val = Min;
    if (val > Max) val = Max;
}

graph::node::node(int _val, float x, float y) {
    val = _val;
    Pos = {x, y};
    Forces = {0.0f, 0.0f};
}

graph::edge::edge(int _u, int _v, int _w) {
    u = _u;
    v = _v;
    w = _w;
}

graph::graph(int _numNode, bool _isDirected, bool _isWeighted) {
    numNode = _numNode;
    numEdge = 0;
    Nodes.resize(numNode + 1, node());
    Adjacent_list.resize(numNode + 1);

    // Set graph properties
    isDirected = _isDirected;
    isWeighted = _isWeighted;

    // Dynamically scale IdealEdgeLength based on DisplayScreen size and number of nodes
    IdealEdgeLength = sqrtf(DisplayScreen.width * DisplayScreen.height / (float)numNode) * 0.5f;
    // Scale Crep more aggressively based on number of nodes
    Crep = numNode * numNode * 10.0f;
}

int graph::edge::other(int x) {
    return u ^ v ^ x;
}

void graph::AddEdge(int u, int v, int w) {
    // Add edge from u to v
    Adjacent_list[u].push_back(Edges.size());
    // For undirected graph, also add edge from v to u
    if (!isDirected) {
        Adjacent_list[v].push_back(Edges.size());
    }
    Edges.push_back(edge(u, v, w));
    ++numEdge;
}

float graph::EulerDist(int u, int v) {
    float dx = Nodes[u].Pos.x - Nodes[v].Pos.x;
    float dy = Nodes[u].Pos.y - Nodes[v].Pos.y;
    return sqrtf(dx * dx + dy * dy);
}

float graph::EulerDist(int u, Vector2 pos) {
    float dx = Nodes[u].Pos.x - pos.x;
    float dy = Nodes[u].Pos.y - pos.y;
    return sqrtf(dx * dx + dy * dy);
}

Vector2 graph::UnitVector(int u, int v) {
    Vector2 direction = {
        Nodes[v].Pos.x - Nodes[u].Pos.x,
        Nodes[v].Pos.y - Nodes[u].Pos.y
    };
    float dist = magnitude(direction);
    if (dist == 0.0f) return {0.0f, 0.0f};
    return {direction.x / dist, direction.y / dist};
}

Vector2 graph::RepulsiveForce(int u, int v) {
    float dist = EulerDist(u, v);
    if (dist == 0.0f) return {0.0f, 0.0f};
    Vector2 direction = UnitVector(v, u);  // Direction from v to u (repulsive)
    float forceMagnitude = Crep / powf(dist, 1.5f);
    return {forceMagnitude * direction.x, forceMagnitude * direction.y};
}

Vector2 graph::SpringForce(int u, int v) {
    float dist = EulerDist(u, v);
    if (dist == 0.0f) return {0.0f, 0.0f};
    Vector2 direction = UnitVector(u, v);  // Direction from u to v (attractive)
    float forceMagnitude = (dist > IdealEdgeLength) ? Cspring * logf(dist / IdealEdgeLength) : 0.0f;
    return {forceMagnitude * direction.x, forceMagnitude * direction.y};
}

Vector2 graph::CenteringForce(int u) {
    // Center of the DisplayScreen rectangle
    Vector2 center = {
        DisplayScreen.x + DisplayScreen.width / 2.0f,
        DisplayScreen.y + DisplayScreen.height / 2.0f
    };
    Vector2 direction = {center.x - Nodes[u].Pos.x, center.y - Nodes[u].Pos.y};
    float dist = magnitude(direction);
    if (dist == 0.0f) return {0.0f, 0.0f};
    return {Ccenter * direction.x, Ccenter * direction.y};
}

void graph::BalanceGraph() {
    if (convergent) return;

    // Traverse all nodes
    for (int u = 1; u <= numNode; ++u) {
        // Skip the selected node (being dragged)
        if (u == selectedNode) continue;

        Nodes[u].Forces = {0.0f, 0.0f};

        // Calculate repulsive force from all other nodes
        for (int v = 1; v <= numNode; ++v) {
            if (v == u) continue;
            Vector2 repulsiveF = RepulsiveForce(u, v);
            Nodes[u].Forces.x += repulsiveF.x;
            Nodes[u].Forces.y += repulsiveF.y;
        }

        // Calculate attractive force from adjacent nodes
        for (const int &EdgeID : Adjacent_list[u]) {
            int v = Edges[EdgeID].other(u);
            if (v == u) continue;
            Vector2 springF = SpringForce(u, v);
            Nodes[u].Forces.x += springF.x;
            Nodes[u].Forces.y += springF.y;
        }

        // Add centering force
        Vector2 centerF = CenteringForce(u);
        Nodes[u].Forces.x += centerF.x;
        Nodes[u].Forces.y += centerF.y;
    }

    // Calculate new positions and check for convergence
    float totalForce = 0.0f;
    for (int u = 1; u <= numNode; ++u) {
        // Skip the selected node
        if (u == selectedNode) continue;

        totalForce += magnitude(Nodes[u].Forces);
        Nodes[u].Pos.x += Nodes[u].Forces.x * currentVeclocity;
        Nodes[u].Pos.y += Nodes[u].Forces.y * currentVeclocity;

        // Clamp node positions to stay within DisplayScreen
        Clamp(Nodes[u].Pos.x, DisplayScreen.x + nodeRadius, DisplayScreen.x + DisplayScreen.width - nodeRadius);
        Clamp(Nodes[u].Pos.y, DisplayScreen.y + nodeRadius, DisplayScreen.y + DisplayScreen.height - nodeRadius);
    }

    // Apply cooling factor
    currentVeclocity *= CoolingFactor;

    // ++iteration;
    // cout << "iteration: " << iteration << endl;

    // Check for convergence
    if (totalForce < epsilon) {
        convergent = true;
    }
}

void graph::HandleMouseInteraction() {
    // Update global mouse position
    mouse = GetMousePosition();

    // Check for mouse button down to select a node
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        selectedNode = -1;  // Reset selection
        for (int u = 1; u <= numNode; ++u) {
            float dist = EulerDist(u, mouse);
            if (dist <= nodeRadius) {
                selectedNode = u;
                break;
            }
        }
    }

    // Update the position of the selected node while the mouse button is held
    if (selectedNode != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Nodes[selectedNode].Pos = mouse;

        // Keep the node within DisplayScreen bounds
        Clamp(Nodes[selectedNode].Pos.x, DisplayScreen.x + nodeRadius, DisplayScreen.x + DisplayScreen.width - nodeRadius);
        Clamp(Nodes[selectedNode].Pos.y, DisplayScreen.y + nodeRadius, DisplayScreen.y + DisplayScreen.height - nodeRadius);

        // Reset forces to prevent jittering while dragging
        Nodes[selectedNode].Forces = {0.0f, 0.0f};
        initEadesFactor(this);
    }

    // Release the node when the mouse button is released
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        selectedNode = -1;
    }
}

void graph::DrawEdge(const edge& edge, bool special) {
    int u = edge.u;
    int v = edge.v;

    // Only draw the edge if both nodes are within the DisplayScreen
    bool uVisible = CheckCollisionPointRec(Nodes[u].Pos, DisplayScreen);
    bool vVisible = CheckCollisionPointRec(Nodes[v].Pos, DisplayScreen);
    if (!uVisible || !vVisible) return;  // Skip if either node is outside

    Vector2 start = Nodes[u].Pos;
    Vector2 end = Nodes[v].Pos;

    // Adjust start and end points to stop at the edge of the nodes
    Vector2 direction = {end.x - start.x, end.y - start.y};
    float dist = magnitude(direction);
    if (dist > 0) {
        direction = {direction.x / dist, direction.y / dist};
        start = {start.x + direction.x * nodeRadius, start.y + direction.y * nodeRadius};
        end = {end.x - direction.x * nodeRadius, end.y - direction.y * nodeRadius};
    }

    if (isDirected) {
        // Draw an arrow from u to v
        if (dist > 0) {
            DrawLineEx(start, end, special ? 2.0f : 1.0f, special ? RED : BLACK);
            // Draw arrowhead
            Vector2 arrow1 = {end.x - direction.x * 10 - direction.y * 5, end.y - direction.y * 10 + direction.x * 5};
            Vector2 arrow2 = {end.x - direction.x * 10 + direction.y * 5, end.y - direction.y * 10 - direction.x * 5};
            DrawLineEx(end, arrow1, special ? 2.0f : 1.0f, special ? RED : BLACK);
            DrawLineEx(end, arrow2, special ? 2.0f : 1.0f, special ? RED : BLACK);
        }
    } else {
        // Draw a simple line for undirected graph
        DrawLineEx(start, end, special ? 2.0f : 1.0f, special ? RED : BLACK);
    }

    // Draw edge weight if the graph is weighted
    if (isWeighted) {
        Vector2 midPoint = {
            (start.x + end.x) / 2.0f,
            (start.y + end.y) / 2.0f
        };
        // Since both nodes are within DisplayScreen, the midpoint will also be within bounds
        const char* weightText = TextFormat("%d", edge.w);
        float weightFontSize = nodeRadius * 1.2f;
        Vector2 textSize = MeasureTextEx(customFont, weightText, weightFontSize, 1.0f);
        DrawTextEx(customFont, weightText,
                   {midPoint.x - textSize.x / 2, midPoint.y - textSize.y / 2},
                   weightFontSize * (special ? 1.5f : 1.0f), 1.0f, special ? Color{172, 23, 84, 255} : Color{165, 91, 75, 255});
    }
}

void graph::DrawNode(int u) {
     // Highlight the selected node
    Color nodeColor = (u == selectedNode) ? GREEN : BLUE;
    DrawCircleV(Nodes[u].Pos, nodeRadius, nodeColor);

    // Draw the node value using the custom font
    const char* nodeText = TextFormat("%d", Nodes[u].val);
    float nodeFontSize = nodeRadius * 1.2f;
    Vector2 textSize = MeasureTextEx(customFont, nodeText, nodeFontSize, 1.0f);
    DrawTextEx(customFont, nodeText,
                {Nodes[u].Pos.x - textSize.x / 2, Nodes[u].Pos.y - textSize.y / 2},
                nodeFontSize, 1.0f, WHITE);
}

void graph::Draw() {
    // Draw edges and their weights
    for (const auto& edge : Edges) {
        DrawEdge(edge);
    }

    // Draw nodes and their values
    for (int u = 1; u <= numNode; ++u) {
        DrawNode(u);
    }
}

void graph::DrawMST() {
    vector<int> MST_edges = getMST(this);
    for (int i = 0; i < Edges.size(); ++i) {
        bool special = 0;
        for (int id : MST_edges) {
            if (i == id) {
                special = 1;
                break;
            }
        }
        DrawEdge(Edges[i], special);
    }

    for (int u = 1; u <= numNode; ++u) {
        DrawNode(u);
    }
}

std::random_device rd;
std::mt19937 gen(rd());

graph* GenerateRandomGraph(int numNodes, int numEdges, bool isDirected, bool isWeighted) {
    if (numNodes < 0) return nullptr;
    if (numEdges < 0) return nullptr;
    // Calculate maximum possible edges
    int maxEdges = isDirected ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
    // Minimum edges is now 0 since connectivity is not required
    int minEdges = 0;

    if (numEdges < minEdges) {
        printf("Number of edges cannot be negative. Setting numEdges to a random value.\n");
        numEdges = gen() % numNodes;
    }
    if (numEdges > maxEdges) {
        printf("Number of edges (%d) exceeds maximum possible edges (%d). Setting numEdges to %d.\n", numEdges, maxEdges, maxEdges);
        numEdges = maxEdges;
    }

    graph* myGraph = new graph(numNodes, isDirected, isWeighted);

    // Initialize nodes with random positions within the DisplayScreen
    std::uniform_real_distribution<float> disX(myGraph->DisplayScreen.x + myGraph->nodeRadius, 
                                               myGraph->DisplayScreen.x + myGraph->DisplayScreen.width - myGraph->nodeRadius);
    std::uniform_real_distribution<float> disY(myGraph->DisplayScreen.y + myGraph->nodeRadius, 
                                               myGraph->DisplayScreen.y + myGraph->DisplayScreen.height - myGraph->nodeRadius);
    for (int i = 1; i <= numNodes; ++i) {
        myGraph->Nodes[i].val = i;
        myGraph->Nodes[i].Pos = {disX(gen), disY(gen)};
    }

    // Add exactly numEdges edges
    if (numEdges > 0) {
        // Create a list of all possible edges (excluding self-loops)
        std::vector<std::pair<int, int>> possibleEdges;
        for (int u = 1; u <= numNodes; ++u) {
            for (int v = (isDirected ? 1 : u + 1); v <= numNodes; ++v) {
                if (u == v) continue;  // No self-loops
                possibleEdges.push_back({u, v});
            }
        }

        // Shuffle the list of possible edges
        std::shuffle(possibleEdges.begin(), possibleEdges.end(), gen);

        // Add edges until we reach the desired number
        int edgesAdded = 0;
        for (const auto& edge : possibleEdges) {
            if (edgesAdded >= numEdges) break;
            int u = edge.first;
            int v = edge.second;

            // Check if the edge already exists (to avoid duplicates)
            bool edgeExists = false;
            for (const int &EdgeID : myGraph->Adjacent_list[u]) {
                int otherNode = myGraph->Edges[EdgeID].other(u);
                if (otherNode == v) {
                    edgeExists = true;
                    break;
                }
            }
            if (edgeExists) continue;

            // Add the edge
            int weight = isWeighted ? (1 + (gen() % 10)) : 1;
            myGraph->AddEdge(u, v, weight);
            edgesAdded++;
        }
    }

    return myGraph;
}

void initEadesFactor(graph* G) {
    if (!G) return;
    G->iteration = 0;
    G->currentVeclocity = G->Veclocity;
    G->convergent = false;
}

void RunGraphVisualization(graph* G) {
    if (!G) return;
    initEadesFactor(G);
    G->HandleMouseInteraction();
    G->BalanceGraph();
    G->Draw();

    // Display info
    DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {900, 10}, 20, 1.0f, DARKGRAY);
    DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {900, 40}, 20, 1.0f, DARKGRAY);
}

void RunGraphVisualization_MST(graph* G) {
    if (!G) return;
    initEadesFactor(G);
    G->HandleMouseInteraction();
    G->BalanceGraph();
    G->DrawMST();

    // Display info
    DrawTextEx(customFont, ("Nodes: " + std::to_string(G->numNode)).c_str(), {900, 10}, 20, 1.0f, DARKGRAY);
    DrawTextEx(customFont, ("Edges: " + std::to_string(G->numEdge)).c_str(), {900, 40}, 20, 1.0f, DARKGRAY);
}

int* par = NULL;
int root(int x, int* par) {
    if (par[x] < 0) return x;
    return par[x] = root(par[x], par); 
}

bool unite(int u, int v, int* par) {
    u = root(u, par);
    v = root(v, par);
    if (u == v) return 0;
    if (-par[u] < -par[v]) swap(u, v);
    par[u] += par[v];
    par[v] = u;
    return 1;
}

std::vector<int> getMST(graph *G) {
    if (!G) return vector<int>();

    par = new int [G->numNode + 1];
    for (int i = 1; i <= G->numNode; ++i) par[i] = -1;

    vector<int> MST_Edges;
    vector<int> edgesList;
    for (int i = 0; i < G -> Edges.size(); ++i) edgesList.push_back(i);
    sort(edgesList.begin(), edgesList.end(), [&](const int &id1, const int &id2){
        return G -> Edges[id1].w < G -> Edges[id2].w;
    });

    for (const int &id : edgesList) {
        int u = G -> Edges[id].u;
        int v = G -> Edges[id].v;
        if (unite(u, v, par))
            MST_Edges.push_back(id);
    }
    delete [] par;
    return MST_Edges;
}   