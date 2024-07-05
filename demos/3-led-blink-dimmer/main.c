//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;                
  P1OUT &= ~LED_GREEN;          /* turn off green LED */
  P1OUT |= LED_RED;             /* turn on red LED */

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state var to count time (keeps track of amount of interrupts)
int secondCount = 0;


void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  secondCount ++;               /* increase counter each time interrupt occurs*/
  if (secondCount >= 125) { 	/* once each half second (twice as fast)... */
    secondCount = 0;		/* reset count */
    P1OUT ^= LED_GREEN;         /* toggle state of green LED */
    P1OUT ^= LED_RED;           /* toggle state of red LED*/
  }
} 

