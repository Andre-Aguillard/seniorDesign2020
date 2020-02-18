/*
     dontCare.c
     gcc -o dontCare dontCare.c -lpigpio -lrt -lpthread 
     sudo ./dontCare
*/
#include <stdio.h>
#include <pigpio.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <wiringPiSPI.h>

#define BASE 100
#define SPI_CHAN 0

int main(int argc, char *argv[])
{
    wiringPiSetup();
    int i;
    mcp3004Setup(BASE,SPI_CHAN);

    // initalize the SPI
    while(1) //Do the following continuously
    {
        printf("Pot on channel 3: value%4d\n",analogRead(BASE+3));// Read from pin 2
        printf("LDR on channel 4: value%4d\n",analogRead(BASE+4));
        printf("Thermistor on channel 5: value%4d\n",analogRead(BASE+5));


        printf("\n");
        sleep(5);
    }
return 0;
}
