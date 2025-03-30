#pragma once
#include "GlobalVar.h"
#include "graph.h"
#include "TextBox.h"
#include <string>
#include "tinyfiledialogs.h"

class GraphMenu {
public:
    // Enum classes for type safety and readability
    enum class MenuOption { CREATE, MST_KRUSKAL, DIJKSTRA, ADD_EDGE, DELETE_EDGE, NONE };
    enum class ActiveField { NONE, NODES, EDGES, SOURCE };
    MenuOption selectedOption = MenuOption::NONE;
    ActiveField activeField = ActiveField::NONE;

    // Constructor
    GraphMenu(); // Declaration only
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

    // New buttons for edge operations
    TextBox addEdgeBtn;
    TextBox deleteEdgeBtn;

    // New UI elements for edge operations
    TextBox edgeOperationsBackground; // Background for Add/Delete Edge buttons

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
    void GetInput(int& numNodes, int& numEdges, int& source);
    void ClearInputBoxes();
    void MakeGraph(Graph* &G);

    // New private helper methods for drawing
    void DrawUpperBox();
    void DrawAlgorithmOptions();
    void DrawEdgeOperations();
    void DrawParameterBox();
    void DrawInputFields();
    void DrawButtons(bool showButtons);
    bool ShouldShowColoredText(const TextBox& btn, const Color& hoverColor) const;
};
