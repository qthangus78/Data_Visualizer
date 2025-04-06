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

void Trie::drawNodeTrie(Vector2 pos, const char& character, float radius)
{
	DrawCircleV(pos, radius, color::nodeNotInMode);
	char text[2] = { character, '\0' };
	Vector2 textSize = MeasureTextEx(customFont, text, 23, 2);
	DrawTextEx(customFont, text, { pos.x - textSize.x / 2, pos.y - textSize.y / 2 }, 22, 2, BLACK);
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

void Trie::Visualize(TrieNode* root, float x, float y, float offset, int depth)
{
	//depth for future usage
	if (root == nullptr)
	{
		return;
	}

	if (depth == 0) //pushback only root
		Nodes.push_back({ { x,y }, '\0'});

	vector<pair<char, TrieNode*>> sortedChildren(root->children.begin(), root->children.end());
	sort(sortedChildren.begin(), sortedChildren.end());

	int childCount = 1;
	int childNumber = root->children.size();

	for (auto& [character, child] : sortedChildren)
	{
		//float spread = offset / max(1, childNumber);
		float spread = max(offset / max(1, childNumber), 30.0f);
		float newOffset = offset * 0.75f; //
		float xNew = x + 80;
		float yNew = y + - offset + childCount * spread;

		Edges.push_back({ {x,y}, {xNew,yNew} });
		Nodes.push_back({ { xNew,yNew }, character });

		Visualize(child, xNew, yNew, newOffset, depth + 1); //

		childCount++;
	}
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
			Visualize(root, screenWidth / 10, screenHeight / 2, 150, 0);
		}
			
		break;
	}

	//Trie
	

}

void Trie::Draw()
{
	//button
	drawButtons();

	switch (curFunct)
	{
	case FunctionNumber::Input:
		drawTypeBox(inputRect);
		break;
	}



	//Trie
	for (auto edge : Edges) {
		drawArrow(edge.first, edge.second, color::edgeNotInMode);
	}

	for (auto node : Nodes) {
		drawNodeTrie(node.first, node.second, 33);
	}
}

namespace Trie_Display {
	void Display()
	{
        display_title("Trie", 1);
		if (TrieInstance)
		{
			TrieInstance->Handle();
			TrieInstance->Draw();
		}
		

        ClearBackground({ 192, 245, 242, 100 });
	}
}

