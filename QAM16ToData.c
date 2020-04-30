/*
QAM16toData.c
The demodulator program for going from QAM16 encoding to ASCII format.

Compile: gcc -Wall -o QAM16toData QAM16toData.c -lliquid

Run program: ./QAM128toData -f "filename"

Output File is : data.txt
*/

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <liquid/liquid.h>
#include <complex.h>
#include <math.h>

#define OUTPUT_FILENAME "output.txt"

// print usage/help message
void usage()
{
    printf("modem_example [options]\n");
    printf("  h     : print help\n");
    printf("  m     : modulation scheme (qam16 default)\n");
    printf("  f     : input file address\n");
    printf("  d     : print debug\n");
    liquid_print_modulation_schemes();
}

int main(int argc, char*argv[])
{
    // create mod/demod objects
    modulation_scheme ms = LIQUID_MODEM_QAM16;
    FILE*inputFile; // = NULL
    int dopt;
    char *filename;
    signed char *fileContents;
    unsigned int lSize = 0;
    int debug = 0;

    while ((dopt = getopt(argc,argv,"hmdf:")) != EOF) {
        switch (dopt) {
        case 'h':   usage();        return 0;
        case 'm':
            ms = liquid_getopt_str2mod(optarg);
            if (ms == LIQUID_MODEM_UNKNOWN) {
                fprintf(stderr,"error: %s, unknown/unsupported modulation scheme '%s'\n", argv[0], optarg);
                return 1;
            }
            break;
        case 'd' :
            debug = 1;
            printf("Debug is on.\n");
            break;
        case 'f' : /* Create the input file and read it's contents: */

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
            fclose(inputFile);
            break;
        default:
            exit(1);
        }
    }

    // create the modem object
    modem demod   = modem_create(ms);

    // if(debug=1) modem_print(demod);

    // open output file
    FILE* fid = fopen(OUTPUT_FILENAME,"w+");
    if (!fid) perror(OUTPUT_FILENAME), exit(1);
    //if (debug = 1) printf("Opened the output file.\n");
    /* Thoughts:
    Each ASCII character in the original file is broken down to it's binary equivalent.
        For example A = 65 = 01000001 in binary
    Then that binary representation of the integer is split into two four bit numbers.
        Example: A = 0100 0001 = 4 and 1 in decimal
    That new binary number is then passed into the modulate function, and the output
    of that function is a complex number x.
    The complex number x is then broken down to it's real and imaginary parts and
    multiplied by 127

    Each index of fileContents is either an I or a Q value.
    print to a file using fprintf */

    char output[(lSize/2)];
    int idx = 0; //output char array index
    int i = 0;
    if(debug == 1) printf("lSize equals: %d \n",lSize);

    while ( i  < lSize)
    {
        if (debug == 1) printf("In the while loop on i= %d\n", i );
      //FIRST HALF
        // Ivalue is First
        int8_t Ivalue1;
        Ivalue1 = fileContents[i];
        float real1;
        real1 = Ivalue1 / 127.0; //unmap the value
        i += 1; // increment i to go to Qvalue

        // then Qvalueprintf("%s",output[idx]);
        if(i  >= lSize) break;
        int8_t Qvalue1;
        Qvalue1 = fileContents[i];
        float imag1 = Qvalue1 / 127.0;

        // Combine real and imaginary to get complex input to modem
        float complex z1 = real1 +imag1*I;

        unsigned int sym_out1; //demodulated first 4 bits
        modem_demodulate(demod, z1, &sym_out1); //demodulates a symbol

      // SECOND HALF
        if(i  >= lSize) break;
        i += 1; //increment i again to go to Ivalue

        // Ivalue is First
        int8_t Ivalue2;
        Ivalue2 = fileContents[i];
        float real2 = Ivalue2 / 127.0; //unmap the value
        i += 1; // increment i to go to Qvalue
        if(i  >= lSize) break;
        // then Qvalue
        int8_t Qvalue2;
        Qvalue2 = fileContents[i];
        float imag2 = Qvalue2 / 127.0;
        i += 1;
        // Combine real and imaginary to get complex input to modem
        float complex z2 = real2 +imag2*I;

        unsigned int sym_out2; //demodulated last 4 bits
        modem_demodulate(demod, z2, &sym_out2); //demodulates a symbol

      // COMBINE THEM
        char sym_out; //final ASCII value of symbol
        //shift the first 4 bits up 4 and OR with second
        sym_out = sym_out1 << 4 | sym_out2;
        // char printThis = (char) sym_out;
        output[idx] = sym_out;
        idx += 1;
        if (debug == 1) printf("%d\n", sym_out);
    }
    fprintf(fid,"%s\n",output);
    if (debug == 1) printf("%s\n", output);
    fclose(fid);
    printf("Results written to %s.\n", OUTPUT_FILENAME);

    modem_destroy(demod);
    free(fileContents);

    return 0;
}
