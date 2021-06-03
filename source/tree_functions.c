#include "../header/tree_functions.h"


TreeNode* treeCreate(FILE *treeFile, long int* freqArray, int charsUsed){
  TreeNode* tree = NULL;

  if (charsUsed < 1)
    return tree;

  // Create a list of tree Nodes first
  LinkedListCreate(&tree, charsUsed, freqArray, -1);
  linkedListSort(&tree);

  // Creating the actual tree from the list created
  treeFromList(&tree);
  // Printing the tree to a file
  PreOrderPrint(treeFile, tree);

  return tree;
}

void codeGen(FILE* codeFile, TreeNode* node, signed char* path, unsigned char idx){
  // Reached a leaf node and need to print
  if ((node->leftChild == NULL) && (node->rightChild == NULL)){
    fprintf(codeFile, "%c:", node->ASCII_val);

    for (int i=0; i < idx; i++){
      fprintf(codeFile, "%d", path[i]);
    }
    fprintf(codeFile, "\n");

    return;
  }

  path[idx] = 0;
  codeGen(codeFile, node->leftChild, path, idx+1);
  path[idx] = 1;
  codeGen(codeFile, node->rightChild, path, idx+1);

  // Reset the path array to -1 (for debugging)
  path[idx] = -1;
}

void usefulCodeGen(TreeNode* node, unsigned char nextSigDigit, unsigned char binaryRep, encodeType* codingArray){
  // Reached a leaf node and need to print
  if ((node->leftChild == NULL) && (node->rightChild == NULL)){
    codingArray[node->ASCII_val].representation = binaryRep;
    codingArray[node->ASCII_val].sigBits = nextSigDigit;

    return;
  }
  unsigned char nextRep;

  // Nothing changes since going left is signified by a zero
  nextRep = binaryRep;
  usefulCodeGen(node->leftChild, nextSigDigit+1, nextRep, codingArray);

  // There should be a one in the next significant place
  nextRep = binaryRep | (1 << nextSigDigit);
  usefulCodeGen(node->rightChild, nextSigDigit+1, nextRep, codingArray);
}

void PreOrderPrint(FILE* treeFile, TreeNode* tree){
  if(tree == NULL)
    return;
  if((tree->leftChild == NULL) && (tree->rightChild == NULL))
    {
      char temp = (char)(tree->ASCII_val);
      fprintf(treeFile, "1%c", temp); //print 1 and value then return
      return;
    }

  fprintf(treeFile, "%d", 0); //print 0

  PreOrderPrint(treeFile, tree->leftChild);
  PreOrderPrint(treeFile, tree->rightChild);
}

char* generateTopology(FILE* inputF, long int *size){
  char dummy;
  char* topology = NULL;

  fseek(inputF, 0, SEEK_SET);
  do {
    dummy = fgetc(inputF);
    if (dummy != EOF)
      *size = *size + 1;

  } while(dummy != EOF);

  // want one extra character for a null character
  topology = malloc(sizeof(char) * (*size+1));

  fseek(inputF, 0, SEEK_SET);
  for (int i = 0; i < *size; i++)
    topology[i] = fgetc(inputF);

  // NULL character at the end for safety
  topology[*size] = 0;

  return topologyHelper(topology, size);
}

char* topologyHelper(char* first, long int* actualSize){
  unsigned char holder = 0x00;
  bool h_valid = false;
  unsigned char valid_h_bits = 0;

  unsigned char readChar;
  unsigned char encoding;
  unsigned char encodingBits;

  long int allocatedSize = *actualSize;

  char* output = malloc(sizeof(char) * allocatedSize);

  int idx = 0;
  long int initialSize = *actualSize;
  *actualSize = 0;

  bool seenOne = false;
  bool prevOne = false;

  while (initialSize > 0){
    readChar = first[idx];

    if (prevOne){
      encoding = (unsigned char) readChar;
      encodingBits = 8;
      seenOne = false;
    }
    else{
      if (readChar == '0'){
        // get the encoding and how many of the bits are relevant
        encoding = 0x00;
        encodingBits = 1;
        seenOne = false;
      }
      else if (readChar == '1'){
        // get the encoding and how many of the bits are relevant
        encoding = 0x01;
        encodingBits = 1;
        seenOne = true;
      }
    }

    // Buffering the previous flag for if I've seen a one bit
    prevOne = seenOne;

    if (h_valid){
      // If the new encoding combined with the left over bits will overflow
      if ((valid_h_bits + encodingBits) > 8 ){
        // OR the bits that can be contributed
        unsigned char addingBits = 8 - valid_h_bits;
        holder = holder | (encoding << (8-addingBits));

        // Put the holder onto the output
        output[*actualSize] = holder;
        *actualSize = *actualSize + 1;
        if (*actualSize == allocatedSize){
          output = (char*) realloc(output, allocatedSize*2);
          allocatedSize *= 2;
        }
        // The holder is now the bits that were not added
        holder = 0x00 | (encoding >> addingBits);
        valid_h_bits = encodingBits - addingBits;
      }
      else if ((valid_h_bits + encodingBits) == 8 ){
        // OR the bits that can be contributed
        unsigned char addingBits = 8 - valid_h_bits;
        holder = holder | (encoding << (8-addingBits));
        // Put the holder onto the output
        output[*actualSize] = holder;
        *actualSize = *actualSize + 1;
        if (*actualSize == allocatedSize){
          output = (char*) realloc(output, allocatedSize*2);
          allocatedSize *= 2;
        }
        // The holder is now the bits that were not added
        holder = 0x00;
        valid_h_bits = 0;
        h_valid = false;
      }
      else{
        // OR the bits that can be contributed
        // Only need to shift by the bits that were left over from before
        holder = holder | (encoding << valid_h_bits);
        // The holder now holds the bits from before plus the new bits
        valid_h_bits += encodingBits;
      }

    }
    else if (encodingBits == 8){
      output[*actualSize] = encoding;
      *actualSize = *actualSize + 1;
      if (*actualSize == allocatedSize){
        output = (char*) realloc(output, allocatedSize*2);
        allocatedSize *= 2;
      }
    }
    else{
      h_valid = true;
      holder = holder | encoding;
      valid_h_bits = encodingBits;
    }

    initialSize = initialSize - 1;
    idx++;
  }


  if (h_valid){
    // Put the holder onto the output
    output[*actualSize] = holder;
    *actualSize = *actualSize + 1;
    if (*actualSize == allocatedSize){
      output = (char*) realloc(output, allocatedSize+1);
      allocatedSize += 1;
    }
  }

  // need to free the first topology array since I'm done with it
  free(first);

  return output;
}

void freeTree(TreeNode * node){
  if (node == NULL)
    return;

  freeTree(node->leftChild);
  freeTree(node->rightChild);
  free(node);
}
