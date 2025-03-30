#include "Graph_Menu.h"

GraphMenu::GraphMenu() {
    float upperBoxY = 30;
    float lowerBoxY = 30 + 110 + 25;

    // Upper box background
    upperBoxBackground.rect = {5, upperBoxY - 10, 290, 110};
    upperBoxBackground.rectColor = {32, 87, 129, 255};
    upperBoxBackground.text = "";
    upperBoxBackground.fontSize = 0;
    upperBoxBackground.scaleRate = 1.0f;

    // Toggle buttons
    undirectedBtn.rect = {25, upperBoxY, 100, 40};
    undirectedBtn.rectColor = BLACK;
    undirectedBtn.text = "Undirected";
    undirectedBtn.textColor = BLACK;
    undirectedBtn.fontSize = 15;
    undirectedBtn.scaleRate = 1.2f;

    directedBtn.rect = {175, upperBoxY, 100, 40};
    directedBtn.rectColor = BLACK;
    directedBtn.text = "Directed";
    directedBtn.textColor = BLACK;
    directedBtn.fontSize = 15;
    directedBtn.scaleRate = 1.2f;

    unweightedBtn.rect = {25, upperBoxY + 50, 100, 40};
    unweightedBtn.rectColor = BLACK;
    unweightedBtn.text = "Unweighted";
    unweightedBtn.textColor = BLACK;
    unweightedBtn.fontSize = 15;
    unweightedBtn.scaleRate = 1.2f;

    weightedBtn.rect = {175, upperBoxY + 50, 100, 40};
    weightedBtn.rectColor = BLACK;
    weightedBtn.text = "Weighted";
    weightedBtn.textColor = BLACK;
    weightedBtn.fontSize = 15;
    weightedBtn.scaleRate = 1.2f;

    // Lower box backgrounds
    lowerAlgoBoxBackground.rect = {5, lowerBoxY - 10, 150, 210};
    lowerAlgoBoxBackground.rectColor = {32, 87, 129, 255};
    lowerAlgoBoxBackground.text = "";
    lowerAlgoBoxBackground.fontSize = 0;
    lowerAlgoBoxBackground.scaleRate = 1.0f;
    lowerAlgoBoxBackground.roundness = 0.2f;

    // Option buttons
    createBtn.rect = {lowerAlgoBoxBackground.rect.x + 5, lowerAlgoBoxBackground.rect.y + 25, lowerAlgoBoxBackground.rect.width - 10, 40};
    createBtn.rectColor = {79, 149, 157, 255};
    createBtn.text = "Create";
    createBtn.textColor = BLACK;
    createBtn.fontSize = 16;
    createBtn.scaleRate = 1.0f;

    mstBtn.rect = {lowerAlgoBoxBackground.rect.x + 5, lowerAlgoBoxBackground.rect.y + 25 + 60, lowerAlgoBoxBackground.rect.width - 10, 40};
    mstBtn.rectColor = {79, 149, 157, 255};
    mstBtn.text = "MST (Kruskal)";
    mstBtn.textColor = BLACK;
    mstBtn.fontSize = 16;
    mstBtn.scaleRate = 1.0f;

    dijkstraBtn.rect = {lowerAlgoBoxBackground.rect.x + 5, lowerAlgoBoxBackground.rect.y + 25 + 120, lowerAlgoBoxBackground.rect.width - 10, 40};
    dijkstraBtn.rectColor = {79, 149, 157, 255};
    dijkstraBtn.text = "Dijkstra";
    dijkstraBtn.textColor = BLACK;
    dijkstraBtn.fontSize = 16;
    dijkstraBtn.scaleRate = 1.0f;


    // Lower box background
    float fieldHeight = 30.0f; // Height of each field
    float labelWidth = 90.0f; // Width of the label
    float boxWidth = 90.0f; // Fixed width for the input box
    float fieldSpacing = 10.0f; // Spacing between fields
    int numFields = 3; // Number of fields (Nodes, Edges, Source)
    float totalHeight = numFields * fieldHeight + (numFields - 1) * fieldSpacing + 20.0f; // Total height of fields + spacing + padding

    lowerParameterBoxBackground.rect = {160, lowerBoxY - 10, labelWidth + boxWidth + 30.0f, totalHeight}; // Adjusted width and height
    lowerParameterBoxBackground.rectColor = {32, 87, 129, 255};
    lowerParameterBoxBackground.text = "";
    lowerParameterBoxBackground.fontSize = 0;
    lowerParameterBoxBackground.scaleRate = 1.0f;
    lowerParameterBoxBackground.roundness = 0.2f;

    // Start position for fields
    float startY = lowerParameterBoxBackground.rect.y + 10.0f; // Padding from the top of the background

    // Nodes field
    nodesField.label.rect = {lowerParameterBoxBackground.rect.x + 10.0f, startY, labelWidth, fieldHeight};
    nodesField.label.rectColor = {79, 149, 157, 255};
    nodesField.label.text = "Vertices";
    nodesField.label.textColor = BLACK;
    nodesField.label.fontSize = 15;
    nodesField.label.scaleRate = 1.0f;
    nodesField.box.rect = {nodesField.label.rect.x + nodesField.label.rect.width + 10.0f, nodesField.label.rect.y, boxWidth, fieldHeight};
    nodesField.box.rectColor = {246, 248, 213, 255};
    nodesField.box.text = "";
    nodesField.box.textColor = BLACK;
    nodesField.box.fontSize = 15;
    nodesField.box.scaleRate = 1.0f;

    // Edges field
    edgesField.label.rect = {nodesField.label.rect.x, nodesField.label.rect.y + fieldHeight + fieldSpacing, labelWidth, fieldHeight};
    edgesField.label.rectColor = {79, 149, 157, 255};
    edgesField.label.text = "Edges";
    edgesField.label.textColor = BLACK;
    edgesField.label.fontSize = 15;
    edgesField.label.scaleRate = 1.0f;
    edgesField.box.rect = {edgesField.label.rect.x + edgesField.label.rect.width + 10.0f, edgesField.label.rect.y, boxWidth, fieldHeight};
    edgesField.box.rectColor = {246, 248, 213, 255};
    edgesField.box.text = "";
    edgesField.box.textColor = BLACK;
    edgesField.box.fontSize = 15;
    edgesField.box.scaleRate = 1.0f;

    // Source field
    sourceField.label.rect = {edgesField.label.rect.x, edgesField.label.rect.y + fieldHeight + fieldSpacing, labelWidth, fieldHeight};
    sourceField.label.rectColor = {79, 149, 157, 255};
    sourceField.label.text = "Source";
    sourceField.label.textColor = BLACK;
    sourceField.label.fontSize = 15;
    sourceField.label.scaleRate = 1.0f;
    sourceField.box.rect = {sourceField.label.rect.x + sourceField.label.rect.width + 10.0f, sourceField.label.rect.y, boxWidth, fieldHeight};
    sourceField.box.rectColor = {246, 248, 213, 255};
    sourceField.box.text = "";
    sourceField.box.textColor = BLACK;
    sourceField.box.fontSize = 15;
    sourceField.box.scaleRate = 1.0f;

    // File button
    fileBtn.rect = {lowerParameterBoxBackground.rect.x + 10.0f, 
        lowerParameterBoxBackground.rect.y + lowerParameterBoxBackground.rect.height + 10.0f, 
        lowerParameterBoxBackground.rect.width - 20.0f, fieldHeight};
    fileBtn.rectColor = BLACK;
    fileBtn.text = "Add File";
    fileBtn.textColor = WHITE;
    fileBtn.fontSize = 15;
    fileBtn.scaleRate = 1.1f;

    // Confirm button
    confirmBtn.rect = {fileBtn.rect.x, fileBtn.rect.y + fieldHeight + fieldSpacing, lowerParameterBoxBackground.rect.width - 20.0f, fieldHeight};
    confirmBtn.rectColor = BLACK;
    confirmBtn.text = "Confirm";
    confirmBtn.textColor = WHITE;
    confirmBtn.fontSize = 15;
    confirmBtn.scaleRate = 1.1f;

     // Background for Add/Delete Edge buttons
    edgeOperationsBackground.rect = {lowerAlgoBoxBackground.rect.x, lowerAlgoBoxBackground.rect.y + lowerAlgoBoxBackground.rect.height + 25, lowerAlgoBoxBackground.rect.width, 110};
    edgeOperationsBackground.rectColor = {32, 87, 129, 255};
    edgeOperationsBackground.text = "";
    edgeOperationsBackground.fontSize = 0;
    edgeOperationsBackground.scaleRate = 1.0f;
    edgeOperationsBackground.roundness = 0.2f;

    // Add Edge button
    addEdgeBtn.rect = {edgeOperationsBackground.rect.x + 5, edgeOperationsBackground.rect.y + 10, edgeOperationsBackground.rect.width - 10, 40};
    addEdgeBtn.rectColor = {79, 149, 157, 255};
    addEdgeBtn.text = "Add Edge";
    addEdgeBtn.textColor = BLACK;
    addEdgeBtn.fontSize = 16;
    addEdgeBtn.scaleRate = 1.0f;

    // Delete Edge button
    deleteEdgeBtn.rect = {addEdgeBtn.rect.x, addEdgeBtn.rect.y + 50, addEdgeBtn.rect.width, 40};
    deleteEdgeBtn.rectColor = {79, 149, 157, 255};
    deleteEdgeBtn.text = "Delete Edge";
    deleteEdgeBtn.textColor = BLACK;
    deleteEdgeBtn.fontSize = 16;
    deleteEdgeBtn.scaleRate = 1.0f;

    // Edge input box setup
    float totalHeight_edge = 3 * fieldHeight + 2 * fieldSpacing + 20.0f; // Height for 3 fields + spacing + padding
    float totalWidth = labelWidth + boxWidth + 30.0f; // Same width as parameter box

    edgeInputBoxBackground.rect = {
        edgeOperationsBackground.rect.x + edgeOperationsBackground.rect.width + 10.0f,
        edgeOperationsBackground.rect.y,
        totalWidth,
        totalHeight_edge
    };
    edgeInputBoxBackground.rectColor = {32, 87, 129, 255};
    edgeInputBoxBackground.roundness = 0.2f;

    startY = edgeInputBoxBackground.rect.y + 10.0f;
    float labelX = edgeInputBoxBackground.rect.x + 10.0f;
    float inputX = labelX + labelWidth + 10.0f;

    // From field
    fromField.label.rect = {labelX, startY, labelWidth, fieldHeight};
    fromField.label.rectColor = {79, 149, 157, 255};
    fromField.label.text = "From";
    fromField.label.textColor = BLACK;
    fromField.label.fontSize = 15;
    fromField.box.rect = {inputX, startY, boxWidth, fieldHeight};
    fromField.box.rectColor = {246, 248, 213, 255};
    fromField.box.textColor = BLACK;
    fromField.box.fontSize = 15;

    // To field
    toField.label.rect = {labelX, startY + fieldHeight + fieldSpacing, labelWidth, fieldHeight};
    toField.label.rectColor = {79, 149, 157, 255};
    toField.label.text = "To";
    toField.label.textColor = BLACK;
    toField.label.fontSize = 15;
    toField.box.rect = {inputX, startY + fieldHeight + fieldSpacing, boxWidth, fieldHeight};
    toField.box.rectColor = {246, 248, 213, 255};
    toField.box.textColor = BLACK;
    toField.box.fontSize = 15;

    // Weight field
    weightField.label.rect = {labelX, startY + 2 * (fieldHeight + fieldSpacing), labelWidth, fieldHeight};
    weightField.label.rectColor = {79, 149, 157, 255};
    weightField.label.text = "Weight";
    weightField.label.textColor = BLACK;
    weightField.label.fontSize = 15;
    weightField.box.rect = {inputX, startY + 2 * (fieldHeight + fieldSpacing), boxWidth, fieldHeight};
    weightField.box.rectColor = {246, 248, 213, 255};
    weightField.box.textColor = BLACK;
    weightField.box.fontSize = 15;
}

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
        if (selectedOption != MenuOption::MST_KRUSKAL) { // MST must be weighted
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
    if (CheckCollisionPointRec(mouse, createBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::CREATE;
        selectedEdgeOp = EdgeOperation::NONE;
    }
    if (CheckCollisionPointRec(mouse, mstBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::MST_KRUSKAL;
        selectedEdgeOp = EdgeOperation::NONE;
        directionType = GraphDirectionType::UNDIRECTED;
        weightType = GraphWeightType::WEIGHTED;
        if (GV.graph) {
            GV.graph->isDirected = false;
            GV.graph->isWeighted = true;
        }
    }
    if (CheckCollisionPointRec(mouse, dijkstraBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedOption = MenuOption::DIJKSTRA;
        selectedEdgeOp = EdgeOperation::NONE;
    }
    if (CheckCollisionPointRec(mouse, addEdgeBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedEdgeOp = EdgeOperation::ADD_EDGE;
        // selectedOption = MenuOption::NONE;
    }
    if (CheckCollisionPointRec(mouse, deleteEdgeBtn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedEdgeOp = EdgeOperation::DELETE_EDGE;   
        // selectedOption = MenuOption::NONE;
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
}

void GraphMenu::HandleEdgeInput() {
    if (!showEdgeInputBox) return;

    float deltaTime = GetFrameTime();
    Vector2 mouse = GetMousePosition();

    // Handle textbox activation
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
    }

    // Update cursor blinking
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= cursorBlinkInterval) {
        showCursor = !showCursor;
        cursorBlinkTimer = 0.0f;
    }

    // Handle numeric input and backspace
    int key = GetKeyPressed();
    if (key >= '0' && key <= '9') {
        if (activeEdgeField == ActiveEdgeField::FROM && fromField.input.size() <= 4) {
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

void GraphMenu::MakeGraph(Graph* &G) {
    int numNodes = 0, numEdges = 0, source = -1;
    GetInput(numNodes, numEdges, source);
    nodesField.input = std::to_string(numNodes);
    edgesField.input = std::to_string(numEdges);
    sourceField.input = std::to_string(source);
    ClearInputBoxes();

    if (G) delete G;
    bool isDirected = (directionType == GraphDirectionType::DIRECTED);
    bool isWeighted = (weightType == GraphWeightType::WEIGHTED);
    
    if (selectedOption == MenuOption::CREATE) {
        G = GenerateRandomGraph(numNodes, numEdges, isDirected, isWeighted);
    }
    else if (selectedOption == MenuOption::MST_KRUSKAL) {
        G = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
    }
    else if (selectedOption == MenuOption::DIJKSTRA) {
        G = GenerateRandomConnectedGraph(numNodes, numEdges, isDirected, isWeighted);
        G->dijkstraSource = source;
    }
}

void GraphMenu::Handle(GraphVisualizer &GV) {
    ChooseGraphType(GV);
    ChooseAlgorithms(GV);
    HandleInput();

    if (selectedEdgeOp != EdgeOperation::NONE) {
        showEdgeInputBox = true;
        HandleEdgeInput();
    } else {
        showEdgeInputBox = false;
    }

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
    DrawUpperBox();
    DrawAlgorithmOptions();
    DrawEdgeOperations();
    DrawParameterBox();
    
    bool showButtons = (selectedOption != MenuOption::NONE);
    
    if (showButtons) {
        DrawInputFields();
        DrawButtons(true);
    }

    if (showEdgeInputBox) {
        DrawEdgeInputBox();
    }
}

void GraphMenu::DrawUpperBox() {
    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};
    
    upperBoxBackground.Draw_TextBox();
    bool isDirected = (directionType == GraphDirectionType::DIRECTED);
    bool isWeighted = (weightType == GraphWeightType::WEIGHTED);
    
    undirectedBtn.rectColor = isDirected ? labelColor[0] : labelColor[1];
    undirectedBtn.Draw_TextBox(!isDirected);
    directedBtn.rectColor = isDirected ? labelColor[1] : labelColor[0];
    directedBtn.Draw_TextBox(isDirected);
    unweightedBtn.rectColor = isWeighted ? labelColor[0] : labelColor[1];
    unweightedBtn.Draw_TextBox(!isWeighted);
    weightedBtn.rectColor = isWeighted ? labelColor[1] : labelColor[0];
    weightedBtn.Draw_TextBox(isWeighted);
}

void GraphMenu::DrawAlgorithmOptions() {
    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};
    Color textHoverColor = {246, 248, 213, 255};

    lowerAlgoBoxBackground.Draw_TextBox();
    
    createBtn.rectColor = (selectedOption == MenuOption::CREATE) ? labelColor[1] : labelColor[0];
    createBtn.textColor = ShouldShowColoredText(createBtn, textHoverColor) ? BLACK : 
                         (CheckCollisionPointRec(mouse, createBtn.rect) ? textHoverColor : BLACK);
    createBtn.Draw_TextBox(selectedOption == MenuOption::CREATE);
    
    mstBtn.rectColor = (selectedOption == MenuOption::MST_KRUSKAL) ? labelColor[1] : labelColor[0];
    mstBtn.textColor = ShouldShowColoredText(mstBtn, textHoverColor) ? BLACK : 
                      (CheckCollisionPointRec(mouse, mstBtn.rect) ? textHoverColor : BLACK);
    mstBtn.Draw_TextBox(selectedOption == MenuOption::MST_KRUSKAL);
    
    dijkstraBtn.rectColor = (selectedOption == MenuOption::DIJKSTRA) ? labelColor[1] : labelColor[0];
    dijkstraBtn.textColor = ShouldShowColoredText(dijkstraBtn, textHoverColor) ? BLACK : 
                           (CheckCollisionPointRec(mouse, dijkstraBtn.rect) ? textHoverColor : BLACK);
    dijkstraBtn.Draw_TextBox(selectedOption == MenuOption::DIJKSTRA);
}

void GraphMenu::DrawEdgeOperations() {
    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};
    Color textHoverColor = {246, 248, 213, 255};

    edgeOperationsBackground.Draw_TextBox();
    
    addEdgeBtn.rectColor = (selectedEdgeOp == EdgeOperation::ADD_EDGE) ? labelColor[1] : labelColor[0];    
    addEdgeBtn.textColor = ShouldShowColoredText(addEdgeBtn, textHoverColor) ? BLACK : 
                          (CheckCollisionPointRec(mouse, addEdgeBtn.rect) ? textHoverColor : BLACK);
    addEdgeBtn.Draw_TextBox();
    
    deleteEdgeBtn.rectColor = (selectedEdgeOp == EdgeOperation::DELETE_EDGE) ? labelColor[1] : labelColor[0];      
    deleteEdgeBtn.textColor = ShouldShowColoredText(deleteEdgeBtn, textHoverColor) ? BLACK : 
                             (CheckCollisionPointRec(mouse, deleteEdgeBtn.rect) ? textHoverColor : BLACK);
    deleteEdgeBtn.Draw_TextBox();
}

void GraphMenu::DrawParameterBox() {
    if (selectedOption == MenuOption::CREATE || selectedOption == MenuOption::MST_KRUSKAL) {
        lowerParameterBoxBackground.rect.height = 30.0 * 2 + 30;
    } 
    else if (selectedOption == MenuOption::DIJKSTRA) {
        lowerParameterBoxBackground.rect.height = 30.0 * 3 + 40;
    } 
    else {
        lowerParameterBoxBackground.rect.height = 0;
    }
    lowerParameterBoxBackground.Draw_TextBox();
}

void GraphMenu::DrawInputFields() {
    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};
    Color textHoverColor = {246, 248, 213, 255};

    // Nodes field
    nodesField.label.rectColor = (activeField == ActiveField::NODES) ? labelColor[1] : labelColor[0];
    nodesField.label.textColor = ShouldShowColoredText(nodesField.label, textHoverColor) ? BLACK : 
        (CheckCollisionPointRec(mouse, nodesField.label.rect) ? textHoverColor : BLACK);
    nodesField.label.Draw_TextBox();
    if (!nodesField.input.empty() || activeField == ActiveField::NODES) {
        nodesField.box.Draw_TextBox();
        if (activeField == ActiveField::NODES && showCursor) nodesField.box.Draw_BlinkingLine();
    }

    // Edges field
    edgesField.label.rectColor = (activeField == ActiveField::EDGES) ? labelColor[1] : labelColor[0];
    edgesField.label.textColor = ShouldShowColoredText(edgesField.label, textHoverColor) ? BLACK :
        (CheckCollisionPointRec(mouse, edgesField.label.rect) ? textHoverColor : BLACK);
    edgesField.label.Draw_TextBox();
    if (!edgesField.input.empty() || activeField == ActiveField::EDGES) {
        edgesField.box.Draw_TextBox();
        if (activeField == ActiveField::EDGES && showCursor) edgesField.box.Draw_BlinkingLine();
    }

    // Source field (only for Dijkstra)
    if (selectedOption == MenuOption::DIJKSTRA) {
        sourceField.label.rectColor = (activeField == ActiveField::SOURCE) ? labelColor[1] : labelColor[0];
        sourceField.label.textColor = ShouldShowColoredText(sourceField.label, textHoverColor) ? BLACK :
            (CheckCollisionPointRec(mouse, sourceField.label.rect) ? textHoverColor : BLACK);
        sourceField.label.Draw_TextBox();
        if (!sourceField.input.empty() || activeField == ActiveField::SOURCE) {
            sourceField.box.Draw_TextBox();
            if (activeField == ActiveField::SOURCE && showCursor) sourceField.box.Draw_BlinkingLine();
        }
    }
}

void GraphMenu::DrawEdgeInputBox() {
    if (!showEdgeInputBox) return;

    Color labelColor[2] = {{79, 149, 157, 255}, {246, 248, 213, 255}};
    Color textHoverColor = {246, 248, 213, 255};

    if (weightType == GraphWeightType::UNWEIGHTED) 
        edgeInputBoxBackground.rect.height = 30.0f * 2 + 30;
    else 
        edgeInputBoxBackground.rect.height = 30.0f * 3 + 40;
    edgeInputBoxBackground.Draw_TextBox();
    
    // From field with highlighting
    fromField.label.rectColor = (activeEdgeField == ActiveEdgeField::FROM) ? labelColor[1] : labelColor[0];
    fromField.label.textColor = ShouldShowColoredText(fromField.label, textHoverColor) ? BLACK :
        (CheckCollisionPointRec(mouse, fromField.label.rect) ? textHoverColor : BLACK);
    fromField.label.Draw_TextBox();
    if (!fromField.input.empty() || activeEdgeField == ActiveEdgeField::FROM) {
        fromField.box.Draw_TextBox();
        if (activeEdgeField == ActiveEdgeField::FROM && showCursor) fromField.box.Draw_BlinkingLine();
    }
    
    // To field with highlighting
    toField.label.rectColor = (activeEdgeField == ActiveEdgeField::TO) ? labelColor[1] : labelColor[0];
    toField.label.textColor = ShouldShowColoredText(toField.label, textHoverColor) ? BLACK :
        (CheckCollisionPointRec(mouse, toField.label.rect) ? textHoverColor : BLACK);
    toField.label.Draw_TextBox();
    if (!toField.input.empty() || activeEdgeField == ActiveEdgeField::TO) {
        toField.box.Draw_TextBox();
        if (activeEdgeField == ActiveEdgeField::TO && showCursor) toField.box.Draw_BlinkingLine();
    }
    
    // Weight field with highlighting (only shown for weighted graphs)
    if (weightType == GraphWeightType::WEIGHTED) {
        weightField.label.rectColor = (activeEdgeField == ActiveEdgeField::WEIGHT) ? labelColor[1] : labelColor[0];
        weightField.label.textColor = ShouldShowColoredText(weightField.label, textHoverColor) ? BLACK :
            (CheckCollisionPointRec(mouse, weightField.label.rect) ? textHoverColor : BLACK);
        weightField.label.Draw_TextBox();
        if (!weightField.input.empty() || activeEdgeField == ActiveEdgeField::WEIGHT) {
            weightField.box.Draw_TextBox();
            if (activeEdgeField == ActiveEdgeField::WEIGHT && showCursor) weightField.box.Draw_BlinkingLine();
        }
    }
}

void GraphMenu::DrawButtons(bool showButtons) {
    if (showButtons) {
        fileBtn.Draw_TextBox();
        confirmBtn.Draw_TextBox();
    }
}

bool GraphMenu::ShouldShowColoredText(const TextBox& btn, const Color& hoverColor) const {
    return btn.rectColor.r == hoverColor.r && 
           btn.rectColor.g == hoverColor.g && 
           btn.rectColor.b == hoverColor.b;
}
