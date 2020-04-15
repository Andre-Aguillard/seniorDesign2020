/*
     followLight.c
     gcc -o followLight followLight.c -lpigpio -lrt -lpthread -lwiringPi -lm -ldl
     sudo ./followLight
*/
#include <stdio.h>
#include <pigpio.h>
#include <math.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <wiringPiSPI.h>

#define BASE 100
#define SPI_CHAN 0	
#define GPIO_UPPER 21
#define GPIO_LOWER 20


void moveLower(int A)
{
    gpioServo(GPIO_LOWER, A);
    printf("Moving the lower servo to %d\n\n", A);
}

void moveUpper(int B)
{
    gpioServo(GPIO_UPPER, B);
    printf("Moving the upper servo to %d\n\n", B);
}

int mapUp(signed int x)  	//returns the input for the servos to move to.
{			// Servo range is 500 to 2300 , LDR range is 0 to 1000
    int y = ((0.9 * x)+1400);
   // printf("The value of y is %d\n\n",y);
    return y;
}
int mapLow(signed int x)  	//returns the input for the servos to move to.
{			// Servo range is 500 to 2400 , LDR range is 0 to 1000
    int y = ((0.95 * x)+1450);
   // printf("The value of y is %d\n\n",y);
    return y;
}

int main(int argc, char *argv[])
{
      if (gpioInitialise()<0)
      {
          fprintf(stderr, "pigpio initalisation failed\n");
          return 1;
      }
 
      // Set servo pulses | updates servos at 50Hz
      gpioServo(GPIO_LOWER, 1450); //Move servo to it's midpoint LOWER

      gpioServo(GPIO_UPPER, 1450); //Move servo to it's midpoint UPPER

      wiringPiSetup();
      int i;
      mcp3004Setup(BASE,SPI_CHAN);

      // initalize the SPI

      while(1) //Do the following continuously
      {

          int tr = analogRead(BASE+ 7); // Top right
          int tl = analogRead(BASE+ 6); // Top Left
          int br = analogRead(BASE+ 5); // Bottom Right
          int bl = analogRead(BASE+ 4); // Bottom Left
	 
	  printf("TopRight =%d \nTopLeft =%d \nBottomRight =%d \nBottomLeft =%d\n\n", tr,tl,br,bl);

          int avgRight = (tr+br)/2;
          int avgLeft = (tl+bl)/2;
          int avgBottom = (br+bl)/2;
          int avgTop = (tr+tl)/2;
	  
	  //printf(" Average Right: %d\n Average Left: %d\n Average Bottom: %d\n AverageTop: %d\n\n", avgRight, avgLeft, avgBottom, avgTop);
	  
          signed int A = avgLeft - avgRight;
          signed int B = avgTop - avgBottom;
	  signed int C = sqrt((A*A)+(B*B));
	  printf("Value of A = %d\n",A);
	  printf("Value of B = %d\n\n",B);
	  
          if ((A > 0 && B > 0) || (A < 0 && B > 0))
          {
              moveLower(mapLow(A));
              moveUpper(mapUp(C));
          }
          else if ((A < 0 && B < 0) || (A > 0 && B < 0))
          {
              moveLower(mapLow(-A));
              moveUpper(mapUp(-C));
          } 
	  	
          sleep(2); // Wait 2 seconds

}
gpioTerminate();
return 0;
}
