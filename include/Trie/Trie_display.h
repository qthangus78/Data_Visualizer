#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "button.h"
#include "Trie.h"
#include "AnnouncementBox.h"

class Trie {
private:


public:
	//announce
	AnnouncementBox announce;
	pair<char*, vector<char*>> InsertCode = {
		"Insert",
		{
			"1.For each character in word:",
			"2.		if character not found: ",
			"3.			create Node",
			"4.Mark end node as word",
		}
	};
	pair<char*, vector<char*>> FindCode = {
		"Find",
		{
			"1.For each character in word:",
			"2.		if character not found: ",
			"3.			return false",
			"4.check if the last character end the word",
		}
	};
	pair<char*, vector<char*>> DeleteCode = {
		"Delete",
		{
			"1.if reach last character",
			"2.		if character ends word",
			"3.			unmarking word",
			"4.		if node is empty and not a word",
			"5.			delete node ",
			"6.if next character in word exists",
			"7.	    traverse by recursion",
			"8.		if node is deleted",
			"9.			erase children of node",
			"10.if node is empty and not a word",
			"11.	delete node",
		}
	};
	pair<char*, vector<char*>> ClearCode = {
		"Clear",
		{
			"1.for each child of node",
			"2.		traverse to child by recursion",
			"3.		remove last node",
		}
	};


	//struct toggle an cap tu linkedlist.h
	struct ToggleSwitch {
		Rectangle bounds;
		bool isStepByStep;
		float toggleX;
		float toggleRadius;
		float labelAlpha;
		float labelAlphaReverse;
		ToggleSwitch();
		void Update(Vector2 mouse, Trie&);
		void Draw();
	}; 
	ToggleSwitch toggle;

	enum FunctionNumber {
		Input = 1,
		FindFunct = 2,
		DeleteFunct = 3,
		ClearFunct = 4,

	};

	//Trie root
	TrieNode* root;
	//function variables
	int letterCount = 0;
	const int MAX_INPUT_CHARS = 15;

	string textIn, word;
	int frameCounter = 0;
	bool isFound = false;

	int curFunct = 0;
	int state = -1;
	

	TrieNode* curNode = root;

	//draw variables
	queue<Step> steps;//step by step
	int curLine = -1;//pseudo code
	string curText ="";

	const float spread = 55/*spacing*/, durationTrie = 5.0f/*Trie animation*/, durationNode = 0.5f /*Node animation*/;
	float elapsedTrie = 0.0f, elapsedNode = 0.0f, progressTrie = 1,progressNode = 1;/*Trie animation*/

	Rectangle inputRect = {};


	//Trie
	Trie();
	
	void Insert(const string& c);
	//void ProcessInsert();

	bool Find(const string& c);

	bool Delete(const string&);

	void Clear();
	
	//Utility
	void drawNodeTrie(Vector2 pos, const char& character, Color colorNode,Color colorText,float radius);
	//void animateTrie(float duration, Vector2 start, Vector2 end);

	string handleTypeBox(Rectangle rect);
	void drawTypeBox(Rectangle rect);

	void HandleButtonClickTrie();
	
	void drawFindResult(bool isFound, const string& key);

	void drawDeleteResult(bool isFound, const string& key);

	void drawClearResult();
	
	//Visualizer---------------------

	//calculate the placement
	int CalculateSubtreeSize(TrieNode* node);
	void MarkNodesEdges(TrieNode* root, float x, float y, float spread, int depth);
	void Visualize(TrieNode* root);

	//calculate position on time
	void calculatePosTrie(TrieNode* root);

	//draw Trie
	void drawTrieNodes(TrieNode* root);
	void drawTrieEdges(TrieNode* root);
	void drawTrie(TrieNode* root);

	//change color
	void changeColorNode(TrieNode*, Color);
	void highlightWord(TrieNode* root,const string& key, Color);
	void resetColorNode(TrieNode* root, Color );

	void processStepbyStep(queue<Step> &steps);

	void StepbyStepBox(const char* title, const vector<char*> content);
	void resetState(pair<char*, vector<char*>> code);

	void Handle();
	void Draw();
};

struct PseudoCodeBox {
	vector<char*> content;
	Rectangle rect = { 0, 0 ,300, 500 };

};

namespace Trie_Display {
	inline Trie* TrieInstance = nullptr;
	void Display();
}
