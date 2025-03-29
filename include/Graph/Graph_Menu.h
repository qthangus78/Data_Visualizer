#pragma once
#include "GlobalVar.h"
#include "graph.h"
#include "TextBox.h"
#include <string>
#include "tinyfiledialogs.h"

class GraphMenu {
public:
    // Enum classes for type safety and readability
    enum class MenuOption { CREATE, MST_KRUSKAL, DIJKSTRA, NONE };
    enum class ActiveField { NONE, NODES, EDGES, SOURCE, DEST };
    MenuOption selectedOption = MenuOption::NONE;
    ActiveField activeField = ActiveField::NONE;

    GraphMenu() {
        float upperBoxY = screenHeight - 110 - 200 - 15; // 375
        float lowerBoxY = screenHeight - 200 - 5;        // 505
    
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
    
        lowerParameterBoxBackground.rect = {lowerAlgoBoxBackground.rect.x + lowerAlgoBoxBackground.rect.width + 5, lowerBoxY - 10, 120, 210};
        lowerParameterBoxBackground.rectColor = {32, 87, 129, 255};
        lowerParameterBoxBackground.text = "";
        lowerParameterBoxBackground.fontSize = 0;
        lowerParameterBoxBackground.scaleRate = 1.0f;
        lowerParameterBoxBackground.roundness = 0.2f;
    
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
    
        // Input fields
        nodesField.label.rect = {lowerParameterBoxBackground.rect.x + 5.0f, lowerBoxY, lowerParameterBoxBackground.rect.width - 10.0f, 30};
        nodesField.label.rectColor = {79, 149, 157, 255};
        nodesField.label.text = "Vertices";
        nodesField.label.textColor = BLACK;
        nodesField.label.fontSize = 15;
        nodesField.label.scaleRate = 1.0f;
        nodesField.box.rect = {lowerParameterBoxBackground.rect.x + lowerParameterBoxBackground.rect.width + 10.0f, lowerBoxY, 100, 30};
        nodesField.box.rectColor = {246, 248, 213, 255};
        nodesField.box.text = "";
        nodesField.box.textColor = BLACK;
        nodesField.box.fontSize = 15;
        nodesField.box.scaleRate = 1.0f;
    
        edgesField.label.rect = {nodesField.label.rect.x, lowerBoxY + 40, nodesField.label.rect.width, 30};
        edgesField.label.rectColor = {79, 149, 157, 255};
        edgesField.label.text = "Edges";
        edgesField.label.textColor = BLACK;
        edgesField.label.fontSize = 15;
        edgesField.label.scaleRate = 1.0f;
        edgesField.box.rect = {nodesField.box.rect.x, lowerBoxY + 40, 100, 30};
        edgesField.box.rectColor = {246, 248, 213, 255};
        edgesField.box.text = "";
        edgesField.box.textColor = BLACK;
        edgesField.box.fontSize = 15;
        edgesField.box.scaleRate = 1.0f;
    
        sourceField.label.rect = {nodesField.label.rect.x, lowerBoxY + 80, nodesField.label.rect.width, 30};
        sourceField.label.rectColor = {79, 149, 157, 255};
        sourceField.label.text = "Source";
        sourceField.label.textColor = BLACK;
        sourceField.label.fontSize = 15;
        sourceField.label.scaleRate = 1.0f;
        sourceField.box.rect = {nodesField.box.rect.x, lowerBoxY + 80, 100, 30};
        sourceField.box.rectColor = {246, 248, 213, 255};
        sourceField.box.text = "";
        sourceField.box.textColor = BLACK;
        sourceField.box.fontSize = 15;
        sourceField.box.scaleRate = 1.0f;
    
        destField.label.rect = {nodesField.label.rect.x, lowerBoxY + 120, nodesField.label.rect.width, 30};
        destField.label.rectColor = {79, 149, 157, 255};
        destField.label.text = "Destination";
        destField.label.textColor = BLACK;
        destField.label.fontSize = 15;
        destField.label.scaleRate = 1.0f;
        destField.box.rect = {nodesField.box.rect.x, lowerBoxY + 120, 100, 30};
        destField.box.rectColor = {246, 248, 213, 255};
        destField.box.text = "";
        destField.box.textColor = BLACK;
        destField.box.fontSize = 15;
        destField.box.scaleRate = 1.0f;
    
        // File and confirm buttons
        fileBtn.rect = {lowerParameterBoxBackground.rect.x + 5.0f, lowerBoxY + 160, lowerParameterBoxBackground.rect.width - 10.0f, 35};
        fileBtn.rectColor = BLACK;
        fileBtn.text = "Add File";
        fileBtn.textColor = WHITE;
        fileBtn.fontSize = 15;
        fileBtn.scaleRate = 1.0f;
    
        confirmBtn.rect = {lowerParameterBoxBackground.rect.x + lowerParameterBoxBackground.rect.width + 10, fileBtn.rect.y, 100, 35};
        confirmBtn.rectColor = BLACK;
        confirmBtn.text = "Confirm";
        confirmBtn.textColor = WHITE;
        confirmBtn.fontSize = 15;
        confirmBtn.scaleRate = 1.1f;
    }

    void Handle(GraphVisualizer &GV);
    void Draw();

private:
    // State variables
    bool isDirected = false;
    bool isWeighted = false;
    
    bool fileSelected = false;
    bool confirmPressed = false;

    // Struct to group input field properties
    struct InputField {
        TextBox label;
        TextBox box;
        std::string input;
    };
    InputField nodesField;
    InputField edgesField;
    InputField sourceField;
    InputField destField;

    // UI elements
    TextBox upperBoxBackground;
    TextBox undirectedBtn;
    TextBox directedBtn;
    TextBox unweightedBtn;
    TextBox weightedBtn;
    TextBox lowerAlgoBoxBackground;
    TextBox createBtn;
    TextBox mstBtn;
    TextBox dijkstraBtn;
    TextBox lowerParameterBoxBackground;
    TextBox fileBtn;
    TextBox confirmBtn;

    // Cursor blinking control
    float cursorBlinkTimer = 0.0f;
    const float cursorBlinkInterval = 0.3f;
    bool showCursor = true;

    // Backspace hold-to-delete control
    float backspaceTimer = 0.0f;
    const float initialBackspaceDelay = 0.5f;
    const float repeatBackspaceDelay = 0.05f;
    bool backspaceHeld = false;
    bool initialBackspaceDelayPassed = false;

    // Helper functions with clear responsibilities
    void ChooseGraphType(GraphVisualizer &GV);
    void ChooseAlgorithms(GraphVisualizer &GV);
    void HandleInput();
    void GetInput(int& numNodes, int& numEdges, int& source, int& dest);
    void ClearInputBoxes();
    void MakeGraph(Graph* &G);
};
