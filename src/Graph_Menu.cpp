#include "Graph_Menu.h"

const char* RandomText = "Random";

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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, nodesBox.rect)) {
            nodesBoxActive = true;

            cursorBlinkTimer = 0.0f; // Reset cursor blink
            showCursor = true;

            if (nodesBox.text == RandomText) {
                nodesBox.text = ""; 
            }
        } 
        else {
            nodesBoxActive = false;
        }

        if (CheckCollisionPointRec(mouse, edgesBox.rect)) {
            edgesBoxActive = true;

            cursorBlinkTimer = 0.0f; // Reset cursor blink
            showCursor = true;

            if (edgesBox.text == RandomText) {
                edgesBox.text = "";
            }
        }
        else {
            edgesBoxActive = false;
        }

        if (CheckCollisionPointRec(mouse, fileBtn.rect)) {
            fileSelected = true;
        }
        else {
            fileSelected = false;
        }

        if (CheckCollisionPointRec(mouse, confirmBtn.rect)) {
            confirmPressed = true;
        }
        else {
            confirmPressed = false;
        }
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
    if (nodesInput.empty() && !nodesBoxActive) {
        nodesBox.text = RandomText;
    }
    else {
        nodesBox.text = nodesInput.c_str();
    }

    if (edgesInput.empty() && !edgesBoxActive) {
        edgesBox.text = RandomText;
    }
    else {
        edgesBox.text = edgesInput.c_str();
    }
}

void Graph_Menu::GetInput(int &numNodes, int &numEdges) {
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
}

void Graph_Menu::ClearInputBoxes() {
    confirmPressed = false;
    nodesInput = "";
    edgesInput = "";
    fileSelected = false;
    nodesBoxActive = false;
    edgesBoxActive = false;

    // Reset textbox texts
    nodesBox.text = "";
    edgesBox.text = "";
}

void Graph_Menu::MakeGraph(graph* &Graphs) {
    // if (!confirmPressed) return;
    int numNodes = 0, numEdges = 0;
    GetInput(numNodes, numEdges);
    ClearInputBoxes();

    if (Graphs) delete Graphs;
    if (selectedOption == CREATE) Graphs = GenerateRandomGraph(numNodes, numEdges, isDirected, isWeighted);
    if (selectedOption == MST_KRUSKAL) Graphs = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
}


void Graph_Menu::Handle(graph* &G) {
    ChooseGraphType(G);
    ChooseAlgorithms(G);

    if (fileSelected) {
        const char* filePath = tinyfd_openFileDialog("Select a File", "", 0, NULL, NULL, 0);
        Handle_InputFile(filePath, G);
        fileSelected = false;
    } 
    else {
        Handle_Input();
        if (confirmPressed) MakeGraph(G);
    }
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

    fileBtn.rectColor = fileSelected ? LIGHTGRAY : BLACK;
    fileBtn.Draw_TextBox();

    confirmBtn.Draw_TextBox();
}