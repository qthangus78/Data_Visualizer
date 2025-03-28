#pragma once
#include "GlobalVar.h"
#include "graph.h"
#include "TextBox.h"
#include <string>
#include "tinyfiledialogs.h"

struct Graph_Menu {
    // Upper box: Directed/Undirected and Weighted/Unweighted toggles
    bool isDirected = false;
    bool isWeighted = false;

    // Lower box: Options and textboxes
    enum MenuOption { CREATE, MST_KRUSKAL, DIJKSTRA, NONE };
    MenuOption selectedOption = NONE;

    // Textbox inputs for "Nodes" and "Edges"
    std::string nodesInput = "";
    std::string edgesInput = "";
    std::string sourceInput = "";
    std::string destInput = "";
    bool fileSelected = false;

    // Textbox states
    bool nodesBoxActive = false;
    bool edgesBoxActive = false;
    bool sourceBoxActive = false;
    bool destBoxActive = false;
    
    // Confirm button state
    bool confirmPressed = false;

    // Upper box: Background and toggle buttons (positioned in bottom-left corner)
    float upperBoxY;
    TextBox upperBoxBackground;
    TextBox undirectedBtn;
    TextBox directedBtn;
    TextBox unweightedBtn;
    TextBox weightedBtn;
    
    // Lower box: Background and left column options (positioned in bottom-left corner)
    float lowerBoxY;
    TextBox lowerAlgoBoxBackground;
    TextBox createBtn;
    TextBox mstBtn;
    TextBox dijkstraBtn;

    TextBox lowerParameterBoxBackground;
    TextBox fileBtn;
    TextBox confirmBtn;

    // Lower box: Right column (Textboxes for "Create" option)
    TextBox nodesLabel;  // Label for "Nodes"
    TextBox nodesBox;    // Input box for nodes
    TextBox edgesLabel;  // Label for "Edges"
    TextBox edgesBox;    // Input box for edges
    // Textboxes for "Source" and "Destination" (for Dijkstra)
    TextBox sourceLabel;  // Label for "Source"
    TextBox sourceBox;    // Input box for source
    TextBox destLabel;    // Label for "Destination"
    TextBox destBox;      // Input box for destination

    // Blinking cursor variables
    float cursorBlinkTimer = 0.0f;
    const float cursorBlinkInterval = 0.3f; // Blink every 0.5 seconds
    bool showCursor = true;

    // Backspace hold-to-delete variables
    float backspaceTimer = 0.0f;
    const float initialBackspaceDelay = 0.5f; // Initial delay before continuous deletion
    const float repeatBackspaceDelay = 0.05f; // Delay between deletions when holding
    bool backspaceHeld = false;
    bool initialBackspaceDelayPassed = false;

