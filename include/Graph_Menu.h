#pragma once
#include "GlobalVar.h"
#include "graph.h"
#include "TextBox.h"
#include <string>

struct Graph_Menu {
    // Upper box: Directed/Undirected and Weighted/Unweighted toggles

    bool isDirected = false;
    bool isWeighted = false;

    // Lower box: Options and textboxes
    enum MenuOption { CREATE, MST_KRUSKAL, DIJKSTRA, NONE };
    MenuOption selectedOption = CREATE;

    // Textbox inputs for "Nodes" and "Edges"
    std::string nodesInput = "";
    std::string edgesInput = "";
    bool randomSelected = false;

    // Textbox states
    bool nodesBoxActive = false;
    bool edgesBoxActive = false;
    
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
    TextBox lowerBoxBackground;
    TextBox createBtn;
    TextBox mstBtn;
    TextBox dijkstraBtn;

    // Lower box: Right column (Textboxes for "Create" option)
    TextBox nodesLabel;  // Label for "Nodes"
    TextBox nodesBox;    // Input box for nodes
    TextBox edgesLabel;  // Label for "Edges"
    TextBox edgesBox;    // Input box for edges
    TextBox randomBtn;
    TextBox confirmBtn;

    // Blinking cursor variables
    float cursorBlinkTimer = 0.0f;
    const float cursorBlinkInterval = 0.3f; // Blink every 0.5 seconds
    bool showCursor = true;

    // Constructor
    Graph_Menu() {
        // Initialize positions based on screenHeight
        upperBoxY = screenHeight - 110 - 170 - 30;  // 110 (upper box height) + 170 (lower box height) + 20 (padding)
        lowerBoxY = screenHeight - 170 - 10;  // 170 (lower box height) + 20 (padding from bottom)

        // Upper box background
        upperBoxBackground.rect = {20, upperBoxY - 10, 290, 110};
        upperBoxBackground.rectColor = {32, 87, 129, 255};
        upperBoxBackground.text = "";  // No text for background
        upperBoxBackground.fontSize = 0;  // No text rendering
        upperBoxBackground.scaleRate = 1.0f;  // No scaling for background
        // upperBoxBackground.roundness = 0.0f; // rectangle

        // Upper box: Toggle buttons
        undirectedBtn.rect = {30 + 10, upperBoxY, 100, 40};
        undirectedBtn.rectColor = BLACK;
        undirectedBtn.text = "Undirected";
        undirectedBtn.textColor = BLACK;
        undirectedBtn.fontSize = 15;
        undirectedBtn.scaleRate = 1.2f;

        directedBtn.rect = {160 + 30, upperBoxY, 100, 40};
        directedBtn.rectColor = BLACK;
        directedBtn.text = "Directed";
        directedBtn.textColor = BLACK;
        directedBtn.fontSize = 15;
        directedBtn.scaleRate = 1.2f;

        unweightedBtn.rect = {30 + 10, upperBoxY + 50, 100, 40};
        unweightedBtn.rectColor = BLACK;
        unweightedBtn.text = "Unweighted";
        unweightedBtn.textColor = BLACK;
        unweightedBtn.fontSize = 15;
        unweightedBtn.scaleRate = 1.2f;

        weightedBtn.rect = {160 + 30, upperBoxY + 50, 100, 40};
        weightedBtn.rectColor = BLACK;
        weightedBtn.text = "Weighted";
        weightedBtn.textColor = BLACK;
        weightedBtn.fontSize = 15;
        weightedBtn.scaleRate = 1.2f;

        // Lower box background
        lowerBoxBackground.rect = {20, lowerBoxY - 20, 330, 180};
        lowerBoxBackground.rectColor = {32, 87, 129, 255};
        lowerBoxBackground.text = "";
        lowerBoxBackground.fontSize = 0;
        lowerBoxBackground.scaleRate = 1.0f;
        // lowerBoxBackground.roundness = 0.0f;

        // Lower box: Left column options
        createBtn.rect = {30, lowerBoxY, 100, 40};
        createBtn.rectColor = {79, 149, 157, 255};
        createBtn.text = "Create";
        createBtn.textColor = BLACK;
        createBtn.fontSize = 15;
        createBtn.scaleRate = 1.1f;

        mstBtn.rect = {30, lowerBoxY + 50, 100, 40};
        mstBtn.rectColor = {79, 149, 157, 255};
        mstBtn.text = "MST (Kruskal)";
        mstBtn.textColor = BLACK;
        mstBtn.fontSize = 15;
        mstBtn.scaleRate = 1.1f;

        dijkstraBtn.rect = {30, lowerBoxY + 100, 100, 40};
        dijkstraBtn.rectColor = {79, 149, 157, 255};
        dijkstraBtn.text = "Dijkstra";
        dijkstraBtn.textColor = BLACK;
        dijkstraBtn.fontSize = 15;
        dijkstraBtn.scaleRate = 1.1f;

        // Lower box: Right column (Textboxes for "Create")
        nodesLabel.rect = {180, lowerBoxY - 20 + 10, 150, 15};  // Smaller height for label
        nodesLabel.rectColor = {79, 149, 157, 255};  // Same as background to blend in
        nodesLabel.text = "Nodes";
        nodesLabel.textColor = BLACK;
        nodesLabel.fontSize = 15;
        nodesLabel.scaleRate = 1.0f;  // No scaling for labels
        nodesLabel.roundness = 0.0f;

        nodesBox.rect = {180, lowerBoxY - 5 + 10, 150, 40};
        nodesBox.rectColor = {246, 248, 213, 255};
        nodesBox.text = "";  // Will be updated with nodesInput
        nodesBox.textColor = BLACK;
        nodesBox.fontSize = 20;
        nodesBox.scaleRate = 1.0f;
        nodesBox.roundness = 0.0f;

        edgesLabel.rect = {180, lowerBoxY + 50, 150, 15};
        edgesLabel.rectColor = {79, 149, 157, 255};
        edgesLabel.text = "Edges";
        edgesLabel.textColor = BLACK;
        edgesLabel.fontSize = 15;
        edgesLabel.scaleRate = 1.0f;
        edgesLabel.roundness = 0.0f;

        edgesBox.rect = {180, lowerBoxY + 50 + 10 + 5, 150, 40};
        edgesBox.rectColor = {246, 248, 213, 255};
        edgesBox.text = "";
        edgesBox.textColor = BLACK;
        edgesBox.fontSize = 20;
        edgesBox.scaleRate = 1.0f;
        edgesBox.roundness = 0.0f;

        randomBtn.rect = {180, lowerBoxY + 100 + 10, 70, 40};
        randomBtn.rectColor = BLACK;
        randomBtn.text = "Random";
        randomBtn.textColor = WHITE;
        randomBtn.fontSize = 15;
        randomBtn.scaleRate = 1.1f;

        confirmBtn.rect = {260, lowerBoxY + 100 + 10, 70, 40};
        confirmBtn.rectColor = BLACK;
        confirmBtn.text = "Confirm";
        confirmBtn.textColor = WHITE;
        confirmBtn.fontSize = 15;
        confirmBtn.scaleRate = 1.1f;
    }

    void ChooseGraphType();
    void ChooseAlgorithms();
    void Handle_Create();
    void Handle();
    void Draw();
};