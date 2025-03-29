#include "Graph_Menu.h"

void Graph_Menu::ChooseGraphType(Graph* &G) {
    if (CheckCollisionPointRec(mouse, undirectedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isDirected = false;
        if (G != nullptr) {
            G->isDirected = isDirected;
        }
    }
    if (CheckCollisionPointRec(mouse, directedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption == MST_KRUSKAL) return; /// MST must be undirected
        isDirected = true;
        if (G != nullptr) {
            G->isDirected = isDirected;
        }
    }
    if (CheckCollisionPointRec(mouse, unweightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption == MST_KRUSKAL) return; /// MST must be weighted
        isWeighted = false;
        if (G != nullptr) {
            G->isWeighted = isWeighted;
        }
    }
    if (CheckCollisionPointRec(mouse, weightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isWeighted = true;
        if (G != nullptr) {
            G->isWeighted = isWeighted;
        }
    }
}

void Graph_Menu::ChooseAlgorithms(Graph* &G) {
    if (CheckCollisionPointRec(mouse, createBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = CREATE;
    }
    if (CheckCollisionPointRec(mouse, mstBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MST_KRUSKAL;
        isDirected = false;
        isWeighted = true;
        if (G != nullptr) {
            G->isDirected = isDirected;
            G->isWeighted = isWeighted;
        }
    }
    if (CheckCollisionPointRec(mouse, dijkstraBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = DIJKSTRA;
    }
}

void Graph_Menu::Handle_Input() {
    // Handle textbox activation
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Nodes textbox
        if (CheckCollisionPointRec(mouse, nodesLabel.rect) 
            || ((nodesInput.size() > 0 || nodesBoxActive) && CheckCollisionPointRec(mouse, nodesBox.rect))) {
            nodesBoxActive = true;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        } else {
            nodesBoxActive = false;
        }

        // Edges textbox
        if (CheckCollisionPointRec(mouse, edgesLabel.rect) 
            || ((edgesInput.size() > 0 || edgesBoxActive) && CheckCollisionPointRec(mouse, edgesBox.rect))) {
            edgesBoxActive = true;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        } else {
            edgesBoxActive = false;
        }

        // Source textbox (only active when Dijkstra is selected)
        if (selectedOption == DIJKSTRA && (CheckCollisionPointRec(mouse, sourceLabel.rect)
            || ((sourceInput.size() > 0 || sourceBoxActive) && CheckCollisionPointRec(mouse, sourceBox.rect)))) {
            sourceBoxActive = true;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        } else {
            sourceBoxActive = false;
        }

        // Destination textbox (only active when Dijkstra is selected)
        if (selectedOption == DIJKSTRA && (CheckCollisionPointRec(mouse, destLabel.rect)
            || ((destInput.size() || destBoxActive) && CheckCollisionPointRec(mouse, destBox.rect))))  {
            destBoxActive = true;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        } else {
            destBoxActive = false;
        }

        if (CheckCollisionPointRec(mouse, fileBtn.rect)) {
            fileSelected = true;
        } else {
            fileSelected = false;
        }

        if (CheckCollisionPointRec(mouse, confirmBtn.rect)) {
            confirmPressed = true;
        } else {
            confirmPressed = false;
        }
    }

    float deltaTime = GetFrameTime();
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= cursorBlinkInterval) {
        showCursor = !showCursor;
        cursorBlinkTimer = 0.0f;
    }

    // Handle text input
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9') {
        if (nodesBoxActive && nodesInput.size() <= 4) {
            nodesInput += (char)key;
        }
        if (edgesBoxActive && edgesInput.size() <= 4) {
            edgesInput += (char)key;
        }
        if (selectedOption == DIJKSTRA) {
            if (sourceBoxActive && sourceInput.size() <= 4) {
                sourceInput += (char)key;
            }
            if (destBoxActive && destInput.size() <= 4) {
                destInput += (char)key;
            }
        }
    }
    
    // Handle holding BACKSPACE
    if (IsKeyDown(KEY_BACKSPACE)) {
        if (!backspaceHeld) {
            backspaceHeld = true;
            backspaceTimer = 0.0f;
            initialBackspaceDelayPassed = false;

            // Delete one character immediately on first press
            if (nodesBoxActive && !nodesInput.empty()) {
                nodesInput.pop_back();
            }
            if (edgesBoxActive && !edgesInput.empty()) {
                edgesInput.pop_back();
            }
            if (sourceBoxActive && !sourceInput.empty()) {
                sourceInput.pop_back();
            }
            if (destBoxActive && !destInput.empty()) {
                destInput.pop_back();
            }
        }

        backspaceTimer += deltaTime;
        if (!initialBackspaceDelayPassed && backspaceTimer >= initialBackspaceDelay) {
            initialBackspaceDelayPassed = true;
            backspaceTimer = 0.0f;
        }

        if (initialBackspaceDelayPassed && backspaceTimer >= repeatBackspaceDelay) {
            if (nodesBoxActive && !nodesInput.empty()) {
                nodesInput.pop_back();
            }
            if (edgesBoxActive && !edgesInput.empty()) {
                edgesInput.pop_back();
            }
            if (sourceBoxActive && !sourceInput.empty()) {
                sourceInput.pop_back();
            }
            if (destBoxActive && !destInput.empty()) {
                destInput.pop_back();
            }
            backspaceTimer = 0.0f;
        }
    } else {
        backspaceHeld = false;
        initialBackspaceDelayPassed = false;
        backspaceTimer = 0.0f;
    }

    // Update the text of the textboxes
    nodesBox.text = nodesInput.c_str();
    edgesBox.text = edgesInput.c_str();

    sourceBox.text = sourceInput.c_str();
    destBox.text = destInput.c_str();
}

void Graph_Menu::GetInput(int &numNodes, int &numEdges, int &source, int &dest) {
    try {
        numNodes = std::stoi(nodesInput);
    } catch (...) {
        std::random_device rd;
        std::mt19937 gen(rd());
        numNodes = 5 + (gen() % 10);    
    }

    try {
        numEdges = std::stoi(edgesInput);
    } catch (...) {
        std::random_device rd;
        std::mt19937 gen(rd());
        int maxEdges = isDirected ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
        numEdges = gen() % maxEdges;
    }

    // For Dijkstra, get source and destination
    if (selectedOption == DIJKSTRA) {
        try {
            source = std::stoi(sourceInput);
        } catch (...) {
            std::random_device rd;
            std::mt19937 gen(rd());
            source = 1 + (gen() % numNodes);  // Random vertex between 1 and numNodes
        }

        try {
            dest = std::stoi(destInput);
        } catch (...) {
            std::random_device rd;
            std::mt19937 gen(rd());
            dest = 1 + (gen() % numNodes);  // Random vertex between 1 and numNodes
        }
    } else {
        source = -1;
        dest = -1;
    }
}

void Graph_Menu::ClearInputBoxes() {
    confirmPressed = false;
    // nodesInput = "";
    // edgesInput = "";
    // sourceInput = "";
    // destInput = "";
    fileSelected = false;
    nodesBoxActive = false;
    edgesBoxActive = false;
    sourceBoxActive = false;
    destBoxActive = false;

    // Reset textbox texts
    // nodesBox.text = "";
    // edgesBox.text = "";
    // sourceBox.text = "";
    // destBox.text = "";
}

void Graph_Menu::MakeGraph(Graph* &Graphs) {
    int numNodes = 0, numEdges = 0, source = -1, dest = -1;
    GetInput(numNodes, numEdges, source, dest);
    ClearInputBoxes();

    if (Graphs) delete Graphs;
    if (selectedOption == CREATE) {
        Graphs = GenerateRandomGraph(numNodes, numEdges, isDirected, isWeighted);
    }
    if (selectedOption == MST_KRUSKAL) {
        Graphs = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
    }
    if (selectedOption == DIJKSTRA) {
        Graphs = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
        Graphs -> DIJKSTRA_parameters = {source, dest};
        // Here you can use source and dest for Dijkstra's algorithm
        // For now, we'll just create the graph
        // You can add Dijkstra's algorithm implementation later
    }
}

void Graph_Menu::Handle(Graph* &G) {
    ChooseGraphType(G);
    ChooseAlgorithms(G);
    Handle_Input();


    if (fileSelected) {
        const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
        Handle_InputFile(filePath, G);
        fileSelected = false;
    } 
    else if (confirmPressed) {
        MakeGraph(G);
    }
}

void Graph_Menu::Draw() {
    // Upper box background
    upperBoxBackground.Draw_TextBox();

    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};
    // Upper box: Toggle buttons
    undirectedBtn.rectColor = !isDirected ? labelColor[1] : labelColor[0];
    undirectedBtn.Draw_TextBox(!isDirected);

    directedBtn.rectColor = isDirected ? labelColor[1] : labelColor[0];
    directedBtn.Draw_TextBox(isDirected);

    unweightedBtn.rectColor = !isWeighted ? labelColor[1] : labelColor[0];
    unweightedBtn.Draw_TextBox(!isWeighted);

    weightedBtn.rectColor = isWeighted ? labelColor[1] : labelColor[0];
    weightedBtn.Draw_TextBox(isWeighted);

    // Lower box background
    lowerAlgoBoxBackground.Draw_TextBox();

    // Lower box: Left column
    createBtn.rectColor = (selectedOption == CREATE) ? labelColor[1] : labelColor[0];
    createBtn.Draw_TextBox(selectedOption == CREATE);

    mstBtn.rectColor = (selectedOption == MST_KRUSKAL) ? labelColor[1] : labelColor[0];
    mstBtn.Draw_TextBox(selectedOption == MST_KRUSKAL);

    dijkstraBtn.rectColor = (selectedOption == DIJKSTRA) ? labelColor[1] : labelColor[0];
    dijkstraBtn.Draw_TextBox(selectedOption == DIJKSTRA);
    
    // Draw input boxes
    if (selectedOption != NONE) {
        lowerParameterBoxBackground.Draw_TextBox();
        nodesLabel.rectColor = labelColor[nodesBoxActive];
        nodesLabel.Draw_TextBox();
        if (nodesInput.size() > 0 || nodesBoxActive) nodesBox.Draw_TextBox();
        if (nodesBoxActive && showCursor) nodesBox.Draw_BlinkingLine();

        edgesLabel.rectColor = labelColor[edgesBoxActive];
        edgesLabel.Draw_TextBox();
        if (edgesInput.size() > 0 || edgesBoxActive) edgesBox.Draw_TextBox();
        if (edgesBoxActive && showCursor) edgesBox.Draw_BlinkingLine();
    }
    // Draw Source and Destination textboxes only if Dijkstra is selected
    if (selectedOption == DIJKSTRA) {
        sourceLabel.rectColor = labelColor[sourceBoxActive];
        sourceLabel.Draw_TextBox();
        if (sourceInput.size() > 0 || sourceBoxActive) sourceBox.Draw_TextBox();
        if (sourceBoxActive && showCursor) sourceBox.Draw_BlinkingLine();

        destLabel.rectColor = labelColor[destBoxActive];
        destLabel.Draw_TextBox();
        if (destInput.size() > 0 || destBoxActive) destBox.Draw_TextBox();
        if (destBoxActive && showCursor) destBox.Draw_BlinkingLine();
    }

    // Draw fileBtn and confirmBtn in the left column
    fileBtn.rectColor = fileSelected ? LIGHTGRAY : BLACK;
    fileBtn.Draw_TextBox();
 
    confirmBtn.Draw_TextBox();
}