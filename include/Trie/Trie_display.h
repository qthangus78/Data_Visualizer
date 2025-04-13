#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "button.h"
#include "Trie.h"

//Interface in LinkList.h


//--------------------------------
//			Trie
//-------------------------------

//class Trie {
//private:
//	IState* mCurrent;
//	IState* mIdle;
//	IState* mInsert;
//	IState* mFind;
//	IState* mRemove;
//	IState* mClear;
//	TrieNode* root;
//
//public:
//
//
//
//};
//
//---------------------------
//		Các State
//---------------------------
//
//class InsertState : public IState {
//
//};
//
//class FindState : public IState {
//
//};
//
//class RemoveState : public IState {
//	
//};
//
//class ClearState : public IState {
//
//};

class Trie {
private:

public:
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

	//draw variables
	queue<Step> steps;
	int curLine = -1;
	string curText ="";

	const float spread = 55, duration = 5.0f;
	bool isAnimated = false;
	float elapsed = 10.0f, progress = 1;

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
	void FindDisplay(TrieNode* root,const string& key, Color);

	void drawDeleteResult(bool isFound, const string& key);

	void drawClearResult();
	

	//Visualizer
	int CalculateSubtreeSize(TrieNode* node);
	void MarkNodesEdges(TrieNode* root, float x, float y, float spread, int depth);
	void Visualize(TrieNode* root);

	void drawTrieNodes(TrieNode* root, float &lerp);
	void drawTrieEdges(TrieNode* root, float& lerp);
	void drawTrie(TrieNode* root, float& lerp);

	void resetColorNode(TrieNode* root, Color );

	void Handle();

	void Draw();
};


namespace Trie_Display {
	inline Trie* TrieInstance = nullptr;
	void Display();
}
