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
        printf("Pot on channel 2: value%4d\n",analogRead(BASE+2));// Read from pin 2
        printf("Thermistor on channel 8: value%4d\n",analogRead(BASE+8));
        printf("LDR on channel 7: value%4d\n",analogRead(BASE+7));


        printf("\n")
        usleep(500000);
    }
return 0;
}
