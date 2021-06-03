#include "main.h"

int main(int argc, char **argv){
   if(argc != 6){
      printf("The executable should be ran with 5 arguments.\n");
      return EXIT_FAILURE;
    }

  FILE * readingFile = NULL;
  FILE * countFile = NULL;
  FILE * treeFile = NULL;
  FILE * codeFile = NULL;
  FILE * compFile = NULL;

  int distinctCharactersUsed;
  TreeNode* createdTree;

  readingFile = fopen(argv[1], "r");
  if(readingFile == NULL) {
    printf("Error in opening the uncompressed file.\n");
    return EXIT_FAILURE;
   }

  countFile = fopen(argv[2], "wb");
  if(countFile == NULL) {
    printf("Error creating pointer for the count file.\n");
    return EXIT_FAILURE;
  }

  treeFile = fopen(argv[3] , "w");
  if(treeFile == NULL){
    printf("Error creating pointer for the tree file.\n");
    return EXIT_FAILURE;
  }

  codeFile = fopen(argv[4], "w");
  if(codeFile == NULL){
    printf("Error creating pointer for the code file.\n");
    return EXIT_FAILURE;
  }

  compFile = fopen(argv[5], "wb");
  if(compFile == NULL) {
    printf("Error creating pointer for compressed file.\n");
    return EXIT_FAILURE;
  }

  /* /////////////////////////////////////////////////////////
  Creating a tree representation of the uncompressed file
  */ /////////////////////////////////////////////////////////
  long int freqArray[256] = {0};

  distinctCharactersUsed = findCount(readingFile, countFile, freqArray);
  fclose(countFile);
  createdTree = treeCreate(treeFile, freqArray, distinctCharactersUsed);
  fclose(treeFile);

  ////////////////////////////////////////////////////////////

  /* /////////////////////////////////////////////////////////
  Code generation, both useless and usefull
  */ /////////////////////////////////////////////////////////
  signed char path[256] = {-1};
  encodeType codeArray[256];
  if(distinctCharactersUsed > 0)
    codeGen(codeFile, createdTree, path, 0);
  fclose(codeFile);

  usefulCodeGen(createdTree, 0, 0, codeArray);

  ////////////////////////////////////////////////////////////

  /* /////////////////////////////////////////////////////////
  Counting characters
  */ /////////////////////////////////////////////////////////

  long int totalCharsInUncompressed = 0;
  long int totalCharsInCompressed = 0;

  for (int i = 0; i < 256; i++){
    totalCharsInUncompressed += freqArray[i];
  }

  ////////////////////////////////////////////////////////////

  // take the tree topology from the generated file and put it into a string
  treeFile = fopen(argv[3] , "r");
  long int lengthOfTopology = 0;
  char* topology = generateTopology(treeFile, &lengthOfTopology);
  fclose(treeFile);


  // Generate the compressed the output and place that into a string
  char* compressedOutput = compress(readingFile, codeArray, &totalCharsInCompressed, totalCharsInUncompressed);

  long int firstheaderVal = (8*3 + lengthOfTopology + totalCharsInCompressed);
  fwrite(&firstheaderVal, sizeof(long int), 1, compFile);
  fwrite(&lengthOfTopology, sizeof(long int), 1, compFile);
  fwrite(&totalCharsInUncompressed, sizeof(long int), 1, compFile);

  fwrite(topology, sizeof(char), lengthOfTopology, compFile);

  fwrite(compressedOutput, sizeof(char), totalCharsInCompressed, compFile);

  free(topology);
  free(compressedOutput);
  fclose(readingFile);
  fclose(compFile);
  freeTree(createdTree);
  return EXIT_SUCCESS;
}
