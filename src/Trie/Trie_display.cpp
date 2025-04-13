#include "Trie_display.h"

//Trie-------------------------
Trie::Trie() {
	root = new TrieNode();
}

void Trie::Insert(const string& c)
{
	InsertTrie(root, c, steps);
}
//void Trie::ProcessInsert()
//{
//	static bool waitForNext = true;
//
//	// Wait for space to go to the next step
//	if (!steps.empty() && IsKeyPressed(KEY_SPACE)) {
//		waitForNext = false;
//	}
//
//	if (!steps.empty() && !waitForNext)
//	{
//		Step step = steps.front();
//		steps.pop();
//
//		// Update the current line to highlight
//		curLine = step.line;
//		curText = step.description;
//
//		// Visual update
//		if (step.node != nullptr)
//		{
//			if (step.isNewNode)
//			{
//				step.node->color = BLUE;
//				step.node->animationProgress = 0.0f; // Let Draw handle animation
//			}
//			else
//			{
//				step.node->color = YELLOW;
//				step.node->animationProgress = 1.0f;
//			}
//		}
//
//		waitForNext = true;
//	}
//}

bool Trie::Find(const string& c)
{
	return FindTrie(root, c, steps);
}

bool Trie::Delete(const string& c)
{
	bool isExist = Find(c);
	if (isExist)
	{
		DeleteTrie(root, c, 0);
		return true;
	}else
	{
		return false;
	}
}

void Trie::Clear()
{
	ClearTrie(root);
}

//Utility------------------------
//trie small draw
void Trie::drawNodeTrie(Vector2 pos, const char& character, Color colorNode, Color colorText, float radius)
{
	DrawCircleV(pos, radius, colorNode);
	char text[2] = { character, '\0' };
	Vector2 textSize = MeasureTextEx(customFont, text, 23, 2);
	DrawTextEx(customFont, text, { pos.x - textSize.x / 2, pos.y - textSize.y / 2 }, 22, 2, colorText);
}

//button draw
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
	DrawRectangleRounded(rect, 0.3f, 30, WHITE);
	drawTextIn(textIn, rect, frameCounter);
}

