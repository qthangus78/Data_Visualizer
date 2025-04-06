#pragma once
#include "GlobalVar.h"
using namespace std;

bool maximize(int &x, const int &y) { if (x < y) {x = y; return 1;} return 0; }


struct node {
    int val;
    int height;

    node *par;
    node *child[2]; 
   
    Vector2 Pos = {};

    node(int _val = 0, int _height = 1, node* _par = nullptr, node* left = nullptr, node* right = nullptr) {
        val = _val;
        height = _height;
        par = _par;
        child[0] = left;
        child[1] = right;
    }
};

struct AVL_tree{
    node *root;

    AVL_tree() {
        root = nullptr;
    }


    node* fake_ins(node* pRoot, int val) {
        if (!root) {
            root = new node(val, 1);
            return root;
        }   

        int cid = (val < pRoot -> val) ? 0 : 1;

        if (!pRoot -> child[cid]) {
            pRoot -> child[cid] = new node(val, 1, pRoot);
            maximize(pRoot -> height, 2);
            return pRoot -> child[cid];
        }
        else {
            node* res = fake_ins(pRoot -> child[cid], val);
            maximize(pRoot -> height, pRoot -> child[cid] -> height + 1);
            return res;
        }
    }

    void adjust_Height(node* p) {
        if (!p) return;
        p -> height = 1;
        for (int i = 0; i < 2; ++i) if (p -> child[i]) 
            maximize(p -> height, 1 + p -> child[i] -> height);
    }

    node* single_rotation(node* X, int cid) {
        node* Y = X -> child[cid];
        Y -> par = X -> par;

        if (X -> par) {
            if (X -> par -> child[0] == X)
                X -> par -> child[0] = Y;
            else
                X -> par -> child[1] = Y;
        }
        X -> par = Y;
        
        X -> child[cid] = Y -> child[cid ^ 1];
        if (X -> child[cid]) X -> child[cid] -> par = X;

        Y -> child[cid ^ 1] = X;

        adjust_Height(X);
        adjust_Height(Y);
        adjust_Height(Y -> par);

        return Y;
    }   

    void Insert(int val) {
        node* X = fake_ins(root, val);
        while(X) {
            int leftHeight = 0, rightHeight = 0;
            pair<int, int> imbalance(-1, -1);

            if (X -> child[0]) leftHeight = X -> child[0] -> height;
            if (X -> child[1]) rightHeight = X -> child[1] -> height;
            
            if (leftHeight == rightHeight + 2) imbalance.first = 0;
            else if (rightHeight == leftHeight + 2) imbalance.first = 1;


            if (imbalance.first == -1) {
                X = X -> par;
                continue;
            }

 
            node* Y = X -> child[imbalance.first];
            leftHeight = rightHeight = 0;
            if (Y && Y -> child[0]) leftHeight = Y -> child[0] -> height;
            if (Y -> child[1]) rightHeight = Y -> child[1] -> height;

            if (leftHeight > rightHeight) imbalance.second = 0;
            else imbalance.second = 1;


            if (imbalance.first != imbalance.second) Y = single_rotation(Y, imbalance.second);         
            X = single_rotation(X, imbalance.first);
        
            if (!X -> par) {
                root = X;
                return;
            }
            X = X -> par;
        }
    }

    // void DFS(node* pRoot, vector<node*> &order) {
    //     if (!pRoot) return;
    //     order.push_back(pRoot);
    //     DFS(pRoot -> child[0], order);
    //     DFS(pRoot -> child[1], order);
    // }

    void DFS_inorder(node *pRoot, vector<int> &ans) {
        if (!pRoot) return;
        DFS_inorder(pRoot -> child[0], ans);
        ans.push_back(pRoot -> val);
        DFS_inorder(pRoot -> child[1], ans);
    }
};



void DrawNode(node* p, float x, float y, int cc = 0) {
    if (!p) return;
    DrawCircleV({x, y}, 25, WHITE);
    DrawText(TextFormat("%d" , p -> val) , x - 5 , y - 10, 20, BLACK);
    p -> Pos = {x, y};
    if (p -> par) {
        DrawLineV({x, y} , p -> par -> Pos, BLACK);
    }

    if (p -> child[0]) 
        DrawNode(p -> child[0], x - 100 + cc * 30, y + 100, cc + 1);

    if (p -> child[1])
        DrawNode(p -> child[1], x + 100 - cc * 30, y + 100, cc + 1);
}