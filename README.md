# Huffman Encoding
- Author: Terrence Randall

## Description:
A compression program intended for the compression of none-empty text files.

## Running the program
- The same format as the documentation given
- $ make
- $ ./compress *input_file* *binary_count_file* *tree_file* *code_file* *compressed_file*.hbt
- Notes:
  binary_count_file: binary file to store the number of bytes in the input
  tree_file: file to store the topology of the created tree
  code_file: file to hold the encoding of characters
  compressed_file: to hold the compressed output, with the extension .hbt
