#include "AnnouncementBox.h"

Table::Table()
    : lineHeight(25.0f)
    , fontSize(16.0f)
    , headerBgColor(Color{220, 220, 220, 255})
    , rowBgColor(Color{245, 245, 245, 255})
    , borderColor(Color{180, 180, 180, 255})
    , textColor(BLACK)
{
    // Default column widths - will be overridden
    columnWidths = {70, 80, 100};
}

void Table::SetHeader(const std::vector<std::string>& headerCells) {
    header = headerCells;
}

void Table::AddRow(const std::vector<std::string>& cells, bool highlighted) {
    rows.push_back(TableRow(cells, highlighted));
    // std::cout << rows.back().data[0] << ' ' << rows.back().data[1] << ' ' << rows.back().data[2] << std::endl;
}

void Table::ClearRows() {
    rows.clear();
}

void Table::Clear() {
    header.clear();
    rows.clear();
}

void Table::SetColumnWidths(const std::vector<float>& widths) {
    columnWidths = widths;
}

void Table::SetColors(Color headerBg, Color rowBg, Color border, Color text) {
    headerBgColor = headerBg;
    rowBgColor = rowBg;
    borderColor = border;
    textColor = text;
}

void Table::SetFontSize(float size) {
    fontSize = size;
}

void Table::Draw(Rectangle rect, float& yPosition, float maxContentHeight, float baseY, Font customFont) {
    float tableWidth = rect.width - 50;
    float startX = rect.x + 20;
    
    // Draw header if it exists
    if (!header.empty()) {
        // Skip if not visible
        if (yPosition + lineHeight >= baseY && yPosition <= baseY + maxContentHeight) {
            // Draw header background
            DrawRectangle(startX, yPosition, tableWidth, lineHeight, headerBgColor);
            
            // Draw header borders
            DrawLine(startX, yPosition, startX + tableWidth, yPosition, borderColor);
            DrawLine(startX, yPosition + lineHeight, startX + tableWidth, yPosition + lineHeight, borderColor);
            
            // Draw vertical borders
            float xPos = startX;
            DrawLine(xPos, yPosition, xPos, yPosition + lineHeight, borderColor);
            
            // Draw header cells and vertical borders
            for (int i = 0; i < header.size(); i++) {
                DrawTextEx(customFont, header[i].c_str(), 
                          (Vector2){xPos + 5, yPosition + 2}, fontSize, 1.0f, textColor);
                
                xPos += columnWidths[i];
                DrawLine(xPos, yPosition, xPos, yPosition + lineHeight, borderColor);
            }
        }
        
        yPosition += lineHeight;
    }
    
    // Draw rows
    for (const auto& row : rows) {
        // Skip if not visible
        if (yPosition + lineHeight < baseY || yPosition > baseY + maxContentHeight) {
            yPosition += lineHeight;
            continue;
        }
        
        // Draw row background
        DrawRectangle(startX, yPosition, tableWidth, lineHeight, rowBgColor);
        
        // Draw row borders
        DrawLine(startX, yPosition, startX + tableWidth, yPosition, borderColor);
        DrawLine(startX, yPosition + lineHeight, startX + tableWidth, yPosition + lineHeight, borderColor);
        
        // Draw cells and vertical borders
        float xPos = startX;
        DrawLine(xPos, yPosition, xPos, yPosition + lineHeight, borderColor);
        
        // Draw row background with appropriate color
        Color rowBgColor = row.highlighted ? Color{255, 220, 220, 255} : Color{245, 245, 245, 255};
        DrawRectangle(rect.x + 20, yPosition, rect.width - 50, lineHeight, rowBgColor);
        for (int i = 0; i < row.data.size() && i < columnWidths.size(); i++) {
            DrawTextEx(customFont, row.data[i].c_str(), 
                      (Vector2){xPos + 5, yPosition + 2}, fontSize, 1.0f, textColor);

            xPos += columnWidths[i];
            DrawLine(xPos, yPosition, xPos, yPosition + lineHeight, borderColor);
        }
        yPosition += lineHeight;
    }
    
    // Add a bit of space after the table
    yPosition += lineHeight / 2;
}

bool Table::HasContent() const {
    return !rows.empty() || !header.empty();
}

float Table::GetTotalHeight() const {
    // Calculate header + rows + spacing
    float height = 0;
    if (!header.empty()) {
        height += lineHeight; // Header row
    }
    height += rows.size() * lineHeight; // Data rows
    height += lineHeight / 2; // Extra spacing after table
    return height;
}


