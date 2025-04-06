#pragma once
#include "GlobalVar.h"
#include "TextBox.h"
#include "button.h"


namespace startMenuScreen {
    void display_textBoxes() {
        TextBox Singly_LinkedList;
        TextBox Heap;
        TextBox AVLTree;
        TextBox TrieBox;
        TextBox Shortest_path;

        Singly_LinkedList.text = "Singly Linked List";
        Heap.text = "Heap";
        AVLTree.text = "AVL Tree";
        Shortest_path.text = "Graphs";
        TrieBox.text = "Trie";

        
        Singly_LinkedList.rect.x = AVLTree.rect.x = TrieBox.rect.x = screenWidth / 20;
        Heap.rect.x = Shortest_path.rect.x = screenWidth / 20 + screenWidth / 2;

        Singly_LinkedList.rect.y = Heap.rect.y = (screenHeight / 7.0f) + (screenHeight / 14.0f);
        AVLTree.rect.y = Shortest_path.rect.y = TrieBox.rect.y = (screenHeight / 7.0f) * (4.0f) + (screenHeight / 14.0f);

        Singly_LinkedList.rect.width
            = Heap.rect.width 
            = TrieBox.rect.width
            = AVLTree.rect.width 
            = Shortest_path.rect.width 
            = screenWidth / 5 * 2;

        Singly_LinkedList.rect.height
            = Heap.rect.height 
            = TrieBox.rect.height
            = AVLTree.rect.height 
            = Shortest_path.rect.height 
            = (screenHeight / 700.0f) * (200.0f);

        Singly_LinkedList.scaleRate
            = Heap.scaleRate
            = AVLTree.scaleRate 
            = Shortest_path.scaleRate 
            = TrieBox.scaleRate
            = 1.2f;

        Singly_LinkedList.rectColor
            = Heap.rectColor
            = AVLTree.rectColor
            = Shortest_path.rectColor
            = TrieBox.rectColor
            = {52, 86, 221, 255}; 
        
        Singly_LinkedList.textColor
            = Heap.textColor
            = AVLTree.textColor
            = Shortest_path.textColor
            = TrieBox.textColor
            = WHITE;


        Singly_LinkedList.Draw_TextBox();
        Heap.Draw_TextBox();
        //AVLTree.Draw_TextBox();
        TrieBox.Draw_TextBox();
        Shortest_path.Draw_TextBox();



        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Singly_LinkedList.rect)) {
            currentScreenID = ScreenID::SLLScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Heap.rect)) {
            currentScreenID = ScreenID::HeapScreen;
        }


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, AVLTree.rect)) {
            currentScreenID = ScreenID::AVLTreeScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Shortest_path.rect)) {
            currentScreenID = ScreenID::GraphScreen;
        }
    }

    void Display() {
        ClearBackground({192, 245, 242, 100});

        display_title("Data Structures Menu", ScreenID::StartScreen);
        display_textBoxes();
    }   
}