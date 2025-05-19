#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include "GlobalVar.h"

class Table {
public:
    Table();
    struct TableRow {
        std::vector<std::string> data;
        bool highlighted = false;

        TableRow(const std::vector<std::string>& cells, bool isHighlighted = false) 
        : data(cells), highlighted(isHighlighted) {}
    };
    
    // Add a header row (column titles)
    void SetHeader(const std::vector<std::string>& headerCells);
    
    // Add a data row
    void AddRow(const std::vector<std::string>& cells, bool highlighted = false);
    
    // Clear all rows (keeps header)
    void ClearRows();
    
    // Clear everything including header
    void Clear();
    
    // Draw the table
    void Draw(Rectangle rect, float& yPosition, float maxContentHeight, float baseY, Font customFont);
    
    // Styling options
    void SetColumnWidths(const std::vector<float>& widths);
    void SetColors(Color headerBgColor, Color rowBgColor, Color borderColor, Color textColor);
    void SetFontSize(float fontSize);
    
    bool HasContent() const;
    float GetTotalHeight() const;

private:
    std::vector<std::string> header;
    std::vector<TableRow> rows;
    std::vector<float> columnWidths;
    float lineHeight;
    float fontSize;
    
    Color headerBgColor;
    Color rowBgColor;
    Color borderColor;
    Color textColor;
};

class AnnouncementBox {
public:
    // Constructors
    AnnouncementBox();
    AnnouncementBox(Rectangle rect, const char* title);
    
    // Public interface methods
    void SetTitle(const char* title);
    void SetContent(const std::vector<char*>& content);
    void SetHighlightLines(int startLine, int endLine);
    
    void AddInfoLine(const std::string& label, const std::string& value);
    void ClearInfoLines();
    
    // Main rendering function
    void Draw();
    void DrawforTrie();
    void DrawForAVL();
    
    // Add table-specific methods
    void StartTable();
    void SetTableHeader(const std::vector<std::string>& header);
    void AddTableRow(const std::vector<std::string>& cells, bool highlightewd = false);
    void EndTable();
    
private:
    // Helper functions for internal use
    bool HandleScrollingBar(float totalContentHeight, float maxContentHeight, float scrollY, float scrollHeight);
    // void DrawTable(float& infoY, int startIndex, int endIndex, float maxContentHeight, float baseY);
    
    // Member variables
    Rectangle rect;
    const char* title;
    std::vector<char*> content;
    int highlightStartLine;
    int highlightEndLine;

    struct InfoLine {
        std::string label;
        std::string value;
    };
    std::vector<InfoLine> infoLines;
    
    // Colors and styling
    Color backgroundColor;
    Color borderColor;
    Color titleColor;
    Color textColor;
    Color highlightColor;
    Color highlightBgColor;
    
    // Size and spacing
    float titleFontSize;
    float contentFontSize;
    float infoFontSize;
    float lineHeight;
    float indentation;

    // Scrolling variables
    float scrollOffset = 0.0f;
    Rectangle scrollBarRect;
    Rectangle scrollThumbRect;

    // Add the Table object
    Table table;
    bool isTableMode;

    // Add these member variables
    bool isDragging = false;
    float dragOffset = 0.0f;
};