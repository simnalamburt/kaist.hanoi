#include <stdio.h>
#include "hanoi.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fputs("Input should be: './hanoi input_file > output_file'\n", stderr);
    return 1;
  }

  hanoi(argv[1]);
  return 0;
}
