#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

// Definition of a Node
struct Node {
    int data;
    struct Node* next;
};

// Function declarations
struct Node* createNode(int value);
void appendNode(struct Node** head, int value);
void displayList(struct Node* head);
void deleteList(struct Node** head);

#endif // LINKED_LIST_H
