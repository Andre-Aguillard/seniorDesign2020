/*
Simple C program to read binary files.

Compile: gcc -o readBin readBin.c

Run using: ./readBin -f "inputFilename"

*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>

// print usage/help message
void usage()
{
    printf("modem_example [options]\n");
    printf("  h     : print help\n");
    printf("  f     : input file address\n");
    printf("  d     : print debug\n");
}

int main(int argc, char*argv[])
{
    FILE*inputFile = NULL;
    int dopt;
    char *filename;
    signed char *fileContents;
    unsigned int lSize = 0;
    int debug = 0;

    while ((dopt = getopt(argc,argv,"hdf:")) != EOF) {
        switch (dopt) {
        case 'h':   usage();        return 0;
        case 'd' :
            debug = 1;
            printf("Debug is on.\n");
            break;
        case 'f' :

            filename = optarg;

            printf("Input Filename is: %s\n", filename);
            inputFile = fopen(optarg,"rb");
            if( !inputFile ) perror(filename), exit(1);

            fseek(inputFile, 0, SEEK_END);
            lSize = ftell(inputFile);
            rewind( inputFile );
            /* allocate memory for entire content */
            fileContents = malloc((lSize+1)*sizeof(*inputFile) );
            if( !fileContents ) fclose(inputFile),fputs("memory alloc fails",stderr),exit(1);

            /* copy the file into the fileContents */
            fread(fileContents, sizeof(signed char), lSize , inputFile);

            break;
        default:
            exit(1);
        }
    }
    printf("Done reading file \n");

    // attempt to print the numbers
    int i =0;
    while(i < lSize)
    {
        int8_t Ivalue = fileContents[i];
        printf("%d \n", Ivalue);
        i += 1;
    }
}
