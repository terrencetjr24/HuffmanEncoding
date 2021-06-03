#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H

typedef struct tNode
{
  struct tNode * rightChild;
  struct tNode * leftChild;
  long int frequency;
  unsigned char ASCII_val;

  int * codeArray;
  int sizeOfCodeArray;

  struct tNode* next;
}TreeNode;


#include <stdio.h>
#include "linkedList_functions.h"
#include "compression.h"



TreeNode* treeCreate(FILE *treeFile, long int * array, int charUsed);
//Note: createTNode is placed in the linked list functions becasue it is primarily used there
void PreOrderPrint(FILE* treeFile, TreeNode* tree);
void codeGen(FILE* codeFile, TreeNode* tree, signed char* path, unsigned char idx);

void usefulCodeGen(TreeNode* tree, unsigned char nextSigDigit, unsigned char binaryRep, encodeType* codingArray);

void freeTree(TreeNode* node);

char* generateTopology(FILE* topology, long int * size);
char* topologyHelper(char* first, long int* size);

#endif
