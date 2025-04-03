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
	vector<pair<Vector2, Vector2>> Edges;
	vector < pair<Vector2, char>> Nodes;
public:
	TrieNode* root;
	//Trie
	Trie();
	
	void Insert(const string& c);
	
	//Visualizer
	void Visualize(TrieNode* root, float x, float y, float offset, int depth);

	void drawNodeTrie(Vector2 pos, const char& character, float radius);

	void Draw();
};


namespace Trie_Display {
	inline Trie* TrieInstance = nullptr;
	void Display();
}
