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
        bool headerVisible = (yPosition + lineHeight > baseY && yPosition < baseY + maxContentHeight);
        if (headerVisible) {
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
        bool isVisible = (yPosition + lineHeight > baseY && yPosition < baseY + maxContentHeight);
        
        if (isVisible) {
            // Draw row only if visible
            Color rowBgColor = row.highlighted ? Color{255, 220, 220, 255} : Color{245, 245, 245, 255};
            DrawRectangle(startX, yPosition, tableWidth, lineHeight, rowBgColor);
            
            // Draw borders
            DrawLine(startX, yPosition, startX + tableWidth, yPosition, borderColor);
            DrawLine(startX, yPosition + lineHeight, startX + tableWidth, yPosition + lineHeight, borderColor);
            
            float xPos = startX;
            DrawLine(xPos, yPosition, xPos, yPosition + lineHeight, borderColor);
            
            // Draw cells
            for (int i = 0; i < row.data.size() && i < columnWidths.size(); i++) {
                DrawTextEx(customFont, row.data[i].c_str(), 
                         (Vector2){xPos + 5, yPosition + 2}, fontSize, 1.0f, textColor);
                
                xPos += columnWidths[i];
                DrawLine(xPos, yPosition, xPos, yPosition + lineHeight, borderColor);
            }
        }
        
        // Always increment position to properly calculate total height
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
    
    // Calculate maximum scroll offset
    float maxScrollOffset = totalContentHeight - maxContentHeight;
    
    // Clamp current scrollOffset to valid range
    scrollOffset = fmax(0, fmin(scrollOffset, maxScrollOffset));
    // std::cout << "scrollOffset " << scrollOffset << std::endl;
    
    // Calculate thumb height to represent visible portion
    float thumbRatio = maxContentHeight / totalContentHeight;
    float scrollThumbHeight = scrollHeight * thumbRatio;
    scrollThumbHeight = fmax(20.0f, scrollThumbHeight); // Minimum size for thumb
    
    // Calculate thumb position based on scroll offset
    float scrollableTrackHeight = scrollHeight - scrollThumbHeight;
    float scrollRatio = (maxScrollOffset > 0) ? (scrollOffset / maxScrollOffset) : 0;
    float scrollThumbY = scrollY + (scrollRatio * scrollableTrackHeight);
    
    // Set the member variables for rectangles
    scrollBarRect = (Rectangle){rect.x + rect.width - 15, scrollY, 10, scrollHeight};
    scrollThumbRect = (Rectangle){rect.x + rect.width - 15, scrollThumbY, 10, scrollThumbHeight};
    
    // Handle mouse wheel for scrolling
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            // Scroll faster when holding shift
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                wheel *= 3.0f;
            }
            
            scrollOffset -= wheel * 20;
            scrollOffset = fmax(0, fmin(scrollOffset, maxScrollOffset));
        }
    }
    
    // Handle scroll bar dragging
    if (CheckCollisionPointRec(GetMousePosition(), scrollThumbRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isDragging = true;
            dragOffset = GetMousePosition().y - scrollThumbY;
        }
    }
    
    if (isDragging) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            float newThumbY = GetMousePosition().y - dragOffset;
            
            // Constrain to scroll track
            newThumbY = fmax(scrollY, fmin(newThumbY, scrollY + scrollHeight - scrollThumbHeight));
            
            // Calculate new scrollOffset based on thumb position
            float dragRatio = (newThumbY - scrollY) / scrollableTrackHeight;
            scrollOffset = dragRatio * maxScrollOffset;
            
            // Double-check bounds
            scrollOffset = fmax(0, fmin(scrollOffset, maxScrollOffset));
        } else {
            isDragging = false;
        }
    }
    
    return true;
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
    
    // Draw info section
    float infoY = codeY + content.size() * lineHeight + 20;
    
    for (int i = 0; i < infoLines.size(); i++, infoY += lineHeight) {
        const auto& info = infoLines[i];

        // Special handling for table markers - PROCESS REGARDLESS OF VISIBILITY
        if (info.label == "__TABLE_START__") {
            // float tableStartY = infoY + lineHeight / 2;
            infoY += lineHeight / 2;
            // Always draw the table, let the table handle its own visibility
            table.Draw(rect, infoY, maxContentHeight, rect.y + 50, customFont);
            
            continue;
        }

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
                
        // Regular info line
        std::string buffer = info.label + ": " + info.value;
        DrawTextEx(customFont, buffer.c_str(), (Vector2){rect.x + 20, infoY}, infoFontSize, 1.0f, textColor);
    }
    
    EndScissorMode();
}

