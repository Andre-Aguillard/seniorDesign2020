#include <stdio.h>

#include <pigpio.h>

/* -----------------------------------------------------------------------

   3V3 ----- Light Dependent Resistor --+-- Capacitor ----- Ground
                                        |
                                        +-- gpio


  cc -o LDR LDR.c -lpigpio -lpthread -lrt
  sudo ./LDR

*/

#define LDR 18

/* forward declaration */

void alert(int pin, int level, uint32_t tick);

int main (int argc, char *argv[])
{
   if (gpioInitialise()<0) return 1;

   gpioSetAlertFunc(LDR, alert); /* call alert when LDR changes state */
    
   while (1)
   {
      gpioSetMode(LDR, PI_OUTPUT); /* drain capacitor */

      gpioWrite(LDR, PI_OFF);

      gpioDelay(200); /* 50 micros is enough, 200 is overkill */

      gpioSetMode(LDR, PI_INPUT); /* start capacitor recharge */

      gpioDelay(10000); /* nominal 100 readings per second */
   }

   gpioTerminate();
}

void alert(int pin, int level, uint32_t tick)
{
   static uint32_t inited = 0;
   static uint32_t lastTick, firstTick;

   uint32_t diffTick;

   if (inited)
   {
      diffTick = tick - lastTick;
      lastTick = tick;

      if (level == 1) printf("%u %d\n", tick-firstTick, diffTick);
   }
   else
   {
      inited = 1;
      firstTick = tick;
      lastTick = firstTick;
   }
}
