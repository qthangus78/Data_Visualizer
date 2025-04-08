#include "Trie_display.h"

//Trie
Trie::Trie() {
	root = new TrieNode();
}

void Trie::Insert(const string& c)
{
	InsertTrie(root, c);
}

//Utility

void Trie::drawNodeTrie(Vector2 pos, const char& character, Color colorNode, float radius)
{
	DrawCircleV(pos, radius, color::nodeNotInMode);
	char text[2] = { character, '\0' };
	Vector2 textSize = MeasureTextEx(customFont, text, 23, 2);
	DrawTextEx(customFont, text, { pos.x - textSize.x / 2, pos.y - textSize.y / 2 }, 22, 2, colorNode);
}

string Trie::handleTypeBox(Rectangle rect)
{
	// Get char pressed (unicode character) on the queue
	int key = GetCharPressed();

	// Check if more characters have been pressed on the same frame
	while (key > 0)
	{
		// NOTE: Only allow keys in range [32..125]
		if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
		{
			textIn += (char)key;
			letterCount++;
		}

		key = GetCharPressed();  // Check next character in the queue
	}

	if (IsKeyPressed(KEY_BACKSPACE) && !textIn.empty())
	{
		textIn.pop_back();
		letterCount--;
	}

	if (IsKeyPressed(KEY_ENTER) )
	{
		string Word = textIn;
		letterCount = 0;
		textIn.clear();
		return Word;
	}

	return "";

	//if (isMouseOnText) frameCounter++;
	//else frameCounter = 0;
}

void Trie::drawTypeBox(Rectangle rect)
{
	DrawRectangleRounded(rect, 0.3f, 30, color::buttonColorHovered);
	drawTextIn(textIn, rect, frameCounter);
}

void Trie::HandleButtonClickTrie()
{
	if (CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		curFunct = FunctionNumber::Input;
	}
	if (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		
	}
	if (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		
	}
	if (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		
	}
}

//Visualizer

int Trie::CalculateSubtreeSize(TrieNode* node) {
	if (node == nullptr) return 0;

	if (node->children.empty()) {
		node->subtreeSize = 1;
		return 1;
	}

	bool isNearlyLeaf = true;
	for (auto& [_, child] : node->children)
	{
		if (!child->children.empty())
		{
			isNearlyLeaf = false;
			break;
		}
	}

	if (isNearlyLeaf)
	{
		int size = node->children.size();
		node->subtreeSize = size;
		return size;
	}

	int size = 0;  
	for (auto& [_, child] : node->children) {
		size += CalculateSubtreeSize(child);  
	}

	node->subtreeSize = size;
	return size;
}

void Trie::MarkNodesEdges(TrieNode* root, float x, float y, float spread, int depth)
{
	//depth for future usage
	if (root == nullptr)
	{
		return;
	}

	if (depth == 0) //pushback only root
		Nodes.push_back({ { { x,y }, '\0'}, BLACK });

	vector<pair<char, TrieNode*>> sortedChildren(root->children.begin(), root->children.end());
	sort(sortedChildren.begin(), sortedChildren.end());

	int childCount = 0;
	int childNumber = root->subtreeSize;
	float buffer = 0.5;

	for (auto& [character, child] : sortedChildren)
	{
		float xNew = x + 80;
		float yNew = y + (childCount + buffer - childNumber / 2.0f) * spread;
		if (child)
		{
			yNew += (child->subtreeSize / 2.0f - buffer) * spread;
		}



		Edges.push_back({ {x,y}, {xNew,yNew} });
		if (child && child->isWord)
		{
			Nodes.push_back({ {{ xNew,yNew }, character }, GREEN });
		}
		else
		{
			Nodes.push_back({ {{ xNew,yNew }, character }, RAYWHITE });

		}

		MarkNodesEdges(child, xNew, yNew, spread, depth + 1); //

		if (child) childCount += child->subtreeSize; 
	}
}

void Trie::Visualize(TrieNode* root)
{
	CalculateSubtreeSize(root);
	MarkNodesEdges(root, screenWidth / 10, screenHeight / 2, spread, 0);
}



void Trie::Handle()
{
	//button
	handleButtonsHover();
	HandleButtonClickTrie();

	switch (curFunct)
	{
	case FunctionNumber::Input:
		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			Insert(word);
			Edges.clear();
			Nodes.clear();
			Visualize(root);
		}
			
		break;
	}

	//Trie
	

}

void Trie::Draw()
{
	//Trie
	for (auto edge : Edges) {
		drawArrow(edge.first, edge.second, color::edgeNotInMode);
	}

	for (auto node : Nodes) {
		drawNodeTrie(node.first.first, node.first.second,node.second, 25);
	}

	//button
	drawButtons();

	switch (curFunct)
	{
	case FunctionNumber::Input:
		drawTypeBox(inputRect);
		break;
	}
}

namespace Trie_Display {
	void Display()
	{
        display_title("Trie", ScreenID::TrieScreen);
		if (TrieInstance)
		{
			TrieInstance->Handle();
			TrieInstance->Draw();
		}
		

        ClearBackground({ 192, 245, 242, 100 });
	}
}

