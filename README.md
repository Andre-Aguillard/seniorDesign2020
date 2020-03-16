# seniorDesign2020

This project contains four key programs:
      **stringToBinary.c** which converts a text file specified in the command line
      and located in the same directory as the program to a string of binary values.
      **dataToQAM16.c** which uses the liquid dsp library to convert a text file to
      IQ values represented as complex numbers.
      **followLight.c** which is the solar follower/ solar tracker program.
      **QAM16ToData.c** is the program for demodulating and reading the transmitted
      data from the HACK_RF using the RTL-SDR.

## stringToBinary.c

To run the program:
1. Create a text file and save it to the same directory as the program

2. Run the command
```
     gcc stringToBinary.c -o stringToBinary && ./stringToBinary {filename.txt}
```
   Type the name of your file to replace {filename.txt}

3. The output file will then be binaryOutput.txt located in the same directory

## dataToQAM16.c
To set up the liquid dsp library do the following:

### Installation and Dependencies

liquid-dsp only relies on `libc` and `libm` (standard C and math)
libraries to run; however liquid will take advantage of other libraries
(such as [FFTW](http://www.fftw.org)) if they are available.

If you build from the Git repository you will also need to install autotools
for generating the `configure.sh` script (e.g.
`brew install autoconf automake` on macOS,
`sudo apt-get install automake autoconf` on Debian variants).

#### Installation ###

The recommended way to obtain the source code is to clone the entire
[repository](https://github.com/jgaeddert/liquid-dsp) from
[GitHub](https://github.com):

     git clone git://github.com/jgaeddert/liquid-dsp.git

Building and installing the main library is a simple as

    ./bootstrap.sh
    ./configure
    make
    sudo make install

If you are installing on Linux for the first time, you will also need
to rebind your dynamic libraries with `sudo ldconfig` to make the
shared object available.
This is not necessary on macOS.

If you decide that you want to remove the installed DSP library, simply
run

    `sudo make uninstall`

#### Running the program
To run the program:
1. Create a text file and save it to the same directory as the program

2. Run the command
    - `gcc -Wall -o dataToQAM128 dataToQAM128.c -lliquid && ./dataToQAM128 -f {filename.txt}`
    - type the name of your file to replace {filename.txt}

3. The output file will then be qamConversion.txt located in the same directory

## QAM16ToData.c

The rtl-sdr tools can be found in the folder rtl-sdr tools and are used for receiving and demodulating
signals at a specific frequency.

more information about the tools can be found here: https://osmocom.org/projects/rtl-sdr/wiki/Rtl-sdr

## Solar Tracker
The solar Tracker program can be found in the Solar Follower folder and the main program is followLight.c

## SENDING AND RECIEVING data

To send data from the HACK-RF use the following:

To receive use the following:



## The Plan for the rest of the project
The plan is to connect this program with the liquid dsp and HackRF tools to send
data found in text files over the radio using QAM

- AA 02/12/20
