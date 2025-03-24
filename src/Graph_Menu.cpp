#include "Graph_Menu.h"

void Graph_Menu::MoveMenuBoxes(float deltaX, float deltaY) {
    upperBoxBackground.rect.x += deltaX;
    undirectedBtn.rect.x += deltaX;
    directedBtn.rect.x += deltaX;
    unweightedBtn.rect.x += deltaX;
    weightedBtn.rect.x += deltaX;
    
    lowerBoxBackground.rect.x += deltaX;
    createBtn.rect.x += deltaX;
    mstBtn.rect.x += deltaX;
    dijkstraBtn.rect.x += deltaX;

    // Lower box: Right column (es for "Create" option)
    nodesLabel.rect.x += deltaX;  // Label for "Nodes"
    nodesBox.rect.x += deltaX;    // Input box for nodes
    edgesLabel.rect.x += deltaX;  // Label for "Edges"
    edgesBox.rect.x += deltaX;    // Input box for edges
    randomBtn.rect.x += deltaX;
    confirmBtn.rect.x += deltaX;



    upperBoxBackground.rect.y += deltaY;
    undirectedBtn.rect.y += deltaY;
    directedBtn.rect.y += deltaY;
    unweightedBtn.rect.y += deltaY;
    weightedBtn.rect.y += deltaY;
    
    lowerBoxBackground.rect.y += deltaY;
    createBtn.rect.y += deltaY;
    mstBtn.rect.y += deltaY;
    dijkstraBtn.rect.y += deltaY;

    // Lower box: Right column (es for "Create" option)
    nodesLabel.rect.y += deltaY;  // Label for "Nodes"
    nodesBox.rect.y += deltaY;    // Input box for nodes
    edgesLabel.rect.y += deltaY;  // Label for "Edges"
    edgesBox.rect.y += deltaY;    // Input box for edges
    randomBtn.rect.y += deltaY;
    confirmBtn.rect.y += deltaY;

}

