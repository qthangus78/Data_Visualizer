#include "Trie_display.h"

//toggle--------------------------
Trie::ToggleSwitch::ToggleSwitch()
{
	bounds = { 50,20,140,40 };
	isStepByStep = true;
	toggleRadius = 40 * 0.8f;
	toggleX = bounds.x + 5;
	labelAlpha = 1.0f;
	labelAlphaReverse = 0.0f;
}
void Trie::ToggleSwitch::Update(Vector2 mouse, Trie& trie)
{
	if (CheckCollisionPointRec(mouse, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		isStepByStep = !isStepByStep;
		//trie.progressNode = trie.progressTrie = 1;
		//trie.elapsedTrie = trie.durationTrie; trie.elapsedNode = trie.durationTrie;
	}
	float targetX = isStepByStep ? bounds.x + 5 : bounds.x + bounds.width - toggleRadius - 5;
	toggleX += (targetX - toggleX) * 0.15f;
	float targetAlpha = isStepByStep ? 1.0f : 0.0f;
	float targetAlphaReverse = isStepByStep ? 0.0f : 1.0f;
	labelAlpha += (targetAlpha - labelAlpha) * 0.1f;
	labelAlphaReverse += (targetAlphaReverse - labelAlphaReverse) * 0.1f;
}
void Trie::ToggleSwitch::Draw() {
	DrawRectangleRoundedLinesEx(bounds, 0.5f, 20, 4, DARKGRAY);
	DrawCircleV({ toggleX + toggleRadius / 2, bounds.y + bounds.height / 2 }, toggleRadius / 2 + 3.0f, DARKBLUE);
	DrawCircleV({ toggleX + toggleRadius / 2, bounds.y + bounds.height / 2 }, toggleRadius / 2, WHITE);
	DrawTextEx(SSLFont, "Step by step", { bounds.x + 40, bounds.y + 10 }, 17, 1, Fade(BLACK, labelAlpha));
	DrawTextEx(SSLFont, "Run at once", { bounds.x + bounds.width - 135, bounds.y + 15 }, 17, 1, Fade(BLACK, labelAlphaReverse));
}


//Trie-------------------------
Trie::Trie() {
	root = new TrieNode();
	root->isNewNode = false;
	announce = AnnouncementBox({1050,350,300,350}, "");
}

void Trie::Insert(const string& c)
{
	InsertTrie(root, c, steps);
}

bool Trie::Find(const string& c)
{
	return FindTrie(root, c, steps);
}

bool Trie::Delete(const string& c)
{
	bool isExist = Find(c);
	if (isExist)
	{
		DeleteTrieOrigin(root, c, 0, steps, toggle.isStepByStep);
		return true;
	}else
	{
		return false;
	}
}

void Trie::Clear()
{
	ClearTrie(root, 0 ,steps, toggle.isStepByStep);
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
		DrawTextEx(SSLFont, "Empty Trie", { inputRect.x - 3, inputRect.y - 20 }, 20, 2, RED);
	}
	else {
		if (frameCounter < 30 /* && !FindProcess*/) {
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
			if (toggle.isStepByStep && child->isNewNode)
			{
				//child->posPrev = root->posCur;
				child->posPrev = {xNew,yNew};
				//child->isNewNode = false;
			}
			else child->posPrev = child->posCur;
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

void Trie::calculatePosTrie(TrieNode* root)
{
	
	//root->pos = lerp(root->posPrev, root->posCur, progressTrie);
	//if (toggle.isStepByStep && root->isNewNode)
	//{
	//	root->pos = lerp(root->posPrev, root->posCur, progressNode);
	//	//root->pos = root->posPrev;
	//}

	root->pos = lerp(root->posPrev, root->posCur, progressTrie);
	
	for (auto& child : root->children)
	{
		calculatePosTrie(child.second);
	}
}

void Trie::drawTrieNodes(TrieNode* root)
{
	if (!root->isNewNode)
	drawNodeTrie(root->pos, root->character, root->colorNode, root->colorText, 25);

	for (auto& child : root->children)
	{
		drawTrieNodes(child.second);
	}
}
void Trie::drawTrieEdges(TrieNode* root)
{
	for (auto& child : root->children)
	{
		if (!child.second->isNewNode)
		drawArrow(root->pos, child.second->pos,color::edgeNotInMode);
		drawTrieEdges(child.second);
	}
}
void Trie::drawTrie(TrieNode* root)
{
	drawTrieEdges(root);
	drawTrieNodes(root);
}

void Trie::highlightWord(TrieNode* root, const string& key, Color colorN)
{
	for (char character : key)
	{
		changeColorNode(root->children[character], colorN);
		root = root->children[character];
	}
}
void Trie::changeColorNode(TrieNode* node, Color colorN)
{
	node->colorNode = colorN;
}
void Trie::resetColorNode(TrieNode* root, Color colorN)
{
	for (auto& child : root->children)
	{
		child.second->colorNode = colorN;
		resetColorNode(child.second, colorN);
	}
}

void Trie::processStepbyStep(queue<Step>& steps)
{
	if (!steps.empty())
	{

		if (!toggle.isStepByStep)
		{
			Step curStep = steps.front();
			steps.pop();
			curStep.node->isNewNode = false;
			return;
		}
		else if (progressNode >= 1)
		{
			Step curStep = steps.front();
			steps.pop();
			TrieNode* curNode = curStep.node;
			switch (curStep.function)
			{
			case functionStep::moving:
				announce.SetHighlightLines(curStep.line-1,curStep.line-1);
				changeColorNode(root, color::nodeNotInMode);
				resetColorNode(root, color::nodeNotInMode);
				changeColorNode(curNode, DARKBLUE);
				break;

			case functionStep::createnode:
				announce.SetHighlightLines(curStep.line-1, curStep.line-1+1);
				changeColorNode(root, color::nodeNotInMode);
				resetColorNode(root, color::nodeNotInMode);
				curStep.node->isNewNode = false;
				changeColorNode(curNode, GREEN);
				break;

			case functionStep::markword:
				announce.SetHighlightLines(curStep.line-1, curStep.line-1+1);
				changeColorNode(root, color::nodeNotInMode);
				resetColorNode(root, color::nodeNotInMode);
				changeColorNode(curNode, YELLOW);
				break;

			case functionStep::invalid:
				announce.SetHighlightLines(curStep.line-1, curStep.line-1);
				changeColorNode(root, color::nodeNotInMode);
				resetColorNode(root, color::nodeNotInMode);
				changeColorNode(curNode, RED);
				break;

			case functionStep::erase:
				announce.SetHighlightLines(curStep.line-1, curStep.line-1+1);
				changeColorNode(root, color::nodeNotInMode);
				resetColorNode(root, color::nodeNotInMode);

				
				delete curNode->children[curStep.character];
				curNode->children[curStep.character] = nullptr;

				curNode->children.erase(curStep.character);

				if (steps.empty())
				{
					Visualize(root);
					elapsedTrie = 0;
					progressTrie = 0; //0 de ko bi giut animation
				}

				break;
			}

			elapsedNode = 0;
			progressNode = 0;


		}

	}

	//bool hasCurStep = false;
	//if (IsKeyPressed(KEY_SPACE)) {
	//	if (!steps.empty()) {
	//		curStep = steps.front();
	//		steps.pop();
	//		hasCurStep = true;
	//	}
	//	else {
	//		hasCurStep = false;
	//	}
	//}
	// 
	//if (hasCurStep) {
	//	DrawStep(currentStep); 
	//}

}

void Trie::StepbyStepBox(const char* title, const vector<char*> content)
{
	Rectangle rect = { 1050 , 350 ,300, 350 };
	announce = AnnouncementBox(rect, title);
	announce.SetContent(content);
}
void Trie::resetState(pair<char*, vector<char*>> code)
{
	if (state != curFunct)
	{
		resetColorNode(root, color::nodeNotInMode);
		progressTrie = progressNode = 1; //reset animation khi doi function dot ngot
		if(toggle.isStepByStep)
		StepbyStepBox(code.first, code.second);
	}
	state = curFunct;
}

void Trie::Handle()
{
	//Trie
	if (progressNode < 1)
	{
		progressNode = elapsedNode / durationNode;
		elapsedNode += GetFrameTime();
	}

	if (progressTrie < 1)
	{
		progressTrie = elapsedTrie / durationTrie;
		elapsedTrie += GetFrameTime();
	}

	//button
	handleButtonsHover();
	HandleButtonClickTrie();
	toggle.Update(mouse, *this);

	switch (curFunct)
	{
	case FunctionNumber::Input:
		resetState(InsertCode);

		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			Insert(word);
			Visualize(root);
			elapsedTrie = elapsedNode = 0;
			progressTrie = progressNode = 0; //0 de ko bi giut animation
		}

		break;

	case FunctionNumber::FindFunct:
		resetState(FindCode);

		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			isFound = Find(word);
			if (isFound)
			{
				resetColorNode(root, color::nodeNotInMode);
				highlightWord(root, word, { 170, 180, 60, 255 });
			}
		}

		break;

	case FunctionNumber::DeleteFunct:
		resetState(DeleteCode);

		word = handleTypeBox(inputRect);
		if (!word.empty())
		{
			isFound = Delete(word);
			if (isFound)
			{
				Visualize(root);
				elapsedTrie = elapsedNode= 0;
				progressTrie =progressTrie= 0; //0 de ko bi giut animation
			}
		}

		break;

	case FunctionNumber::ClearFunct:
		resetState(ClearCode);
		if (!root->children.empty() && steps.empty())
		{
			Clear();
			Visualize(root);
		}
		break;
	}

	calculatePosTrie(root);

	processStepbyStep(steps);


}

void Trie::Draw()
{
	//stupid
	if (toggle.isStepByStep)
	{
		announce.DrawforTrie();
	}
	//trie
	drawTrie(root);

	//button

	drawButtons();
	toggle.Draw();

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
		inputRect = { buttonVar::buttonClear.rect.x + 130,buttonVar::buttonClear.rect.y,130, static_cast<float>(button::sizeH) };
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

