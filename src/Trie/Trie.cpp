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
    color = WHITE;
    posCur = posPrev = { 0,0 };
    //for (int i = 0; i < 26; i++) {
    //    children['a'+ i] = nullptr;
    //}
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

bool FindTrie(TrieNode*& root, const string& key)
{
    if (root == nullptr) {
        return false;
    }

    TrieNode* cur = root;
    
    for (char character : key)
    {
        if (cur->children.find(character) == cur->children.end())
        {
            return false;
        }

        cur = cur->children[character];
    }

    return cur->isWord;
}

bool isEmpty(TrieNode* root)
{
    for (int i = 0; i < 26; i++)
        if (root->children[i])
            return false;
    return true;
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

        if (isEmpty(root))
        {
            delete root;
            root = nullptr;
        }

        return root;
    }

    char character = key[depth];
    root->children[character] = DeleteTrie(root->children[character], key, depth + 1);

    if (isEmpty(root) && root->isWord == false) {
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


//=======================================
//
//              Draw
//
//======================================









