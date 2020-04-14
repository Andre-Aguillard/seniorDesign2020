/*
     sensorData.c
     Program for collecting sensor data and saving it as a data file
     for transmission to the ground station.
     Compile:
     gcc -o sensorData sensorData.c -lpigpio -lrt -lpthread
     Run:
     sudo ./sensorData
*/
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <wiringPiSPI.h>

#define BASE 100
#define SPI_CHAN 0
#define OUTPUT_FILENAME "sensorData.txt"
#define WAIT_TIME 5 //defines the time before new data is collected

int main(int argc, char *argv[])
{
    wiringPiSetup();
    mcp3004Setup(BASE,SPI_CHAN);
    // open output file
    FILE *fid = fopen(OUTPUT_FILENAME,"w+");

    // initalize the SPI
    while(1) //Do the following continuously
    {
      // Collect data
        int A = analogRead(BASE); // Read from pin 0
        int B = analogRead(BASE+1); // Read from pin 1
        int C =  analogRead(BASE+2); // Read from pin 2
      // Save data to file
        fprintf(fid,"Pot on channel 0: value%4d\n",A);
        fprintf(fid,"LDR on channel 1: value%4d\n",B);
        fprintf(fid,"Thermistor on channel 2: value%4d\n",C);
      // print data to standard out
        printf("Pot on channel 0: value%4d\n",A);
        printf("LDR on channel 1: value%4d\n",B);
        printf("Thermistor on channel 2: value%4d\n",C);

        printf("\n");
        fclose(fid);
        printf("results written to %s.\n", OUTPUT_FILENAME);
        printf("Next update will be in %d seconds", WAIT_TIME);
        sleep(WAIT_TIME);
    }
return 0;
}
