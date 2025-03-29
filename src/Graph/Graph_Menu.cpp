#include "Graph_Menu.h"
void GraphMenu::ChooseGraphType(GraphVisualizer &GV) {
    if (CheckCollisionPointRec(mouse, undirectedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isDirected = false;
        if (GV.graph) GV.graph->isDirected = isDirected;
    }
    if (CheckCollisionPointRec(mouse, directedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption != MenuOption::MST_KRUSKAL) { // MST must be undirected
            isDirected = true;
            if (GV.graph) GV.graph->isDirected = isDirected;
        }
    }
    if (CheckCollisionPointRec(mouse, unweightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (selectedOption != MenuOption::MST_KRUSKAL) { // MST must be weighted
            isWeighted = false;
            if (GV.graph) GV.graph->isWeighted = isWeighted;
        }
    }
    if (CheckCollisionPointRec(mouse, weightedBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isWeighted = true;
        if (GV.graph) GV.graph->isWeighted = isWeighted;
    }
}

void GraphMenu::ChooseAlgorithms(GraphVisualizer &GV) {
    if (CheckCollisionPointRec(mouse, createBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::CREATE;
    }
    if (CheckCollisionPointRec(mouse, mstBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::MST_KRUSKAL;
        isDirected = false;
        isWeighted = true;
        if (GV.graph) {
            GV.graph->isDirected = isDirected;
            GV.graph->isWeighted = isWeighted;
        }
    }
    if (CheckCollisionPointRec(mouse, dijkstraBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::DIJKSTRA;
    }
}

void GraphMenu::HandleInput() {
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
        else if (selectedOption == MenuOption::DIJKSTRA &&
                 (CheckCollisionPointRec(mouse, destField.label.rect) ||
                  ((destField.input.size() > 0 || activeField == ActiveField::DEST) && CheckCollisionPointRec(mouse, destField.box.rect)))) {
            activeField = ActiveField::DEST;
            cursorBlinkTimer = 0.0f;
            showCursor = true;
        }
        else {
            activeField = ActiveField::NONE;
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
        else if (activeField == ActiveField::DEST && destField.input.size() <= 4) {
            destField.input += static_cast<char>(key);
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
            else if (activeField == ActiveField::DEST && !destField.input.empty()) destField.input.pop_back();
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
            else if (activeField == ActiveField::DEST && !destField.input.empty()) destField.input.pop_back();
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
    destField.box.text = destField.input.c_str();
}

void GraphMenu::GetInput(int& numNodes, int& numEdges, int& source, int& dest) {
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
        int maxEdges = isDirected ? numNodes * (numNodes - 1) : numNodes * (numNodes - 1) / 2;
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
        try {
            dest = std::stoi(destField.input);
        }
        catch (...) {
            std::random_device rd;
            std::mt19937 gen(rd());
            dest = 1 + (gen() % numNodes);
        }
    }
    else {
        source = -1;
        dest = -1;
    }
}

void GraphMenu::ClearInputBoxes() {
    confirmPressed = false;
    fileSelected = false;
    activeField = ActiveField::NONE;
}

void GraphMenu::MakeGraph(Graph* &G) {
    int numNodes = 0, numEdges = 0, source = -1, dest = -1;
    GetInput(numNodes, numEdges, source, dest);
    ClearInputBoxes();

    if (G) delete G;
    if (selectedOption == MenuOption::CREATE) {
        G = GenerateRandomGraph(numNodes, numEdges, isDirected, isWeighted);
    }
    else if (selectedOption == MenuOption::MST_KRUSKAL) {
        G = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
    }
    else if (selectedOption == MenuOption::DIJKSTRA) {
        G = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
        G->DIJKSTRA_parameters = {source, dest};
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
        MakeGraph(GV.graph);
        GV.initEadesFactor();
    }
}

void GraphMenu::Draw() {
    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};

    // Upper box
    upperBoxBackground.Draw_TextBox();
    undirectedBtn.rectColor = isDirected ? labelColor[0] : labelColor[1];
    undirectedBtn.Draw_TextBox(!isDirected);
    directedBtn.rectColor = isDirected ? labelColor[1] : labelColor[0];
    directedBtn.Draw_TextBox(isDirected);
    unweightedBtn.rectColor = isWeighted ? labelColor[0] : labelColor[1];
    unweightedBtn.Draw_TextBox(!isWeighted);
    weightedBtn.rectColor = isWeighted ? labelColor[1] : labelColor[0];
    weightedBtn.Draw_TextBox(isWeighted);

    // Lower box: Algorithm options
    lowerAlgoBoxBackground.Draw_TextBox();
    createBtn.rectColor = (selectedOption == MenuOption::CREATE) ? labelColor[1] : labelColor[0];
    createBtn.Draw_TextBox(selectedOption == MenuOption::CREATE);
    mstBtn.rectColor = (selectedOption == MenuOption::MST_KRUSKAL) ? labelColor[1] : labelColor[0];
    mstBtn.Draw_TextBox(selectedOption == MenuOption::MST_KRUSKAL);
    dijkstraBtn.rectColor = (selectedOption == MenuOption::DIJKSTRA) ? labelColor[1] : labelColor[0];
    dijkstraBtn.Draw_TextBox(selectedOption == MenuOption::DIJKSTRA);

    // Input fields
    if (selectedOption != MenuOption::NONE) {
        lowerParameterBoxBackground.Draw_TextBox();

        nodesField.label.rectColor = (activeField == ActiveField::NODES) ? labelColor[1] : labelColor[0];
        nodesField.label.Draw_TextBox();
        if (!nodesField.input.empty() || activeField == ActiveField::NODES) nodesField.box.Draw_TextBox();
        if (activeField == ActiveField::NODES && showCursor) nodesField.box.Draw_BlinkingLine();

        edgesField.label.rectColor = (activeField == ActiveField::EDGES) ? labelColor[1] : labelColor[0];
        edgesField.label.Draw_TextBox();
        if (!edgesField.input.empty() || activeField == ActiveField::EDGES) edgesField.box.Draw_TextBox();
        if (activeField == ActiveField::EDGES && showCursor) edgesField.box.Draw_BlinkingLine();

        if (selectedOption == MenuOption::DIJKSTRA) {
            sourceField.label.rectColor = (activeField == ActiveField::SOURCE) ? labelColor[1] : labelColor[0];
            sourceField.label.Draw_TextBox();
            if (!sourceField.input.empty() || activeField == ActiveField::SOURCE) sourceField.box.Draw_TextBox();
            if (activeField == ActiveField::SOURCE && showCursor) sourceField.box.Draw_BlinkingLine();

            destField.label.rectColor = (activeField == ActiveField::DEST) ? labelColor[1] : labelColor[0];
            destField.label.Draw_TextBox();
            if (!destField.input.empty() || activeField == ActiveField::DEST) destField.box.Draw_TextBox();
            if (activeField == ActiveField::DEST && showCursor) destField.box.Draw_BlinkingLine();
        }
    }

    // Buttons
    fileBtn.rectColor = fileSelected ? LIGHTGRAY : BLACK;
    fileBtn.Draw_TextBox();
    confirmBtn.Draw_TextBox();
}