#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  FILE *outputFile;
  outputFile = fopen("binaryOutput.txt", "w+");
  fprintf(outputFile, "This is a Test File!\n");
  fclose(outputFile);
  return 0;
}
