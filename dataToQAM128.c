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

#define OUTPUT_FILENAME "qamConversion.txt"

// print usage/help message
void usage()
{
    printf("modem_example [options]\n");
    printf("  h     : print help\n");
    printf("  v/q   : verbose/quiet\n");
    printf("  m     : modulation scheme (qam128 default)\n");
    liquid_print_modulation_schemes();
}


int main(int argc, char*argv[])
{
    // create mod/demod objects
    modulation_scheme ms = LIQUID_MODEM_QAM128;
    int verbose = 1;
    FILE*inputFile;
    int dopt;
    char *filename;
    char *fileContents;
    unsigned int lSize;

    while ((dopt = getopt(argc,argv,"hvqmf:")) != EOF) {
        switch (dopt) {
        case 'h':   usage();        return 0;
        case 'v':   verbose = 1;    break;
        case 'q':   verbose = 0;    break;
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
    modem demod = modem_create(ms);

    // ensure bits/symbol matches modem description (only
    // applicable to certain specific modems)
    unsigned int bps = modem_get_bps(mod);

    modem_print(mod);

    // open output file
    FILE*fid = fopen(OUTPUT_FILENAME,"w");

    unsigned int sym_in; // modulated symbol
    unsigned int s; // demodulated symbol
    unsigned int num_symbols = 1<<bps;
    float complex x;
    unsigned int num_sym_errors = 0;
    unsigned int num_bit_errors = 0;

    // Iterate through each Symbol in the input file and modulate it
    for (int i=0; fileContents[i]!='\0' && i< num_symbols; i++)
    {

        sym_in = fileContents[i];
        modem_modulate(mod, sym_in, &x);
        modem_demodulate(demod, x, &s);

        if (verbose)
            printf("%4u : %12.8f + j*%12.8f\n", sym_in, crealf(x), cimagf(x));

        num_sym_errors += sym_in == s ? 0 : 1;
        num_bit_errors += count_bit_errors(sym_in,s);

        // write symbol to output file
        fprintf(fid,"%c -> c(%3u) = %12.4e + j*%12.4e;\n", sym_in, sym_in, crealf(x), cimagf(x));
        //fprintf(fid,"i_str{%3u} = [num2str(%3u)];\n", i+1, i);
    }
    printf("num sym errors: %4u / %4u\n", num_sym_errors, lSize);
    printf("num bit errors: %4u / %4u\n", num_bit_errors, lSize);


    fclose(fid);
    printf("results written to %s.\n", OUTPUT_FILENAME);

    modem_destroy(mod);
    modem_destroy(demod);
    fclose(inputFile);
    free(fileContents);

    return 0;
}
