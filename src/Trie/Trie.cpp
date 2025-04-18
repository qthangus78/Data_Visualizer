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
    character  = '\0';
    colorText = WHITE;
    colorNode = color::nodeNotInMode;
    pos = posCur = posPrev = { 0,0 };
    isNewNode = true;
    isDeleted = false;
}

void InsertTrie(TrieNode*& root, const string& key, queue<Step> &steps)
{
    if (root == nullptr)
    {
        root = new TrieNode();
        steps.push({ "create root",1,root,functionStep::createnode , '\0'});
    }

    TrieNode* cur = root;

    for (char character : key) //đi qua từng chữ cái
    {
        //nếu chữ cái không tồn tại thì tạo Node mới
        steps.push({ "Processing character " + string(1,character), 1 , cur,functionStep::moving , '\0' });

        if (cur->children.count(character) == 0 )
        {
            cur->children[character] = new TrieNode();
            cur->children[character]->posPrev = cur->posCur;
            cur->children[character]->posCur = cur->posCur;
            steps.push({"Character " + string(1,character) + " not found. Create new node",2, cur->children[character], functionStep::createnode, '\0' });
        }

        cur = cur->children[character];
    }

    cur->isWord = true;
    steps.push({"Marking current node as end of word.",4, cur, functionStep::markword, '\0' });
}

bool FindTrie(TrieNode*& root, const string& key, queue<Step> &steps)
{
    if (root == nullptr or root->children.empty()) {
        steps.push({ "no trie",1,root, functionStep::invalid, '\0' });
        return false;
    }

    TrieNode* cur = root;
    
    for (char character : key)
    {
        steps.push({ "Processing character " + string(1,character), 1 , cur,functionStep::moving, '\0' });
        if (cur->children.find(character) == cur->children.end())
        {
            return false;
            steps.push({ "Character " + string(1,character) + " not found. return false",2, cur, functionStep::invalid , '\0' });
        }

        cur = cur->children[character];
    }


    if (cur->isWord)
    {
        steps.push({ "check if the last character end the word",4, cur, functionStep::markword, '\0' });
        return true;
    }
    else
    {
        steps.push({ "check if the last character end the word",4, cur, functionStep::invalid, '\0' });
        return false;
    }
}

void deleteNode(TrieNode* root, bool isStepbyStep)
{
    root->isDeleted = true;
    if (!isStepbyStep)
    {
        delete root;
        root = nullptr;
    }
}

bool isEmpty(TrieNode* root)
{
    if (root->children.empty()) return true;

    for (auto& [_, child] : root->children)
    {
        if (!child->isDeleted) return false;
    }

    return true;
}

TrieNode* DeleteTrieOrigin(TrieNode* root, const string& key, int depth, queue<Step>& steps, bool isStepbyStep)
{
    //check if trie exists (trivial since root always exists)
    if (!root)
    {
        steps.push({ "no trie",1, nullptr , -1 });
        return nullptr;
    }

    //if reach end of key
    if (depth == key.size())
    {
        //unmark if word
        if (root->isWord)
        {
            steps.push({ "Reached end of word, unmarking isWord", 2, root, functionStep::markword, '\0' });
            root->isWord = false;
        }

        //no children not word -> delete
        if (isEmpty(root) && depth != 0)
        {
            steps.push({ "node is empty and not a word, delete except root", 4, root, functionStep::invalid , '\0' });
            deleteNode(root, isStepbyStep);
        }

        //return updated node
        return root;
    }

    //check if have that character
    char character = key[depth];
    if (root->children.count(character))
    {
        steps.push({ "Traversing to child '" + string(1, character) + "'", 7, root, functionStep::moving , '\0' });

        //do that for the next character aka recursion
        root->children[character] = DeleteTrieOrigin(root->children[character], key, depth + 1, steps, isStepbyStep);

        // If child was deleted, remove the pointer from the map
        if (!root->children[character] or root->children[character]->isDeleted)
        {
            steps.push({ "Child '" + string(1, character) + "' erase", 8, root, functionStep::erase, character});
            if(!isStepbyStep)
            root->children.erase(character);
        }
    }

    //is empty not word, delete it (except root)
    if (isEmpty(root) && root->isWord == false && depth != 0) {
        steps.push({ "node '" + string(1, character) + " 'is empty and not a word, delete except root", 12, root, functionStep::invalid , '\0' });
        deleteNode(root, isStepbyStep);
    }

    //return updated node
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

void ClearTrie(TrieNode*& root, int depth, queue<Step>& steps, bool isStepbyStep)
{
    if (!root)
    {
        steps.push({ "no trie",1,root, functionStep::invalid , '\0' });
        return;
    }
    steps.push({ "traverse each node", 2, root, functionStep::moving, '\0' });

    for (auto& [character, child] : root->children)
    {
        ClearTrie(child, depth +1, steps, isStepbyStep);

        steps.push({ "reach end of trie, remove", 3, child, functionStep::invalid, '\0' });
        steps.push({ "reach end of trie, remove", 3, root, functionStep::erase, character });

        if(depth !=0)
        deleteNode(root, isStepbyStep);
    }

    
}


//=======================================
//
//              Draw
//
//======================================









