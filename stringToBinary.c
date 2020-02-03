// Program to convert Strings to Binary
// run using ./strToBinary filename

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// When this function is called from main it will print
// the binary values of each string to an output file.
void strToBinary(char* s)
{
    FILE *outputFile; //Creates the file pointer
    outputFile = fopen("binaryOutput.txt", "w+");
    printf("Made it past fopen - outputFile\n");

    // Each value of s is a character which is then converted to binary
    for (int i = 0; s[i]!='\0'; i++)
    {
        char *binary = malloc(9);
        int num = s[i];
        // printf("Converting: %c (%d) to Binary\n", num, num);
        for (int i=7; i>=0; i--)
        {
            binary[i] = ((num%2)==0) ? '0' : '1';
            //printf("Value of binary[i] is %d",binary[i]);
            num /= 2;
        }
        //printf("Printing %s to outputFile", binary);
        fprintf(outputFile, "%s ", binary);
    }
    fclose(outputFile);
    printf("Exiting subprogram\n");
}


/// Main program ////
// Desired input from cmd line is: ./stringToBinary -f test.txt
// Input file should be in same directory as program
int main(int argc, char *argv[])
{
	int opt;
	char *fileContents;
    long lSize;
    char *filename = argv[1];

	printf("Input Filename is: %s\n", filename);

    FILE *inputFile; //Creates the input file pointer

    inputFile = fopen( filename , "r");
    printf("Made it past fopen\n");

    if( !inputFile ) perror(filename),exit(1);

    fseek(inputFile , 0L , SEEK_END);
    lSize = ftell( inputFile );
    rewind( inputFile );

    printf("Made it past rewind\n");
    /* allocate memory for entire content */
    fileContents = calloc( 1, lSize+1 );
    if( !fileContents ) fclose(inputFile),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the fileContents */
    if( 1!=fread( fileContents , lSize, 1 , inputFile) )
        fclose(inputFile),free(fileContents),fputs("entire read fails",stderr),exit(1);

    /* fileContents is a string contains the whole text */
    printf("%s file contents read successfully\n", filename);
    printf("Entering strToBinary sub\n");
    strToBinary(fileContents);

    printf("Binary Conversion complete\n");
    fclose(inputFile);
    free(fileContents);


	return 0;
}
