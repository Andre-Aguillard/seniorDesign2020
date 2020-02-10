/*
     followLight.c
     gcc -o followLight followLight.c -lpigpio -lrt -lpthread
     sudo ./followLight
*/
#include <stdio.h>
#include <pigpio.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <wiringPiSPI.h>

#define BASE 100
#define SPI_CHAN 0
#define GPIO_UPPER 20
#define GPIO_LOWER 21
float map(int x);

void moveLower(int A)
{
    gpioServo(GPIO_LOWER, A);
}

void moveUpper(int B)
{
    gpioServo(GPIO_UPPER, B);
}

float map(int x) //returns teh input for the servos to move to.
{
    float y = 500/524 * x + 1500;
    return y;
}

int main(int argc, char *argv[])
{
      double start;
      if (gpioInitialise()<0)
      {
          fprintf(stderr, "pigpio initalisation failed\n");
          return 1;
      }
      // Setup pins for Servo output
      gpioSetMode(GPIO_LOWER, PI_OUTPUT); //Horizontal Servo output
      gpioSetMode(GPIO_UPPER, PI_OUTPUT); //Vertical Servo output

      // Set servo pulses | updates servos at 50Hz
      gpioServo(GPIO_LOWER, 1500); //Move servo to it's midpoint LOWER

      gpioServo(GPIO_UPPER, 1500); //Move servo to it's midpoint UPPER

      wiringPiSetup();
      int i;
      mcp3004Setup(BASE,SPI_CHAN);

      // initalize the SPI
      while(1) //Do the following continuously
      {
          for(i=0;i<8;i++)printf("Channel %d: value%4d\n",i,analogRead(BASE+i));
          printf("\n");

          int tr = analogRead(BASE+ 0); // Top right
          int tl = analogRead(BASE+ 1); //
          int br = analogRead(BASE+ 5); //
          int bl = analogRead(BASE+ 7); //

          int avgRight = (tr+tl)/2;
          int avgLeft = (tl+bl)/2;
          int avgBottom = (br+bl)/2;
          int avgTop = (tr+tl)/2;

          int A = avgLeft - avgRight;
          int B = avgTop - avgBottom;

          if ((A > 0 && B > 0) || (A < 0 && B > 0))
          {
              moveLower(map(A));
              moveUpper(map(B));
          }
          if ((A > 0 && B > 0) || (A < 0 && B > 0))
          {
              moveLower(map(A));
              moveUpper(map(B));
          }



          usleep(500000);


/*
     int dtime = 10;
     int tol = 50;


     dvert = avt-avd; // check the difference of up and down
     dhoriz = avl - avr; // check the difference of left and right

     if (-1*tol > dvert || dvert > tol) // check if the difference is in the tolerance else change the vertical angle
     {
     if (avt > avd)
     {




     }
     gpioTerminate();*/
}
gpioTerminate();
return 0;
}