    // Constructor
    Graph_Menu() {
        // Initialize positions based on screenHeight (screenHeight = 700)
        upperBoxY = screenHeight - 110 - 200 - 15;  // 700 - 110 - 200 - 15 = 375
        lowerBoxY = screenHeight - 200 - 5;  // 700 - 200 + 5 = 505

        // Upper box background
        upperBoxBackground.rect = {5, upperBoxY - 10, 290, 110};  // {5, 365, 290, 110}
        upperBoxBackground.rectColor = {32, 87, 129, 255};
        upperBoxBackground.text = "";
        upperBoxBackground.fontSize = 0;
        upperBoxBackground.scaleRate = 1.0f;

        // Upper box: Toggle buttons
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

        // Lower box background
        lowerAlgoBoxBackground.rect = {5, lowerBoxY - 10, 150, 210};  // {5, 485, 340, 200}
        lowerAlgoBoxBackground.rectColor = {32, 87, 129, 255};
        lowerAlgoBoxBackground.text = "";
        lowerAlgoBoxBackground.fontSize = 0;
        lowerAlgoBoxBackground.scaleRate = 1.0f;
        lowerAlgoBoxBackground.roundness = 0.2f;

        lowerParameterBoxBackground.rect = {
            lowerAlgoBoxBackground.rect.x + lowerAlgoBoxBackground.rect.width + 5,
            lowerAlgoBoxBackground.rect.y,
            120, 210
        };
        lowerParameterBoxBackground.rectColor = {32, 87, 129, 255};
        lowerParameterBoxBackground.text = "";
        lowerParameterBoxBackground.fontSize = 0;
        lowerParameterBoxBackground.scaleRate = 1.0f;
        lowerParameterBoxBackground.roundness = 0.2f;

        // Lower box: Left column options
        createBtn.rect = {lowerAlgoBoxBackground.rect.x + 5, lowerAlgoBoxBackground.rect.y + 25, lowerAlgoBoxBackground.rect.width - 10, 40};
        createBtn.rectColor = {79, 149, 157, 255};
        createBtn.text = "Create";
        createBtn.textColor = BLACK;
        createBtn.fontSize = 16;
        createBtn.scaleRate = 1.0f;
        // createBtn.roundness = 0.0f;

        mstBtn.rect = {lowerAlgoBoxBackground.rect.x + 5, lowerAlgoBoxBackground.rect.y + 25 + 60, lowerAlgoBoxBackground.rect.width - 10, 40};
        mstBtn.rectColor = {79, 149, 157, 255};
        mstBtn.text = "MST (Kruskal)";
        mstBtn.textColor = BLACK;
        mstBtn.fontSize = 16;
        mstBtn.scaleRate = 1.0f;
        // mstBtn.roundness = 0.0f;

        dijkstraBtn.rect = {lowerAlgoBoxBackground.rect.x + 5, lowerAlgoBoxBackground.rect.y + 25 + 120, lowerAlgoBoxBackground.rect.width - 10, 40};
        dijkstraBtn.rectColor = {79, 149, 157, 255};
        dijkstraBtn.text = "Dijkstra";
        dijkstraBtn.textColor = BLACK;
        dijkstraBtn.fontSize = 16;
        dijkstraBtn.scaleRate = 1.0f;
        // dijkstraBtn.roundness = 0.0f;

        nodesLabel.rect = {lowerParameterBoxBackground.rect.x + 5.0f, lowerBoxY, 
                lowerParameterBoxBackground.rect.width - 10.0f, 30}; 
        nodesLabel.rectColor = {79, 149, 157, 255};
        nodesLabel.text = "Vertices";
        nodesLabel.textColor = BLACK;
        nodesLabel.fontSize = 15;
        nodesLabel.scaleRate = 1.0f;
        // nodesLabel.roundness = 0.0f;

        nodesBox.rect = {lowerParameterBoxBackground.rect.x + lowerParameterBoxBackground.rect.width + 10.0f,
                    lowerBoxY, 100, 30};  // Input box on the right
        nodesBox.rectColor = {246, 248, 213, 255};
        nodesBox.text = "";
        nodesBox.textColor = BLACK;
        nodesBox.fontSize = 15;
        nodesBox.scaleRate = 1.0f;
        // nodesBox.roundness = 0.0f;

        edgesLabel.rect = {nodesLabel.rect.x, lowerBoxY + 40, 
                nodesLabel.rect.width, 30};
        edgesLabel.rectColor = {79, 149, 157, 255};
        edgesLabel.text = "Edges";
        edgesLabel.textColor = BLACK;
        edgesLabel.fontSize = 15;
        edgesLabel.scaleRate = 1.0f;
        // edgesLabel.roundness = 0.0f;

        edgesBox.rect = {nodesBox.rect.x, lowerBoxY + 40, 100, 30};
        edgesBox.rectColor = {246, 248, 213, 255};
        edgesBox.text = "";
        edgesBox.textColor = BLACK;
        edgesBox.fontSize = 15;
        edgesBox.scaleRate = 1.0f;
        // edgesBox.roundness = 0.0f;

        // Textboxes for "Source" and "Destination" (for Dijkstra)
        sourceLabel.rect = {nodesLabel.rect.x, lowerBoxY + 80, 
            nodesLabel.rect.width, 30};
        sourceLabel.rectColor = {79, 149, 157, 255};
        sourceLabel.text = "Source";
        sourceLabel.textColor = BLACK;
        sourceLabel.fontSize = 15;
        sourceLabel.scaleRate = 1.0f;
        // sourceLabel.roundness = 0.0f;

        sourceBox.rect = {nodesBox.rect.x, lowerBoxY + 80, 100, 30};
        sourceBox.rectColor = {246, 248, 213, 255};
        sourceBox.text = "";
        sourceBox.textColor = BLACK;
        sourceBox.fontSize = 15;
        sourceBox.scaleRate = 1.0f;
        // sourceBox.roundness = 0.0f;

        destLabel.rect = {nodesLabel.rect.x, lowerBoxY + 120, nodesLabel.rect.width, 30};
        destLabel.rectColor = {79, 149, 157, 255};
        destLabel.text = "Destination";
        destLabel.textColor = BLACK;
        destLabel.fontSize = 15;
        destLabel.scaleRate = 1.0f;
        // destLabel.roundness = 0.0f;

        destBox.rect = {nodesBox.rect.x, lowerBoxY + 120, 100, 30};
        destBox.rectColor = {246, 248, 213, 255};
        destBox.text = "";
        destBox.textColor = BLACK;
        destBox.fontSize = 15;
        destBox.scaleRate = 1.0f;
        // destBox.roundness = 0.0f;

        fileBtn.rect = {lowerParameterBoxBackground.rect.x + 5.0f, 
            lowerBoxY + 160, lowerParameterBoxBackground.rect.width - 10.0f, 35};
        fileBtn.rectColor = BLACK;
        fileBtn.text = "Add File";
        fileBtn.textColor = WHITE;
        fileBtn.fontSize = 15;
        fileBtn.scaleRate = 1.0f;

        confirmBtn.rect = {lowerParameterBoxBackground.rect.x + lowerParameterBoxBackground.rect.width + 10, 
                fileBtn.rect.y, 
                100, 35};
        confirmBtn.rectColor = BLACK;
        confirmBtn.text = "Confirm";
        confirmBtn.textColor = WHITE;
        confirmBtn.fontSize = 15;
        confirmBtn.scaleRate = 1.1f;
    }

    void ChooseGraphType(graph* &G);
    void ChooseAlgorithms(graph* &G);
    void Handle_Input();
    void GetInput(int &numNodes, int &numEdges, int &source, int &dest);
    void ClearInputBoxes();
    void Handle(graph* &G);
    void Draw();
    void MakeGraph(graph* &Graphs);
};