//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN; //turn off green LED
  P1OUT |= LED_RED; //turn on red LED

  or_sr(0x18);		/* CPU off, GIE on */
}
