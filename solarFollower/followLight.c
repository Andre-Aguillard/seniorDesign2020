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
#define GPIO_UPPER 21
#define GPIO_LOWER 20
int map(int x);

//void moveLower(int A)

//void moveUpper(int B)


int map(int x) //returns teh input for the servos to move to.
{
    int y = 500/524 * x + 1500;
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


          int tr = analogRead(BASE+ 0); // Top right
          int tl = analogRead(BASE+ 1); // Top Left
          int br = analogRead(BASE+ 6); // Bottom Right
          int bl = analogRead(BASE+ 7); // Bottom Left

          for(i=0;i<2;i++)printf("Channel %d: value%4d\n",i,analogRead(BASE+i));
          for(i=6;i<8;i++)printf("Channel %d: value%4d\n",i,analogRead(BASE+i));
          printf("\n");

          int avgRight = (tr+tl)/2;
          int avgLeft = (tl+bl)/2;
          int avgBottom = (br+bl)/2;
          int avgTop = (tr+tl)/2;

          int A = avgLeft - avgRight;
          int B = avgTop - avgBottom;

          if ((A > 0 && B > 0) || (A < 0 && B > 0))
          {
              gpioServo(GPIO_LOWER, map(A));
              printf("%d\n",A);

              gpioServo(GPIO_UPPER, map(B));
              printf("%d\n",B);
          }

          if ((A > 0 && B > 0) || (A < 0 && B > 0))
          {
            gpioServo(GPIO_LOWER, map(A));
            printf("%d\n",map(A));

            gpioServo(GPIO_UPPER, map(-B));
            printf("%d\n",map(-B));
          }

          sleep(3); // Wait 10 seconds

}
gpioTerminate();
return 0;
}
