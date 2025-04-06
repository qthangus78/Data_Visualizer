#pragma once
#include "GlobalVar.h"
#include "graph.h"
#include "TextBox.h"
#include "tinyfiledialogs.h"
#include "random"

class GraphMenu {
public:
    enum class GraphDirectionType { DIRECTED, UNDIRECTED };
    enum class GraphWeightType { WEIGHTED, UNWEIGHTED };
    enum class MenuOption { CREATE, MST_KRUSKAL, DIJKSTRA, NONE };
    enum class EdgeOperation { ADD_EDGE, DELETE_EDGE, NONE };
    enum class ActiveField { NONE, NODES, EDGES, SOURCE };
    enum class ActiveEdgeField { FROM, TO, WEIGHT, NONE };

    MenuOption selectedOption = MenuOption::NONE;
    EdgeOperation selectedEdgeOp = EdgeOperation::NONE;
    ActiveField activeField = ActiveField::NONE;
    ActiveEdgeField activeEdgeField = ActiveEdgeField::NONE;

    // Constructor
    GraphMenu(); // Declaration only
    void Handle(GraphVisualizer &GV);
    void Draw();

private:
    // State variables
    GraphDirectionType directionType = GraphDirectionType::UNDIRECTED;
    GraphWeightType weightType = GraphWeightType::UNWEIGHTED;

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
    bool showButtons = false; // Control visibility of buttons
    TextBox showParameterBoxBtn;


    // buttons for edge operations
    TextBox addEdgeBtn;
    TextBox deleteEdgeBtn;
    TextBox edgeOperationsBackground; // Background for Add/Delete Edge buttons

    // Edge operation input box
    TextBox edgeInputBoxBackground;
    InputField fromField;  // For source vertex
    InputField toField;    // For destination vertex
    InputField weightField; // For edge weight
    TextBox edgeConfirmBtn;  

    TextBox showEdgeInputBoxBtn;
    bool showEdgeInputBox = false;
    

    
    // buttons for clearing graph
    TextBox clearGraphBackground;
    TextBox clearGraphBtn;
    TextBox clearGraphConfirmBtn;
    bool showClearConfirm = false;
    bool clearConfirmPressed = false;


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
    void HandleEdgeOperations(GraphVisualizer &GV);
    void HandleClearGraphBtn(GraphVisualizer &GV);
    void GetInput(int& numNodes, int& numEdges, int& source);
    void GetEdgeInput(int& from, int& to, int& weight); // Add this line
    void ClearInputBoxes();
    void ClearEdgeInputBoxes();
    void MakeGraph(Graph* &G);

    // methods for drawing
    void DrawGraphtypeBox();
    void DrawAlgorithmOptions();
    void DrawEdgeOperations();
    void DrawParameterBox();
    void DrawEdgeInputBox();
    void DrawShowEdgeInputBoxBtn();
    void DrawShowParameterBoxBtn();
    void DrawButtons();
    void DrawClearGraphBtn();
    bool ShouldShowColoredText(const TextBox& btn, const Color& hoverColor) const;
};


