#include "../header/linkedList_functions.h"


void LinkedListCreate(TreeNode **head, int charactersLeft, long int* array, unsigned char lastValue){
  // If there are no more nodes to create, return
  if(charactersLeft < 1){
      head = NULL;
      return;
    }

  // Find the next value in the freqArray to create a node for
  unsigned char idx = lastValue+1;
  while(array[idx] == 0)
    idx++;

  // Assign the head to the created node
  *head = createTNode(idx, array[idx]);

  // Create the next node
  LinkedListCreate(&(*head)->next, charactersLeft - 1, array, idx);
}

TreeNode *createTNode(unsigned char ASCII_val, long int freq){
  TreeNode* node = NULL;

  node = malloc(sizeof(TreeNode));
  if(node == NULL){
    printf("Error in allocating memory for a node.\n");
  }

  node->frequency = freq;
  node->ASCII_val = ASCII_val;
  node->rightChild = NULL;
  node->leftChild = NULL;

  node->next = NULL;

  return node;
}

void linkedListSort(TreeNode** head){
  TreeNode* split1 = NULL;
  TreeNode* split2 = NULL;

  if ( ((*head) == NULL) || ( (*head)->next == NULL ) )
    return;

  SplitList(head, &split1, &split2);

  linkedListSort(&split1);
  linkedListSort(&split2);

  *head = WeaveLists(&split1, &split2);
}

void SplitList(TreeNode** head, TreeNode** leftList, TreeNode** rightList){
  TreeNode* OneStep;
  TreeNode* TwoStep;

  OneStep = *head;
  TwoStep = (*head)->next;

  //Create two nodes that increment through the list at different speeds
  //When the two stp one reaches the end it will come out of loop
  //If an odd number of nodes the left list will have more nodes
  //EX. if 7 nodes, when the "two step" reaches NULL the "one step" will be pointing to the 4th node,
  //then the second list will start at the 5th node, and the next of the forth node will be changed to NULL to break the connection
  while(TwoStep != NULL){
      TwoStep = TwoStep->next;

      if(TwoStep != NULL){
      	  OneStep = OneStep->next;
      	  TwoStep = TwoStep->next;
      	}
    }

  // Do the actual splitting
  *leftList = *head;
  *rightList = OneStep->next;
  // Then break the connection
  OneStep->next = NULL;
}


TreeNode* WeaveLists(TreeNode** upper, TreeNode** lower){
  TreeNode* merged = NULL;

  if(*upper == NULL)
    return *lower;
  else if (*lower == NULL)
    return *upper;

  // Sort by frequency
  if ((*upper)->frequency < (*lower)->frequency)
    {
      merged = *upper;
      merged->next = WeaveLists(&(*upper)->next, lower);
    }
  // If frequency is the same, but the ASCII is differen sort based on that
  else if ( ( (*upper)->frequency == (*lower)->frequency) && ((*upper)->ASCII_val <= (*lower)->ASCII_val) )
  {
    merged = *upper;
    merged->next = WeaveLists(&(*upper)->next, lower);
  }
  else
    {
      merged = *lower;
      merged->next = WeaveLists(upper, &(*lower)->next);
    }

  return merged;
}

void treeFromList(TreeNode** head){
  TreeNode* holder;
  long int newFreq = 0;

  // While ther is more than one node in the list
  while ( (*head)->next != NULL){

    // create a new node with it's left childs ASCII character (for sorting)
    // and the combined frequency of both children
    newFreq = (*head)->frequency + (*head)->next->frequency;
    holder = createTNode((*head)->ASCII_val, newFreq);

    // Assign the new node's children
    holder->leftChild = (*head);
    holder->rightChild = (*head)->next;

    // Assign the holder's next value in the list
    holder->next = (*head)->next->next;

    // Stop the children from having next values (purely a tree now)
    holder->leftChild->next = NULL;
    holder->rightChild->next = NULL;

    // Assign head to the new node and re sort
    *head = holder;
    linkedListSort(head);

  }

}
