/*
     sensorData.c
     Program for collecting sensor data and saving it as a data file
     for transmission to the ground station. Program only samples once right now.
     Compile:
     gcc -o sensorData sensorData.c -lpigpio -lwiringPi -lrt -lpthread
     Run:
     sudo ./sensorData
*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <wiringPiSPI.h>

#define BASE 100
#define SPI_CHAN 0
#define OUTPUT_FILENAME "sensorData.txt"
#define WAIT_TIME 8 //defines the time before new data is collected

int main(int argc, char *argv[])
{
    wiringPiSetup();
    mcp3004Setup(BASE,SPI_CHAN);
    // open output file
    FILE *fid = fopen(OUTPUT_FILENAME,"w+");

    // Collect data
      int A = analogRead(BASE+7); // Read from pin 0
      int B = analogRead(BASE+6); // Read from pin 1
      int C =  analogRead(BASE+5); // Read from pin 2
    // Save data to file
      fprintf(fid,"Pot on channel 7: %d\n",A);
      fprintf(fid,"LDR on channel 6: %d\n",B);
      fprintf(fid,"Thermistor on channel 5: %d\n\n",C);
    // print data to standard out
      printf("Pot on channel 7: %d\n",A);
      printf("LDR on channel 6: %d\n",B);
      printf("Thermistor on channel 5: %d\n\n",C);

      printf("Results written to %s.\n", OUTPUT_FILENAME);
      fclose(fid);
      return 0;
}
