#include "graph.h"
#include <random>

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

graph::graph(int _numNode) {
    numNode = _numNode;
    numEdge = 0;
    Nodes.resize(numNode + 1, node());
    Adjacent_list.resize(numNode + 1);

    // Dynamically scale IdealEdgeLength based on global screen size and number of nodes
    IdealEdgeLength = sqrtf(screenWidth * screenHeight / (float)numNode) * 0.75f;
    // Scale Crep based on number of nodes
    Crep = numNode * numNode * 10.0f;
}

int graph::edge::other(int x) {
    return u ^ v ^ x;
}

void graph::AddEdge(int u, int v, int w) {
    Adjacent_list[u].push_back(Edges.size());
    Adjacent_list[v].push_back(Edges.size());
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
    Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};
    Vector2 direction = {center.x - Nodes[u].Pos.x, center.y - Nodes[u].Pos.y};
    float dist = magnitude(direction);
    if (dist == 0.0f) return {0.0f, 0.0f};
    return {Ccenter * direction.x, Ccenter * direction.y};
}

void graph::BalanceGraph() {
    // if (convergent || iteration >= numIteration) return;

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
        Clamp(Nodes[u].Pos.x, nodeRadius, screenWidth - nodeRadius);
        Clamp(Nodes[u].Pos.y, nodeRadius, screenHeight - nodeRadius);
    }

    // Apply cooling factor
    currentVeclocity *= CoolingFactor;

    // Increase iteration
    ++iteration;

    // Check for convergence
    if (totalForce < epsilon) {
        convergent = true;
    }
}

void graph::HandleMouseInteraction() {
    Vector2 mousePos = GetMousePosition();

    // Check for mouse button down to select a node
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        selectedNode = -1;  // Reset selection
        for (int u = 1; u <= numNode; ++u) {
            float dist = EulerDist(u, mousePos);
            if (dist <= nodeRadius) {
                selectedNode = u;
                break;
            }
        }
    }

    // Update the position of the selected node while the mouse button is held
    if (selectedNode != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Nodes[selectedNode].Pos = mousePos;
        // Keep the node within bounds
        Clamp(Nodes[selectedNode].Pos.x, nodeRadius, screenWidth - nodeRadius);
        Clamp(Nodes[selectedNode].Pos.y, nodeRadius, screenHeight - nodeRadius);
        // Reset forces to prevent jittering while dragging
        Nodes[selectedNode].Forces = {0.0f, 0.0f};
        initEadesFactor(this);
    }

    // Release the node when the mouse button is released
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        selectedNode = -1;
    }
}

void graph::Draw() {
    // Draw edges and their weights
    for (const auto& edge : Edges) {
        int u = edge.u;
        int v = edge.v;
        DrawLineV(Nodes[u].Pos, Nodes[v].Pos, BLACK);

        // Calculate midpoint of the edge for placing the weight
        Vector2 midPoint = {
            (Nodes[u].Pos.x + Nodes[v].Pos.x) / 2.0f,
            (Nodes[u].Pos.y + Nodes[v].Pos.y) / 2.0f
        };

        const char* weightText = TextFormat("%d", edge.w);
        // Scale the font size for edge weight text based on nodeRadius
        float weightFontSize = nodeRadius * 1.2f;  // Adjust the scaling factor as needed
        Vector2 textSize = MeasureTextEx(customFont, weightText, weightFontSize, 1.0f);
        DrawTextEx(customFont, weightText,
                   {midPoint.x - textSize.x / 2, midPoint.y - textSize.y / 2},
                   weightFontSize, 1.0f, RED);
    }

    // Draw nodes and their values
    for (int u = 1; u <= numNode; ++u) {
        // Highlight the selected node
        Color nodeColor = (u == selectedNode) ? GREEN : BLUE;
        DrawCircleV(Nodes[u].Pos, nodeRadius, nodeColor);

        // Draw the node value using the custom font
        const char* nodeText = TextFormat("%d", Nodes[u].val);
        // Scale the font size for node text based on nodeRadius
        float nodeFontSize = nodeRadius * 1.2f;  // Adjust the scaling factor as needed
        Vector2 textSize = MeasureTextEx(customFont, nodeText, nodeFontSize, 1.0f);
        DrawTextEx(customFont, nodeText,
                   {Nodes[u].Pos.x - textSize.x / 2, Nodes[u].Pos.y - textSize.y / 2},
                   nodeFontSize, 1.0f, WHITE);
    }
}

std::random_device rd;
std::mt19937 gen(rd());

graph* GenerateRandomGraph() {
    int numNode = 5 + (gen() % 10);  
    graph* myGraph = new graph(numNode);

    // Initialize nodes with random positions
    std::uniform_real_distribution<float> disX(myGraph->nodeRadius, screenWidth - myGraph->nodeRadius);
    std::uniform_real_distribution<float> disY(myGraph->nodeRadius, screenHeight - myGraph->nodeRadius);
    for (int i = 1; i <= numNode; ++i) {
        myGraph->Nodes[i].val = i;
        myGraph->Nodes[i].Pos = {disX(gen), disY(gen)};
    }

    // Create a cycle to ensure connectivity
    for (int i = 1; i <= numNode; ++i) {
        int u = i;
        int v = (i % numNode) + 1;  // Connects 1-2, 2-3, ..., numNode-1
        // Assign a random weight between 1 and 10
        std::uniform_int_distribution<int> disWeight(1, 10);
        myGraph->AddEdge(u, v, disWeight(gen));
    }

    // Add random edges with probability 0.1
    std::uniform_real_distribution<float> disProb(0.0f, 1.0f);
    std::uniform_int_distribution<int> disWeight(1, 10);
    for (int u = 1; u <= numNode; ++u) {
        for (int v = u + 1; v <= numNode; ++v) {
            if (v == (u % numNode) + 1) continue;  // Skip cycle edges
            if (disProb(gen) < 0.1f) {
                myGraph->AddEdge(u, v, disWeight(gen));
            }
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