void Graph_Menu::ChooseGraphType(graph* &G) {
    if (CheckCollisionPointRec(mouse, undirectedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isDirected = false;
        if (G != nullptr) {
            G -> isDirected = isDirected;
        }
    }
    if (CheckCollisionPointRec(mouse, directedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption == MST_KRUSKAL) return; /// MST must be undirected
        isDirected = true;
        if (G != nullptr) {
            G -> isDirected = isDirected;
        }
    }
    if (CheckCollisionPointRec(mouse, unweightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption == MST_KRUSKAL) return; /// MST must be weighted
        isWeighted = false;
        if (G != nullptr) {
            G -> isWeighted = isWeighted;
        }
    }
    if (CheckCollisionPointRec(mouse, weightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isWeighted = true;
        if (G != nullptr) {
            G -> isWeighted = isWeighted;
        }
    }
}

void Graph_Menu::ChooseAlgorithms(graph* &G) {
    if (CheckCollisionPointRec(mouse, createBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = CREATE;
    }
    if (CheckCollisionPointRec(mouse, mstBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MST_KRUSKAL;
        isDirected = false;
        isWeighted = true;
        if (G != nullptr) {
            G -> isDirected = isDirected;
            G -> isWeighted = isWeighted;
        }
    }
    if (CheckCollisionPointRec(mouse, dijkstraBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = DIJKSTRA;
    }
}

void Graph_Menu::Handle_Input() {
    // Handle textbox activation
    if (CheckCollisionPointRec(mouse, nodesBox.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        nodesBoxActive = true;
        edgesBoxActive = false;
        randomSelected = false;

        cursorBlinkTimer = 0.0f; // Reset cursor blink
        showCursor = true;
    }
    if (CheckCollisionPointRec(mouse, edgesBox.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        edgesBoxActive = true;
        nodesBoxActive = false;
        randomSelected = false;

        cursorBlinkTimer = 0.0f; // Reset cursor blink
        showCursor = true;
    }
    if (CheckCollisionPointRec(mouse, randomBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        randomSelected = true;
        nodesInput = "Random";
        edgesInput = "Random";
        nodesBoxActive = false;
        edgesBoxActive = false;
    }
    if (CheckCollisionPointRec(mouse, confirmBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        confirmPressed = true;
    }

    float deltaTime = GetFrameTime();
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= cursorBlinkInterval) {
        showCursor = !showCursor; // Toggle cursor visibility
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
    }
    
    /// Hanle holding BACKSPACE
    if (IsKeyDown(KEY_BACKSPACE)) {
        if (!backspaceHeld) {
            // First press of Backspace
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
        }

        // Update timer while holding Backspace
        backspaceTimer += deltaTime;

        // Check if initial delay has passed
        if (!initialBackspaceDelayPassed && backspaceTimer >= initialBackspaceDelay) {
            initialBackspaceDelayPassed = true;
            backspaceTimer = 0.0f; // Reset timer for repeat delay
        }

        // Continuous deletion after initial delay
        if (initialBackspaceDelayPassed && backspaceTimer >= repeatBackspaceDelay) {
            if (nodesBoxActive && !nodesInput.empty()) {
                nodesInput.pop_back();
            }
            if (edgesBoxActive && !edgesInput.empty()) {
                edgesInput.pop_back();
            }
            backspaceTimer = 0.0f; // Reset timer for next deletion
        }
    } 
    else {
        // Reset Backspace state when key is released
        backspaceHeld = false;
        initialBackspaceDelayPassed = false;
        backspaceTimer = 0.0f;
    }


    // Update the text of the nodesBox and edgesBox
    nodesBox.text = nodesInput.c_str();
    edgesBox.text = edgesInput.c_str();

}

void Graph_Menu::GetInput(int &numNodes, int &numEdges) {
    if (randomSelected) {
        std::random_device rd;
        std::mt19937 gen(rd());
        numNodes = 10 + (gen() % 10);

        int maxEdges = isDirected ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
        numEdges = numNodes + (gen() % (maxEdges - numNodes + 1));

    } else {
        try {
            numNodes = std::stoi(nodesInput);
        } catch (...) {
            numNodes = 10;
        }

        try {
            numEdges = std::stoi(edgesInput);
        } catch (...) {
            // If edges input is invalid, set a default number of edges (e.g., 2 * numNodes)
            numEdges = 2 * numNodes;
        }
    }
}

void Graph_Menu::ClearInputBoxes() {
    confirmPressed = false;
    nodesInput = "";
    edgesInput = "";
    randomSelected = false;
    nodesBoxActive = false;
    edgesBoxActive = false;

    // Reset textbox texts
    nodesBox.text = "";
    edgesBox.text = "";
}

void Graph_Menu::MakeGraph(graph* &Graphs) {
    if (!confirmPressed) return;
    int numNodes = 0, numEdges = 0;
    GetInput(numNodes, numEdges);
    ClearInputBoxes();

    if (Graphs) delete Graphs;
    Graphs = GenerateRandomGraph(numNodes, numEdges, isDirected, isWeighted);
}

void Graph_Menu::Handle(graph* &G) {
    ChooseGraphType(G);
    ChooseAlgorithms(G);
    Handle_Input();
    if (confirmPressed) MakeGraph(G);
}


void Graph_Menu::Draw() {
    // Upper box background
    upperBoxBackground.Draw_TextBox();

    Color labelColor[2] = {{79, 149, 157, 255} , {246, 248, 213, 255}};
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
    lowerBoxBackground.Draw_TextBox();

    // Lower box: Left column
    createBtn.rectColor = (selectedOption == CREATE) ? labelColor[1] : labelColor[0];
    createBtn.Draw_TextBox(selectedOption == CREATE);

    mstBtn.rectColor = (selectedOption == MST_KRUSKAL) ? labelColor[1] : labelColor[0];
    mstBtn.Draw_TextBox(selectedOption == MST_KRUSKAL);

    dijkstraBtn.rectColor = (selectedOption == DIJKSTRA) ? labelColor[1] : labelColor[0];
    dijkstraBtn.Draw_TextBox(selectedOption == DIJKSTRA);
    
    /// Draw input boxes
    nodesLabel.Draw_TextBox();
    nodesBox.Draw_TextBox();
    if (nodesBoxActive && showCursor) nodesBox.Draw_BlinkingLine();

    edgesLabel.Draw_TextBox();
    edgesBox.Draw_TextBox();
    if (edgesBoxActive && showCursor) edgesBox.Draw_BlinkingLine();

    randomBtn.rectColor = randomSelected ? LIGHTGRAY : BLACK;
    randomBtn.Draw_TextBox();

    confirmBtn.Draw_TextBox();
}