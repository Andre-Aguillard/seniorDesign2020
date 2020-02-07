/* 
     followLight.c
     gcc -o followLight followLight.c -lpigpio -lrt -lpthread
     sudo ./followLight
*/
#include <stdio.h>
#include <pigpio.h>
#include "mcp3008.h"

int main(int argc, char *argv[])
{
     double start;
     if (gpioInitialise()<0)
     {
          fprintf(stderr, "pigpio initalisation failed\n");
          return 1;
     }
     gpioSetMode(21, PI_OUTPUT); //Light up an lED

     // Setup input from 4 photoresitors
     gpioSetMode(16, PI_INPUT); // topLeft  (ldrlt) 
     gpioSetMode(20, PI_INPUT); // topRight (ldrrt)
     gpioSetMode(12, PI_INPUT); // downLeft (ldrdl)
     gpioSetMode(25, PI_INPUT); // downRight (ldrdr)
     
     // Setup pins for Servo output
     gpioSetMode(26, PI_OUTPUT); //Horizontal Servo output
     gpioSetMode(19, PI_OUTPUT); //Vertical Servo output
     
     // Set servo pulses | updates servos at 50Hz
     gpioServo(26, 1500); //Move servo to it's midpoint
     gpioServo(19, 1500); //Move servo to it's midpoint
     
     // initalize the SPI 
     while(1) //Do the following continuously
     {
     // Input code relating to the analog read stuff here
     	int lt = mcp3008_value(0, 11, 9, 8); //read top left
	printf(%d, lt);
	int rt = mcp3008_value(1, 11, 9, 8); // read top right
	int ld = mcp3008_value(2, 11, 9, 8); //read down left
	int rd = mcp3008_value(3, 11, 9, 8); //read down right
	printf(%d, rt);
	printf(%d, ld);
	printf(%d, rd);
     */
  
     int dtime = 10;
     int tol = 50;

     int avt = (lt + rt) / 2; // average value top
     int avd = (ld + rd) / 2; // average value down
     int avl = (lt + ld) / 2; // average value left
     int avr = (rt + rd) / 2; // average value right

     dvert = avt-avd; // check the difference of up and down
     dhoriz = avl - avr; // check the difference of left and right
     
/*
     if (-1*tol > dvert || dvert > tol) // check if the difference is in the tolerance else change the vertical angle
     {
     if (avt > avd)
     {
           



     }
*/
     start = time_time();
     while((time_time() - start) < 60.0)
     {
          gpioWrite(21,1);
	  time_sleep(0.5);
	  gpioWrite(21,0);
	  time_sleep(0.5);
     }
     gpioTerminate();
     return 0;
}
