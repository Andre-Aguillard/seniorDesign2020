# seniorDesign2020

The main program is stringToBinary.c which converts a text file specified in the
command line and located in the same directory as the program to a string of binary values.

## stringToBinary.c

To run the program:
1. Create a text file and save it to the same directory as the program
2. Run the command
    - gcc stringToBinary.c -o stringToBinary && ./stringToBinary {filename.txt}
    - type the name of your file to replace {filename.txt}
3. The output file will then be binaryOutput.txt located in the same place directory

## The Plan for the rest of the project
The plan is to connect this program with the liquid dsp and HackRF tools to send
data found in text files over the radio using QAM

- AA 02/02/20
