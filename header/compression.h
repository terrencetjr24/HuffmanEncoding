#ifndef COMPRESSION_H
#define COMPRESSION_H

typedef struct encode
{
  unsigned char representation;
  unsigned char sigBits;
} encodeType;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tree_functions.h"

char * compress(FILE* inputF, encodeType* codeArray, long int* size, long int uncompressedSize);


#endif
