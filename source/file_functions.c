#include "../header/file_functions.h"

int findCount(FILE * ogFile, FILE *countFile,long int* array){
  int idx = 0;
  int distinctCharsInFile = 0;

  // Ensure the FILE pointer is set to the beginning of the file
  fseek(ogFile, 0, SEEK_SET);

  // Count the occurances of each ASCII character and increment the freqArray
  do {
    idx = fgetc(ogFile);
    if (idx != EOF)
      array[idx]++;

  } while(idx != EOF);

  // Write the frequency count of each character to the count file
  for(int i = 0; i < 256; i++)
    {
      fwrite(&(array[i]), sizeof(long int), 1, countFile);
      if(array[i] != 0)
	     distinctCharsInFile++;
    }

  // place the file pointer back at the beginning of the file
  fseek(ogFile,0, SEEK_SET);

  return distinctCharsInFile;
}
