/******************************************************************************
                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
void strToBinary(char *s);

int main()
{
    char test[] = "This is a test file.";
    printf("%s\n",test);
    strToBinary(test);

    return 0;
}

void strToBinary(char* s)
{
    for (int i = 0; s[i]!='\0'; i++)
    {
        char *binary = malloc(9);
        int num = s[i];
        printf("Converting: %c to Binary\n", num);
        for (int i=7; i>=0; i--)
        {
            binary[i] = ((num%2)==0) ? '0' : '1';
            num /= 2;
        }
        printf("Printing %s to outputFile\n", binary);
    }
}
