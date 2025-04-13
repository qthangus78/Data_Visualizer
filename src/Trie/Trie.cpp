#include "Trie.h"

using namespace std;

//==========================================
// 
//            Function chinh
// 
//=========================================

TrieNode::TrieNode() {
    isWord = false;
    subtreeSize = 1;
    character = '\0';
    colorText = WHITE;
    colorNode = color::nodeNotInMode;
    posCur = posPrev = { 0,0 };
}

void InsertTrie(TrieNode*& root, const string& key, queue<Step> steps)
{
    if (root == nullptr)
    {
        root = new TrieNode();
        steps.push({ "create root",1,root,true });
    }

    TrieNode* cur = root;

    for (char character : key) //đi qua từng chữ cái
    {
        //nếu chữ cái không tồn tại thì tạo Node mới
        steps.push({ "Processing character " + string(1,character), 2 , cur,false });

        if (cur->children.count(character) == 0 )
        {
            cur->children[character] = new TrieNode();
            cur->children[character]->posPrev = cur->posCur;
            cur->children[character]->posCur = cur->posCur;
            steps.push({"Character " + string(1,character) + " not found. Create new node",3, cur->children[character], true});
        }
        else
        {
            steps.push({ "Character " + string(1,character) + " found. Move next child",4, cur->children[character], false});
        }

        cur = cur->children[character];
    }

    cur->isWord = true;
    steps.push({"Marking current node as end of word.",5, cur,false});
}

bool FindTrie(TrieNode*& root, const string& key, queue<Step> steps)
{
    if (root == nullptr) {
        return false;
        steps.push({ "no trie",1,root,false });
    }

    TrieNode* cur = root;
    
    for (char character : key)
    {
        steps.push({ "Processing character " + string(1,character), 2 , cur,false });
        if (cur->children.find(character) == cur->children.end())
        {
            return false;
            steps.push({ "Character " + string(1,character) + " not found. return false",3, cur->children[character], false });
        }
        else
        {
            steps.push({ "Character " + string(1,character) + " found. Move next character",4, cur->children[character], false });
        }

        cur = cur->children[character];
    }

    steps.push({ "check if the last character end the word",5, cur,false });
    return cur->isWord;
}

bool isEmpty(TrieNode* root)
{
    return root->children.empty();
}

TrieNode* DeleteTrie(TrieNode* root, const string& key, int depth)
{
    if (!root)
        return NULL;

    if (depth == key.size())
    {
        if (root->isWord)
        {
            root->isWord = false;
        }

        if (isEmpty(root) && depth !=0)
        {
            delete root;
            root = nullptr;
        }

        return root;
    }

    char character = key[depth];
    if (root->children.count(character))
    {
        root->children[character] = DeleteTrie(root->children[character], key, depth + 1);

        // If child was deleted, remove the pointer from the map
        if (!root->children[character])
        {
            root->children.erase(character);
        }
    }
 
    if (isEmpty(root) && root->isWord == false && depth != 0) {
        delete (root);
        root = NULL;
    }

    return root;
}

void InputFileTrie(const string& file_name, TrieNode*& root, queue<Step> steps)
{
    ifstream file(file_name);

    if (!file.is_open())
    {
        cout << "hell nah" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line))
    {
        InsertTrie(root, line, steps);
    }
}

void ClearTrie(TrieNode*& root, int depth)
{
    if (!root or root->children.empty()) return;


    for (auto& [_, child] : root->children)
    {
        ClearTrie(child, depth +1);
    }

    root->children.clear();
    if (depth != 0)
    {
        delete root;
        root = nullptr;
    }
}


//=======================================
//
//              Draw
//
//======================================









