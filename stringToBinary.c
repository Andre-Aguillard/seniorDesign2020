// Program to convert Strings to Binary


#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

long long convert(int n);

// When this function is called from main it will print 
// the binary values of each string to an output file. 
void strToBinary(char* s) 
{ 
    int n = sizeof(s);
    char bin[n];
    FILE *outputFile; //Creates the file pointer
    outputFile = fopen(optarg, "w+");
    

    // Each value of s is a character which is then converted to binary
    for (int i = 0; s[i]!='\0'; i++) 
    { 
        // convert each char's ASCII value to binary 
        bin[i]=convert(s[i]);
        // Now bin[i] is a string of 0s and 1s
        printf("%d", bin[i]);

        // Need to pad the new binary number so that it is 8 bites
        int size = sizeof(bin[i])/sizeof(char);
        size -= 1;
        char pad[] = "00000000";
        int diff = 8 - size;

        int dre = bin[i];
        char binary_number[sizeof(bin[i])] = dre;

        for (int j = 0; j < size; j++)
        {
            pad[diff+j] = binary_number[j];
        }
        printf("%s", pad);

        fprintf(outputFile, "%s\n", pad);
    } 
                    
    fclose(outputFile);
}

long long convert(int n) {
    long long bin = 0;
    int rem, i = 1, step = 1;
    while (n != 0) {
        rem = n % 2;
        //printf("Step %d: %d/2, Remainder = %d, Quotient = %d\n", step++, n, rem, n / 2);
        n /= 2;
        bin += rem * i;
        i *= 10;
    }
    return bin;
}

/*char* padder(char binary_number, int size)
{
    size -= 1;
    char pad[] = "00000000";
    int diff = 8 - size;
    for (int i = 0; i < size; i++)
    {
        pad[diff+i] = binary_number[i];
    }
    printf("%s", pad);
    // return the string for printing to the output file
    return pad*;
}*/

/// Main program ////
// Desired input from cmd line is: ./stringToBinary -f test.txt 
// Input file should be in same directory as program
int main(int argc, char *argv[]) 
{ 
	int opt; 
	char filecontents[140],c;
	// put ':' in the starting of the 
	// string so that program can 
	//distinguish between '?' and ':' 
	while((opt = getopt(argc, argv, ":f")) != -1) 
	{ 

		switch(opt) 
		{ 
			case 'f':
 				printf("Input Filename is: %s\n", optarg); 

                FILE *inputFile; //Creates the input file pointer
                inputFile = fopen(optarg, "r");
                if (inputFile = NULL )
                {
                    printf("%s file failed to open.", optarg);
                        exit(0);
                }
                else
                {
                
                    fscanf(inputFile, "%s", filecontents);

                    printf("%s file contents read successfully\n", optarg);

                    strToBinary(filecontents);

                    printf("Binary Conversion complete");
                    fclose(inputFile);
                }
				break; 
 			case ':': 
				printf("option needs a value\n"); 
				break; 
			case '?': 
				printf("unknown option: %c\n", optopt); 
				break; 
		} 
	} 
    
	
	// optind is for the extra arguments 
	// which are not parsed 
	for(; optind < argc; optind++){	 
		printf("extra arguments: %s\n", argv[optind]); 
	} 
	
	return 0; 
} 

