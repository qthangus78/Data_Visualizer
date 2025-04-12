#include "Graph_Display.h"


void RunGraphVisualization(GraphVisualizer &visualizer) {
    if (!visualizer.checkGraph()) return;
    visualizer.HandleMouseInteraction();
    visualizer.BalanceGraph();
    visualizer.Draw();
}

void RunGraphVisualization_MST(GraphVisualizer &visualizer) {
    if (!visualizer.checkGraph()) return;
    visualizer.HandleMouseInteraction();
    visualizer.BalanceGraph();

    if (visualizer.isKruskalInited() == false) {
        visualizer.Draw();
    }
    else {
        visualizer.DrawMST_StepByStep();
    }
}

void RunGraphVisualization_DIJKSTRA(GraphVisualizer &visualizer) {
    if (!visualizer.checkGraph()) return;
    visualizer.HandleMouseInteraction();
    visualizer.BalanceGraph();

    if (visualizer.graph->dijkstraSource == -1) {
        visualizer.Draw();
    }
    else {
        visualizer.DrawDIJKSTRA_StepByStep();
    }
}


namespace Graph_display {
    GraphMenu Gmenu;
    GraphVisualizer myGraphVisualizer;

    void SetUpTextureButtons() {
        float centerX = GraphDisplayScreen.x + GraphDisplayScreen.width / 2;

        PlayButton.SetPosition(centerX - PlayButton.MainTexture.width / 2, 
                GraphDisplayScreen.y + GraphDisplayScreen.height);

        PauseButton.SetPosition(centerX - PlayButton.MainTexture.width / 2, 
                GraphDisplayScreen.y + GraphDisplayScreen.height);

        const float BUTTON_SPACING = 50;  
        
        UndoButton.SetPosition(PlayButton.x - BUTTON_SPACING - UndoButton.width, 
                PlayButton.y + 10);

        RedoButton.SetPosition(PlayButton.x + PlayButton.width + BUTTON_SPACING, 
                PlayButton.y + 10);

        if (myGraphVisualizer.isPaused() && PlayButton.isPressed()) {
            myGraphVisualizer.Resume();
        }
        else if (!myGraphVisualizer.isPaused() && PauseButton.isPressed()) {
            myGraphVisualizer.Pause();
        }

        myGraphVisualizer.initSpeedControler({GraphDisplayScreen.x + GraphDisplayScreen.width + 20 + 150, 
                PlayButton.y + 10 + 11});
        myGraphVisualizer.UpdateSpeedControler();
    }

    void DrawTextureButtons() {
        if (myGraphVisualizer.isPaused()) {
            PlayButton.Drawtexture();
        }
        else {
            PauseButton.Drawtexture();
        }
        UndoButton.Drawtexture();
        RedoButton.Drawtexture();

        myGraphVisualizer.DrawSpeedControler();
    }

    void Display() {
        ClearBackground({192, 245, 242, 100});
        display_title("Graphs", ScreenID::StartMenuScreen);
        DrawRectangleRec(GraphDisplayScreen, {238, 241, 218, 255}); // rgb(51, 145, 154)

        // Draw mytexture
        SetUpTextureButtons();
        DrawTextureButtons();

        Gmenu.Handle(myGraphVisualizer);
        Gmenu.Draw();

        if (myGraphVisualizer.checkGraph()) {
            if (Gmenu.selectedOption == GraphMenu::MenuOption::CREATE) {
                RunGraphVisualization(myGraphVisualizer);
            }
            if (Gmenu.selectedOption == GraphMenu::MenuOption::MST_KRUSKAL) {
                RunGraphVisualization_MST(myGraphVisualizer);
            }
            if (Gmenu.selectedOption == GraphMenu::MenuOption::DIJKSTRA) {
                RunGraphVisualization_DIJKSTRA(myGraphVisualizer);
            }
        }
    }
}