AnnouncementBox::AnnouncementBox() 
    : rect({0, 0, 300, 500})
    , title("")
    , highlightStartLine(-1)
    , highlightEndLine(-1)
    , backgroundColor({240, 240, 240, 230})
    , borderColor({50, 50, 50, 255})
    , titleColor(DARKBLUE)
    , textColor(BLACK)
    , highlightColor({244, 67, 54, 255})
    , highlightBgColor({255, 235, 238, 150})
    , titleFontSize(20.0f)
    , contentFontSize(16.0f)
    , infoFontSize(16.0f)
    , lineHeight(25.0f)
    , indentation(15.0f)
{
}

AnnouncementBox::AnnouncementBox(Rectangle rect, const char* title)
    : rect(rect)
    , title(title)
    , highlightStartLine(-1)
    , highlightEndLine(-1)
    , backgroundColor({240, 240, 240, 230})
    , borderColor({50, 50, 50, 255})
    , titleColor(DARKBLUE)
    , textColor(BLACK)
    , highlightColor({244, 67, 54, 255})
    , highlightBgColor({255, 235, 238, 150})
    , titleFontSize(20.0f)
    , contentFontSize(16.0f)
    , infoFontSize(16.0f)
    , lineHeight(25.0f)
    , indentation(15.0f)
{
}

void AnnouncementBox::SetTitle(const char* newTitle) {
    title = newTitle;
}

void AnnouncementBox::SetContent(const std::vector<char*>& newContent) {
    content = newContent;
}

void AnnouncementBox::SetHighlightLines(int startLine, int endLine) {
    highlightStartLine = startLine;
    highlightEndLine = endLine;
}

void AnnouncementBox::AddInfoLine(const std::string& label, const std::string& value) {
    InfoLine newLine;
    newLine.label = label;  // Make a copy of the string
    newLine.value = value;  // Make a copy of the string
    infoLines.push_back(newLine);
}

void AnnouncementBox::ClearInfoLines() {
    infoLines.clear();
}

bool AnnouncementBox::HandleScrollingBar(float totalContentHeight, float maxContentHeight, float scrollY, float scrollHeight) {
    // Calculate scroll bar parameters
    bool needsScrollBar = totalContentHeight > maxContentHeight;
    
    // Early return if no scrollbar needed
    if (!needsScrollBar) {
        scrollOffset = 0;
        return false;
    }
    
    // Calculate thumb height and position
    float scrollThumbHeight = needsScrollBar ? (scrollHeight / totalContentHeight) * scrollHeight : 0;
    scrollThumbHeight = fmax(20.0f, scrollThumbHeight); // Minimum size for thumb
    
    float scrollThumbY = scrollY + (scrollOffset / totalContentHeight) * scrollHeight;
    
    // Set the member variables for rectangles
    scrollBarRect = (Rectangle){rect.x + rect.width - 15, scrollY, 10, scrollHeight};
    scrollThumbRect = (Rectangle){rect.x + rect.width - 15, scrollThumbY, 10, scrollThumbHeight};
    
    // Handle mouse wheel for scrolling
    if (needsScrollBar && CheckCollisionPointRec(GetMousePosition(), rect)) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            scrollOffset -= wheel * 20;
            scrollOffset = fmax(0, fmin(scrollOffset, totalContentHeight - maxContentHeight));
        }
    }
    
    // Handle scroll bar dragging - using instance variables now
    if (needsScrollBar) {
        if (CheckCollisionPointRec(GetMousePosition(), scrollThumbRect)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isDragging = true;
                dragOffset = GetMousePosition().y - scrollThumbY;
            }
        }
        
        if (isDragging) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                float newThumbY = GetMousePosition().y - dragOffset;
                newThumbY = fmax(scrollY, fmin(newThumbY, scrollY + scrollHeight - scrollThumbHeight));
                scrollOffset = ((newThumbY - scrollY) / scrollHeight) * totalContentHeight;
            } else {
                isDragging = false;
            }
        }
    }
    
    return needsScrollBar;
}

void AnnouncementBox::StartTable() {
    table.Clear();
    AddInfoLine("__TABLE_START__", "");
    isTableMode = true;
}

void AnnouncementBox::SetTableHeader(const std::vector<std::string>& header) {
    if (isTableMode) {
        table.SetHeader(header);
    }
}

