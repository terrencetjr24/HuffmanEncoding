#ifndef LINKEDLIST_FUNCTIONS_H
#define LINKEDLIST_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "tree_functions.h"

// Included in here because this is where the nodes will be created originally and sorted
TreeNode *createTNode(unsigned char ASCII_val, long int freq);

void LinkedListCreate(TreeNode **head, int charactersLeft, long int* array, unsigned char lastValue);

void linkedListSort(TreeNode** head);
void SplitList(TreeNode** head, TreeNode** leftList, TreeNode** rightList);
TreeNode* WeaveLists(TreeNode** upper, TreeNode** lower);

void treeFromList(TreeNode** node);

#endif
