//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  //P1OUT |= LED_RED;
  P1OUT &= ~LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

char green_state = 0;
char red_state = 0;

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // dim green
  if (green_state == 2) {
    P1OUT |= LED_GREEN; // green LED on
    green_state = 3;
  } else if (green_state == 3) {
    P1OUT &= ~LED_GREEN; // green LED off
    green_state = 0;
  } else {
    green_state++;
  }

  //dim red
  if (red_state == 2) {
    P1OUT |= LED_RED; // red LED on
    red_state = 3;
  } else if (red_state == 3) {
    P1OUT &= ~ LED_RED; //red LED off
    red_state = 0;
  } else {
    red_state++;
  }
  
} 