void AnnouncementBox::AddTableRow(const std::vector<std::string>& rowData, bool highlight) {
    if (isTableMode) {
        table.AddRow(rowData, highlight);
    }
}


void AnnouncementBox::EndTable() {
    isTableMode = false;
}

void AnnouncementBox::Draw() {
    DrawRectangleRec(rect, backgroundColor);
    DrawRectangleLinesEx(rect, 1.0f, borderColor);
    
    // Draw title
    float titleY = rect.y + 10;
    Vector2 titlePos = {rect.x + rect.width / 2.0f - MeasureTextEx(customFont, title, titleFontSize, 1.0f).x / 2.0f, titleY};
    DrawTextEx(customFont, title, titlePos, titleFontSize, 1.0f, textColor);

     // Add bold underline for title
    float titleWidth = MeasureTextEx(customFont, title, titleFontSize, 1.0f).x;
    float underlineY = titleY + titleFontSize + 4;
    DrawLineEx(
        (Vector2){titlePos.x, underlineY},
        (Vector2){titlePos.x + titleWidth, underlineY},
        3.0f,  // Line thickness (bold)
        titleColor
    );
    
    // Calculate visible content height
    float codeHeight = content.size() * lineHeight;
    float infoHeight = infoLines.size() * lineHeight;
    
    // Calculate table height (header + all rows)
    float tableHeight = 0;
    if (table.HasContent()) {
        // Header row + data rows + spacing
        tableHeight = table.GetTotalHeight();
    }
    
    float totalContentHeight = codeHeight + infoHeight + tableHeight + 50; // 50 for padding
    float maxContentHeight = rect.height - 60; // Account for title and padding
    
    // Handle scrolling bar
    float scrollY = rect.y + 50;
    float scrollHeight = maxContentHeight;
    bool needsScrollBar = HandleScrollingBar(totalContentHeight, maxContentHeight, scrollY, scrollHeight);
    
    // std::cout << needsScrollBar << std::endl;

    if (needsScrollBar) {
        // Draw scroll bar
        DrawRectangleRec(scrollBarRect, Color{200, 200, 200, 100});
        DrawRectangleRec(scrollThumbRect, Color{150, 150, 150, 200});
    }
    // Apply scrolling to content
    BeginScissorMode(rect.x, rect.y + 50, rect.width - (needsScrollBar ? 20 : 0), maxContentHeight);
    
    float codeY = rect.y + 50 - scrollOffset;
    
    // Draw code with highlighting
    for (int i = 0; i < content.size(); i++) {
        float y = codeY + i * lineHeight;
        
        // Skip if not visible
        if (y + lineHeight < rect.y + 50 || y > rect.y + 50 + maxContentHeight) {
            continue;
        }
        
        Color bgColor = (i >= highlightStartLine && i <= highlightEndLine) ? highlightBgColor : backgroundColor;
        Color txtColor = (i >= highlightStartLine && i <= highlightEndLine) ? highlightColor : textColor;
        DrawRectangle(rect.x + 10, y, rect.width - 30, lineHeight, bgColor);
        DrawTextEx(customFont, content[i], (Vector2){rect.x + 15, y + 5}, contentFontSize, 1.0f, txtColor);
    }
    
    // Draw divider line
    float lineY = codeY + content.size() * lineHeight;
    
    // Draw info section
    float infoY = lineY + 20;
    
    for (int i = 0; i < infoLines.size(); i++, infoY += lineHeight) {
        const auto& info = infoLines[i];
        
        // Skip if not visible
        if (infoY + lineHeight < rect.y + 50 || infoY > rect.y + 50 + maxContentHeight) {
            continue;
        }
        
        // Check for separator marker
        if (info.label == "__SEPARATOR__") {
            // Draw a horizontal line instead of text
            DrawLine(rect.x + 20, infoY + lineHeight/2, rect.x + rect.width - 30, 
                    infoY + lineHeight/2, Color{100, 100, 100, 150});
            continue;
        }
        
        // Check for table markers
        if (info.label == "__TABLE_START__") {
            infoY += lineHeight / 2; // Add a bit of space before the table
            table.Draw(rect, infoY, maxContentHeight, rect.y + 50, customFont);
            infoY -= lineHeight; 
            continue;
        }
        
        // Regular info line
        std::string buffer = info.label + ": " + info.value;
        DrawTextEx(customFont, buffer.c_str(), (Vector2){rect.x + 20, infoY}, infoFontSize, 1.0f, textColor);
    }
    
    EndScissorMode();
}

