#include "Trie_display.h"

//Trie
Trie::Trie() {
	root = new TrieNode();
}

void Trie::Insert(const string& c)
{
	InsertTrie(root, c);
}

//Visualizer
void Trie::Visualize(TrieNode* root, float x, float y, float offset, int depth)
{
	//depth for future usage
	if (root == nullptr)
	{
		return;
	}

	Nodes.push_back({ { x,y }, '\0'});

	int childCount = 0; //use for offset
	for (auto& [character, child] : root->children)
	{
		float xNew = x + 80;
		float yNew = y;//need changing

		Edges.push_back({ {x,y}, {xNew,yNew} });
		Nodes.push_back({ { xNew,yNew }, character });

		Visualize(root->children[character], xNew, yNew, offset, depth + 1);

		childCount++;
	}
}

void Trie::drawNodeTrie(Vector2 pos, const char& character, float radius)
{
	DrawCircleV(pos, radius, color::nodeNotInMode);
	char text[2] = { character, '\0' };
	Vector2 textSize = MeasureTextEx(customFont, text, 23, 2);
	DrawTextEx(customFont, text, { pos.x - textSize.x / 2, pos.y - textSize.y / 2 }, 22, 2, BLACK);
}

void Trie::Draw()
{
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
			TrieInstance->Draw();
		}
		

        ClearBackground({ 192, 245, 242, 100 });
	}
}

