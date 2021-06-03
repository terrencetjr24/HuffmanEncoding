#include "../header/compression.h"


char * compress(FILE* inputF, encodeType* codeArray, long int* size, long int inputLength){
  unsigned char holder = 0x00;
  bool h_valid = false;
  unsigned char valid_h_bits = 0;

  unsigned char readChar;
  unsigned char encoding;
  unsigned char encodingBits;

  long int allocatedSize = 100;

  char* output = malloc(sizeof(char) * allocatedSize);

  fseek(inputF, 0, SEEK_SET);

  while (inputLength > 0){
    readChar = fgetc(inputF);
    // printf("Read %c\n", readChar);

    // If we read something valid
    if (readChar != EOF){
      // get the encoding and how many of the bits are relevant
      encoding = codeArray[readChar].representation;
      encodingBits = codeArray[readChar].sigBits;

      //printf("Encoding is %x (%d bits)\n", encoding, encodingBits);
      if (h_valid){
        // If the new encoding combined with the left over bits will overflow
        if ((valid_h_bits + encodingBits) > 8 ){
          // OR the bits that can be contributed
          unsigned char addingBits = 8 - valid_h_bits;
          holder = holder | (encoding << (8-addingBits));

          // Put the holder onto the output
          output[*size] = holder;
          //printf("Wrote (%x) to the output\n", holder);
          *size = *size + 1;
          if (*size == allocatedSize){
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
          output[*size] = holder;
          //printf("Wrote (%x) to the output\n", holder);
          *size = *size + 1;
          if (*size == allocatedSize){
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
        output[*size] = encoding;
        //printf("Wrote (%x) to the output\n", encoding);
        *size = *size + 1;
        if (*size == allocatedSize){
          output = (char*) realloc(output, allocatedSize*2);
          allocatedSize *= 2;
        }
      }
      else{
        h_valid = true;
        holder = holder | encoding;
        valid_h_bits = encodingBits;
      }

    }
    // if (h_valid)
    //   printf("holder is valid with %x (%d bits)\n", holder, valid_h_bits);
    // else
    //   printf("holder is invalid\n");
    //
    // printf("\n");

    inputLength = inputLength - 1;
  }


  if (h_valid){
    // Put the holder onto the output
    output[*size] = holder;
    // printf("Wrote (%x) to the output\n", holder);
    *size = *size + 1;
    if (*size == allocatedSize){
      output = (char*) realloc(output, allocatedSize*2);
      allocatedSize *= 2;
    }
  }


  return output;
}