//buttonhandle
void Trie::HandleButtonClickTrie()
{
	if (CheckCollisionPointRec(mouse, buttonVar::buttonIns.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		curFunct = FunctionNumber::Input;
	}
	if (CheckCollisionPointRec(mouse, buttonVar::buttonDel.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		curFunct = FunctionNumber::DeleteFunct;
	}
	if (CheckCollisionPointRec(mouse, buttonVar::buttonF.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		curFunct = FunctionNumber::FindFunct;

	}
	if (CheckCollisionPointRec(mouse, buttonVar::buttonClear.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		curFunct = FunctionNumber::ClearFunct;
	}
}

//function draw
void Trie::FindDisplay(TrieNode* root,const string& key, Color colorN)
{
	for (char character : key)
	{
		root->children[character]->colorNode = colorN;
		root = root->children[character];
	}
}
void Trie::drawFindResult(bool isFound, const string&key ) {
	//drawTextCode(curline, curlinetmp);
	
	if (root->children.empty() && frameCounter < 30) {
		DrawTextEx(SSLFont, "Empty List", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
	}
	else {
		//if (FindProcess) findAnimation(currentAnimatingNode); else
		if (root && isFound && frameCounter < 30 /* && !FindProcess*/) {
			DrawTextEx(SSLFont, "Value Found", { inputRect.x - 3, inputRect.y - 20 }, 22, 2, RED);
		}else 
		{
			if (frameCounter < 30 /* && FindProcess */) {
				DrawTextEx(SSLFont, "Value not Found", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
			}
		}
		//if (!FindProcess) drawLinkedList(mSSL->getRoot(), startLinkedListPos, mSSL);
		//if (shadedPos.node && shadedPos.pos.x != 0 && shadedPos.pos.y != 0) {
		//	std::string str = std::to_string(shadedPos.node->data);
		//	drawNode(shadedPos.pos, str, NODE_SIZE, color::nodeRendered);
		//	drawTextDown("pointer", 20, shadedPos.pos);
		//	if (frameCounter < 30 /* && FindProcess */ ) {
		//		DrawTextEx(SSLFont, "Value Found", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
		//	}
		//}






	}
}

void Trie::drawDeleteResult(bool isFound, const string& key) {

	if (root->children.empty() && frameCounter < 30) {
		DrawTextEx(SSLFont, "Empty List", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
	}
	else {
		//if (FindProcess) findAnimation(currentAnimatingNode); else
		if (root && isFound && frameCounter < 30 /* && !FindProcess*/) {
			DrawTextEx(SSLFont, "Value Deleted", { inputRect.x - 3, inputRect.y - 20 }, 22, 2, RED);
		}
		else
		{
			if (frameCounter < 30 /* && FindProcess */) {
				DrawTextEx(SSLFont, "Value not Found", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
			}
		}
	}
}

void Trie::drawClearResult()
{
	if (root->children.empty() && frameCounter < 30) {
		DrawTextEx(SSLFont, "Empty List", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
	}
	else {
		if (root && frameCounter < 30 /* && !FindProcess*/) {
			DrawTextEx(SSLFont, "Cleared", { inputRect.x - 3, inputRect.y - 20 }, 22, 2, RED);
		}
	}
}
//Visualizer-----------------

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

	if (depth == 0) //only root is '\0'
	{
		root->posPrev = { x,y };
		root->character = '\0';
	}

	root->posCur = { x,y };

	vector<pair<char, TrieNode*>> sortedChildren(root->children.begin(), root->children.end());
	std::sort(sortedChildren.begin(), sortedChildren.end());

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

		if (child)
		{
			if (child->isWord) child->colorText = GREEN;
			else
			{
				child->colorText = RAYWHITE;
			}

			child->character = character;
			child->posPrev = child->posCur;
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

void Trie::drawTrieNodes(TrieNode* root, float& progress)
{
	Vector2 pos = lerp(root->posPrev,root->posCur,progress);
	drawNodeTrie(pos, root->character, root->colorNode, root->colorText, 25);

	for (auto& child : root->children)
	{
		drawTrieNodes(child.second, progress);
	}
}
void Trie::drawTrieEdges(TrieNode* root, float& progress)
{
	for (auto& child : root->children)
	{
		Vector2 posHead = lerp(root->posPrev, root->posCur,  progress);
		Vector2 posTail = lerp(child.second->posPrev, child.second->posCur, progress);
		drawArrow(posTail, posHead,color::edgeNotInMode);
		drawTrieEdges(child.second, progress);
	}
}
void Trie::drawTrie(TrieNode* root, float& progress)
{
	drawTrieEdges(root, progress);
	drawTrieNodes(root, progress);
}

void Trie::resetColorNode(TrieNode* root, Color colorN)
{
	for (auto& child : root->children)
	{
		child.second->colorNode = colorN;
		resetColorNode(child.second, colorN);
	}
}

//void Trie::animateTrie(float duration, Vector2 start, Vector2 end) {
//
//	float elapsed = 0.0f;
//	while (elapsed < duration)
//	{
//		BeginDrawing();
//		ClearBackground({ 192, 245, 242, 100 });
//
//		float progress = elapsed / duration;
//		drawTrie(root, progress);
//
//		EndDrawing();
//		elapsed += GetFrameTime();
//	}
//}

void Trie::Handle()
{
	//button
	handleButtonsHover();
	HandleButtonClickTrie();

	switch (curFunct)
	{
	case FunctionNumber::Input:
		if (state != curFunct) elapsed = duration; //reset elapsed aka animation khi doi function dot ngot
		state = curFunct;
		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			Insert(word);
			Visualize(root);
			elapsed = 0;
		}
			
		break;

	case FunctionNumber::FindFunct:
		if (state != curFunct) elapsed = duration; //reset elapsed aka animation khi doi function dot ngot
		state = curFunct;
		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			isFound = Find(word);
			if (isFound)
			{
				resetColorNode(root, color::nodeNotInMode);
				FindDisplay(root, word, { 170, 180, 60, 255 });
			}
		}

		break;

	case FunctionNumber::DeleteFunct:
		if (state != curFunct) elapsed = duration; //reset elapsed aka animation khi doi function dot ngot
		state = curFunct;
		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			isFound = Delete(word);
			if (isFound)
			{
				Visualize(root);
				elapsed = 0;
			}
		}

		break;

	case FunctionNumber::ClearFunct:
		if (state != curFunct) elapsed = duration; //reset elapsed aka animation khi doi function dot ngot
		state = curFunct;
		Clear();
		Visualize(root);
		break;
	}

	//Trie
	if (elapsed <= duration)
	{
		progress = elapsed / duration;
		elapsed += GetFrameTime();
	}

	

}

void Trie::Draw()
{
	//Trie
	/*if (!isAnimated)
	{
		drawTrie(root, );
	}*/


	drawTrie(root, progress);

	//button
	drawButtons();

	switch (curFunct)
	{
	case FunctionNumber::Input:
		inputRect = { buttonVar::buttonIns.rect.x + 130,buttonVar::buttonIns.rect.y,130, static_cast<float>(button::sizeH) };
		drawTypeBox(inputRect);
		break;

	case FunctionNumber::FindFunct:
		inputRect = { buttonVar::buttonF.rect.x + 130,buttonVar::buttonF.rect.y,130, (float)button::sizeH };
		drawTypeBox(inputRect);
		drawFindResult(isFound,word);
		break;

	case FunctionNumber::DeleteFunct:
		inputRect = { buttonVar::buttonDel.rect.x + 130,buttonVar::buttonDel.rect.y,130, static_cast<float>(button::sizeH) };
		drawTypeBox(inputRect);
		drawDeleteResult(isFound, word);
		break;

	case FunctionNumber::ClearFunct:
		drawClearResult();
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

