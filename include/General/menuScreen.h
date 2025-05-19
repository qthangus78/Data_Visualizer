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

        Singly_LinkedList.rect.y = 70;
        Heap.rect.y = 210;
        TrieBox.rect.y = 350;
        AVLTree.rect.y = 490;
        Shortest_path.rect.y = 630 ;

        Singly_LinkedList.rect.width
            = Heap.rect.width 
            = TrieBox.rect.width
            = AVLTree.rect.width 
            = Shortest_path.rect.width 
            = screenWidth / 5 * 2;

            Singly_LinkedList.rect.x = Heap.rect.x = TrieBox.rect.x = AVLTree.rect.x = Shortest_path.rect.x = (screenWidth - Singly_LinkedList.rect.width)/2;

        Singly_LinkedList.rect.height
            = Heap.rect.height 
            = TrieBox.rect.height
            = AVLTree.rect.height 
            = Shortest_path.rect.height 
            = 100;

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
        AVLTree.Draw_TextBox();
        TrieBox.Draw_TextBox();
        Shortest_path.Draw_TextBox();



        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Singly_LinkedList.rect)) {
            currentScreenID = ScreenID::SLLScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Heap.rect)) {
            currentScreenID = ScreenID::HeapScreen;
        }


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, AVLTree.rect)) {
            currentScreenID = ScreenID::AVLScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, TrieBox.rect)) {
            currentScreenID = ScreenID::TrieScreen;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, Shortest_path.rect)) {
            currentScreenID = ScreenID::GraphScreen;
        }
    }
    void Display() {
        ClearBackground(theme.getTheme());

        display_title("Data Structures Menu", ScreenID::StartScreen);
        display_textBoxes();
    }   
}