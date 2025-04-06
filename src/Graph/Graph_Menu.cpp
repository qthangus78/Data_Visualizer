#include "Graph_Menu.h"

void GraphMenu::ChooseGraphType(GraphVisualizer &GV) {
    if (CheckCollisionPointRec(mouse, undirectedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        directionType = GraphDirectionType::UNDIRECTED;
        if (GV.graph) GV.graph->isDirected = false;
    }
    if (CheckCollisionPointRec(mouse, directedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption != MenuOption::MST_KRUSKAL) { // MST must be undirected
            directionType = GraphDirectionType::DIRECTED;
            if (GV.graph) GV.graph->isDirected = true;
        }
    }
    if (CheckCollisionPointRec(mouse, unweightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption == MenuOption::CREATE) { // MST and DIJKSTRA must be weighted
            weightType = GraphWeightType::UNWEIGHTED;
            if (GV.graph) GV.graph->isWeighted = false;
        }
    }
    if (CheckCollisionPointRec(mouse, weightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        weightType = GraphWeightType::WEIGHTED;
        if (GV.graph) GV.graph->isWeighted = true;
    }
}

void GraphMenu::ChooseAlgorithms(GraphVisualizer &GV) {
    // Choose menu option
    if (CheckCollisionPointRec(mouse, createBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::CREATE;
        showButtons = true;
    }
    if (CheckCollisionPointRec(mouse, mstBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (!GV.graph) {
            showButtons = false;
            std::string errorMessage = "Please create a graph first!"; // Show this error message in the AnnouncementBox
            // DrawText(errorMessage.c_str(), 10, GetScreenHeight() - 30, 20, RED);
            return;
        }
        selectedOption = MenuOption::MST_KRUSKAL;
        directionType = GraphDirectionType::UNDIRECTED;
        weightType = GraphWeightType::WEIGHTED;
        if (GV.graph) {
            GV.graph->isDirected = false;
            GV.graph->isWeighted = true;
        }
        showButtons = true;
    }
    if (CheckCollisionPointRec(mouse, dijkstraBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::DIJKSTRA;
        weightType = GraphWeightType::WEIGHTED;
        showButtons = true;
    }

    // Choose edge options
    if (CheckCollisionPointRec(mouse, addEdgeBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedEdgeOp = EdgeOperation::ADD_EDGE;
        showEdgeInputBox = true;
    }
    if (CheckCollisionPointRec(mouse, deleteEdgeBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedEdgeOp = EdgeOperation::DELETE_EDGE;   
        showEdgeInputBox = true;
    }
}

void GraphMenu::HandleInput() {
    mouse = GetMousePosition();
    float deltaTime = GetFrameTime();

    // Handle textbox activation
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, nodesField.label.rect) ||
            ((nodesField.input.size() > 0 || activeField == ActiveField::NODES) && CheckCollisionPointRec(mouse, nodesField.box.rect))) {
            activeField = ActiveField::NODES;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else if (CheckCollisionPointRec(mouse, edgesField.label.rect) ||
                 ((edgesField.input.size() > 0 || activeField == ActiveField::EDGES) && CheckCollisionPointRec(mouse, edgesField.box.rect))) {
            activeField = ActiveField::EDGES;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else if (selectedOption == MenuOption::DIJKSTRA &&
                 (CheckCollisionPointRec(mouse, sourceField.label.rect) ||
                  ((sourceField.input.size() > 0 || activeField == ActiveField::SOURCE) && CheckCollisionPointRec(mouse, sourceField.box.rect)))) {
            activeField = ActiveField::SOURCE;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else {
            activeField = ActiveField::NONE;
        }

        if (CheckCollisionPointRec(mouse, fromField.label.rect) ||
            ((fromField.input.size() > 0 || activeEdgeField == ActiveEdgeField::FROM) && 
             CheckCollisionPointRec(mouse, fromField.box.rect))) {
            activeEdgeField = ActiveEdgeField::FROM;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else if (CheckCollisionPointRec(mouse, toField.label.rect) ||
                ((toField.input.size() > 0 || activeEdgeField == ActiveEdgeField::TO) && 
                 CheckCollisionPointRec(mouse, toField.box.rect))) {
            activeEdgeField = ActiveEdgeField::TO;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else if (weightType == GraphWeightType::WEIGHTED && 
                (CheckCollisionPointRec(mouse, weightField.label.rect) ||
                 ((weightField.input.size() > 0 || activeEdgeField == ActiveEdgeField::WEIGHT) && 
                  CheckCollisionPointRec(mouse, weightField.box.rect)))) {
            activeEdgeField = ActiveEdgeField::WEIGHT;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else {
            activeEdgeField = ActiveEdgeField::NONE;
        }

        fileSelected = CheckCollisionPointRec(mouse, fileBtn.rect);
        confirmPressed = CheckCollisionPointRec(mouse, confirmBtn.rect);
    }

    // Update cursor blinking
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= cursorBlinkInterval) {
        showCursor = !showCursor;
        cursorBlinkTimer = 0.0f;
    }

    // Handle numeric input
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9') {
        if (activeField == ActiveField::NODES && nodesField.input.size() <= 4) {
            nodesField.input += static_cast<char>(key);
        }
        else if (activeField == ActiveField::EDGES && edgesField.input.size() <= 4) {
            edgesField.input += static_cast<char>(key);
        }
        else if (activeField == ActiveField::SOURCE && sourceField.input.size() <= 4) {
            sourceField.input += static_cast<char>(key);
        }
        else if (activeEdgeField == ActiveEdgeField::FROM && fromField.input.size() <= 4) {
            fromField.input += (char)key;
        }
        else if (activeEdgeField == ActiveEdgeField::TO && toField.input.size() <= 4) {
            toField.input += (char)key;
        }
        else if (activeEdgeField == ActiveEdgeField::WEIGHT && weightType == GraphWeightType::WEIGHTED && 
                 weightField.input.size() <= 4) {
            weightField.input += (char)key;
        }
    }

    // Handle backspace with hold-to-delete
    if (IsKeyDown(KEY_BACKSPACE)) {
        if (!backspaceHeld) {
            backspaceHeld = true;
            backspaceTimer = 0.0f;
            initialBackspaceDelayPassed = false;
            if (activeField == ActiveField::NODES && !nodesField.input.empty()) nodesField.input.pop_back();
            else if (activeField == ActiveField::EDGES && !edgesField.input.empty()) edgesField.input.pop_back();
            else if (activeField == ActiveField::SOURCE && !sourceField.input.empty()) sourceField.input.pop_back();
            
            if (activeEdgeField == ActiveEdgeField::FROM && !fromField.input.empty()) fromField.input.pop_back();
            else if (activeEdgeField == ActiveEdgeField::TO && !toField.input.empty()) toField.input.pop_back();
            else if (activeEdgeField == ActiveEdgeField::WEIGHT && !weightField.input.empty()) weightField.input.pop_back();
        }
        backspaceTimer += deltaTime;
        if (!initialBackspaceDelayPassed && backspaceTimer >= initialBackspaceDelay) {
            initialBackspaceDelayPassed = true;
            backspaceTimer = 0.0f;
        }
        if (initialBackspaceDelayPassed && backspaceTimer >= repeatBackspaceDelay) {
            if (activeField == ActiveField::NODES && !nodesField.input.empty()) nodesField.input.pop_back();
            else if (activeField == ActiveField::EDGES && !edgesField.input.empty()) edgesField.input.pop_back();
            else if (activeField == ActiveField::SOURCE && !sourceField.input.empty()) sourceField.input.pop_back();
            
            if (activeEdgeField == ActiveEdgeField::FROM && !fromField.input.empty()) fromField.input.pop_back();
            else if (activeEdgeField == ActiveEdgeField::TO && !toField.input.empty()) toField.input.pop_back();
            else if (activeEdgeField == ActiveEdgeField::WEIGHT && !weightField.input.empty()) weightField.input.pop_back();
            backspaceTimer = 0.0f;
        }
    }
    else {
        backspaceHeld = false;
        initialBackspaceDelayPassed = false;
        backspaceTimer = 0.0f;
    }

    // Update textbox texts
    nodesField.box.text = nodesField.input.c_str();
    edgesField.box.text = edgesField.input.c_str();
    sourceField.box.text = sourceField.input.c_str();
    fromField.box.text = fromField.input.c_str();
    toField.box.text = toField.input.c_str();
    weightField.box.text = weightField.input.c_str();
}

void GraphMenu::GetInput(int& numNodes, int& numEdges, int& source) {
    try {
        numNodes = std::stoi(nodesField.input);
    }
    catch (...) {
        std::random_device rd;
        std::mt19937 gen(rd());
        numNodes = 5 + (gen() % 10);
    }

    try {
        numEdges = std::stoi(edgesField.input);
    }
    catch (...) {
        std::random_device rd;
        std::mt19937 gen(rd());
        int maxEdges = (directionType == GraphDirectionType::DIRECTED) ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
        numEdges = gen() % maxEdges;
    }

    if (selectedOption == MenuOption::DIJKSTRA) {
        try {
            source = std::stoi(sourceField.input);
        }
        catch (...) {
            std::random_device rd;
            std::mt19937 gen(rd());
            source = 1 + (gen() % numNodes);
        }
    }
    else {
        source = -1;
    }
}

void GraphMenu::GetEdgeInput(int& from, int& to, int& weight) {
    try {
        from = std::stoi(fromField.input);
    }
    catch (...) {
        from = -1;
    }

    try {
        to = std::stoi(toField.input);
    }
    catch (...) {
        to = -1;
    }

    try {
        if (weightType == GraphWeightType::WEIGHTED) {
            weight = std::stoi(weightField.input);
        } else {
            weight = 1; // Default weight for unweighted graphs
        }
    }
    catch (...) {
        weight = weightType == GraphWeightType::WEIGHTED ? -1 : 1;
    }
}

void GraphMenu::ClearInputBoxes() {
    confirmPressed = false;
    fileSelected = false;
    activeField = ActiveField::NONE;
}

void GraphMenu::ClearEdgeInputBoxes() {
    fromField.input.clear();
    toField.input.clear();
    weightField.input.clear();
    activeField = ActiveField::NONE;
}

void GraphMenu::HandleEdgeOperations(GraphVisualizer &GV) {
    int from, to, weight;
    GetEdgeInput(from, to, weight);

    bool invalidInput = false;
    std::string errorMessage = "";

    // Validate input
    if (from <= 0 || to <= 0 || fromField.input.empty() || toField.input.empty()) {
        invalidInput = true;
        errorMessage = "Invalid edge input: Missing or invalid vertex numbers";
    }
    else if (weightType == GraphWeightType::WEIGHTED && weightField.input.empty()) {
        invalidInput = true;
        errorMessage = "Invalid edge input: Missing weight for weighted graph";
    }
    else if (GV.graph && (from > GV.graph->numNode || to > GV.graph->numNode)) {
        invalidInput = true;
        errorMessage = "Invalid edge input: Vertex numbers exceed graph size";
    }

    if (invalidInput) {
        const int msgDuration = 1200; // Duration in frames 
        static int msgTimer = 0;
        if (msgTimer == 0) {
            msgTimer = msgDuration;
        }
        if (msgTimer > 0) {
            DrawText(errorMessage.c_str(), 10, GetScreenHeight() - 30, 20, RED);
            msgTimer--;
        }
        return;
    }

    if (GV.graph) {
        if (selectedEdgeOp == EdgeOperation::ADD_EDGE) {
            // Check if weight is valid for weighted graphs
            if (!GV.graph->isWeighted || (GV.graph->isWeighted && weight >= 0)) {
                GV.graph->AddEdge(from, to, weight);
                ClearEdgeInputBoxes();
            }
            edgesField.input = std::to_string(GV.graph->numEdge);
        }
        else if (selectedEdgeOp == EdgeOperation::DELETE_EDGE) {
            // Find and remove the edge
            bool edgeFound = false;
            for (int i = 0; i < GV.graph->Edges.size(); i++) {
                const auto& edge = GV.graph->Edges[i];
                if ((edge.u == from && edge.v == to) || (!GV.graph->isDirected && edge.u == to && edge.v == from)) {
                    // Remove edge from adjacency lists
                    for (int u = 1; u <= GV.graph->numNode; u++) {
                        auto& adj = GV.graph->Adjacent_list[u];
                        adj.erase(std::remove(adj.begin(), adj.end(), i), adj.end());
                        // Update remaining edge indices in adjacency list
                        for (auto& idx : adj) {
                            if (idx > i) idx--;
                        }
                    }
                    // Remove edge from edge list
                    GV.graph->Edges.erase(GV.graph->Edges.begin() + i);
                    GV.graph->numEdge--;
                    edgeFound = true;
                    ClearEdgeInputBoxes();
                    break;
                }
            }

            if (!edgeFound) {
                const int msgDuration = 1200; // Duration in frames 
                static int msgTimer = 0;
                if (msgTimer == 0) {
                    msgTimer = msgDuration;
                }
                if (msgTimer > 0) {
                    DrawText("Edge not found", 10, GetScreenHeight() - 30, 20, RED);
                    msgTimer--;
                }
            }
            edgesField.input = std::to_string(GV.graph->numEdge);
        }
    }
} 

void GraphMenu::HandleClearGraphBtn(GraphVisualizer &GV) {
    if (CheckCollisionPointRec(mouse, clearGraphBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showClearConfirm = true;
    }
    if (showClearConfirm && CheckCollisionPointRec(mouse, clearGraphConfirmBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (GV.graph) {
            delete GV.graph;
            GV.graph = nullptr;
        }
        showClearConfirm = false;
        selectedOption = MenuOption::NONE;
        selectedEdgeOp = EdgeOperation::NONE;
        showEdgeInputBox = false;
        showButtons = false;
        ClearInputBoxes();
        ClearEdgeInputBoxes();
    }
}

void GraphMenu::Handle(GraphVisualizer &GV) {
    ChooseGraphType(GV);
    ChooseAlgorithms(GV);
    HandleInput();

    if (fileSelected) {
        const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
        Handle_InputFile(filePath, GV.graph);
        GV.initEadesFactor();
        fileSelected = false;
    }
    else if (confirmPressed) {
        if (selectedOption == MenuOption::CREATE) {
            MakeGraph(GV.graph);
            GV.initEadesFactor();
        }
        else if (selectedOption == MenuOption::DIJKSTRA) {
            int numNode, numEdge, source;
            GetInput(numNode, numEdge, source);
            if (!GV.graph || GV.graph->numNode != numNode || GV.graph->numEdge != numEdge) {
                MakeGraph(GV.graph);
                GV.initEadesFactor();
            }
            else {
                GV.graph->dijkstraSource = source;
            }
            GV.initDijkstra();
        }
        else if (selectedOption == MenuOption::MST_KRUSKAL) {
            GV.initKruskal();
        }

        confirmPressed = false;  // Reset confirm button state
    }

    if (showEdgeInputBox && CheckCollisionPointRec(mouse, edgeConfirmBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        HandleEdgeOperations(GV);
    }

    HandleClearGraphBtn(GV);
}

void GraphMenu::MakeGraph(Graph* &G) {
    int numNodes = 0, numEdges = 0, source = -1;
    GetInput(numNodes, numEdges, source);
    ClearInputBoxes();

    if (G) delete G;
    bool isDirected = (directionType == GraphDirectionType::DIRECTED);
    bool isWeighted = (weightType == GraphWeightType::WEIGHTED);
    G = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
    G -> dijkstraSource = source;

    nodesField.input = std::to_string(G -> numNode);
    edgesField.input = std::to_string(G -> numEdge);
    sourceField.input = std::to_string(G -> dijkstraSource);
}

