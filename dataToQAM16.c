/*
dataToQAM128.c

Compile: gcc -Wall -o dataToQAM128 dataToQAM128.c -lliquid

Run program: ./dataToQAM128

Output File is : qamConversion.txt
*/

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <liquid/liquid.h>

#define OUTPUT_FILENAME "qamConversion"

// print usage/help message
void usage()
{
    printf("modem_example [options]\n");
    printf("  h     : print help\n");
    printf("  m     : modulation scheme (qam16 default)\n");
    liquid_print_modulation_schemes();
}

int main(int argc, char*argv[])
{
    // create mod/demod objects
    modulation_scheme ms = LIQUID_MODEM_QAM16;
    FILE*inputFile;
    int dopt;
    char *filename;
    char *fileContents;
    unsigned int lSize;

    while ((dopt = getopt(argc,argv,"hmf:")) != EOF) {
        switch (dopt) {
        case 'h':   usage();        return 0;
        case 'm':
            ms = liquid_getopt_str2mod(optarg);
            if (ms == LIQUID_MODEM_UNKNOWN) {
                fprintf(stderr,"error: %s, unknown/unsupported modulation scheme '%s'\n", argv[0], optarg);
                return 1;
            }
            break;
        case 'f' :

            filename = optarg;

            printf("Input Filename is: %s\n", filename);
            inputFile = fopen(optarg,"r");
            if( !inputFile ) perror(filename), exit(1);

            fseek(inputFile , 0L , SEEK_END);
            lSize = ftell( inputFile );
            rewind( inputFile );
            /* allocate memory for entire content */
            fileContents = calloc( 1, lSize+1 );
            if( !fileContents ) fclose(inputFile),fputs("memory alloc fails",stderr),exit(1);

            /* copy the file into the fileContents */
            if( 1!=fread( fileContents , lSize, 1 , inputFile) )
                fclose(inputFile),free(fileContents),fputs("entire read fails",stderr),exit(1);
            break;
        default:
            exit(1);
        }
    }

    // create the modem objects
    modem mod   = modem_create(ms);

    // ensure bits/symbol matches modem description (only
    // applicable to certain specific modems)
    unsigned int bps = modem_get_bps(mod);

    modem_print(mod);

    // open output file
    FILE*fid = fopen(OUTPUT_FILENAME,"wb");

    unsigned int sym_in; // modulated symbol
    unsigned int num_symbols = 1<<bps;
    float complex x;

    // Iterate through each Symbol in the input file and modulate it
    for (int i=0; fileContents[i]!='\0' && i< num_symbols; i++)
    {

        sym_in = fileContents[i]; //This stores the integer value of the character

        // modulate the first half of the integer
        modem_modulate(mod, ((0xF0&sym_in)/16), &x);

        // Map each component of the complex symbol to a range of -127 t0 127
        float real = 127 * crealf(x);
        float imag = 127 * cimagf(x);

        // convert the numbers to  8 bit signed integer format
        int8_t Ivalue[1];
        int8_t Qvalue[1];
        Ivalue[1] = (int8_t) real;
        Qvalue[1] = (int8_t) imag;
        // write binary to output file
        fwrite(&Ivalue, 1, 1, fid);
        fwrite(&Qvalue, 1, 1, fid);

        // modulate the second half of the integer
        modem_modulate(mod, (0xF&sym_in), &x);

        // Map each component of the complex symbol to a range of -127 t0 127
        real = 127 * crealf(x);
        imag = 127 * cimagf(x);

        // convert the numbers to  8 bit signed integer format
        Ivalue[1] = (int8_t) real;
        Qvalue[1] = (int8_t) imag;
        // write binary to output file
        fwrite(&Ivalue, 1, 1, fid);
        fwrite(&Qvalue, 1, 1, fid);

    }

    fclose(fid);
    printf("results written to %s.\n", OUTPUT_FILENAME);

    modem_destroy(mod);
    fclose(inputFile);
    free(fileContents);

    return 0;
}
