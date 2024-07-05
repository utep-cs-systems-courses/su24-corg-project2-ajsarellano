//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;          /* turn green LED off */
  P1OUT |= LED_RED;             /* turn red LED on */

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state vars that control blinking
int blinkLimitGreen = 5;  // duty cycle = 1/blinkLimit
int blinkCountGreen = 0;  // cycles 0...blinkLimit-1 (keep track of current blink cycle)

int blinkLimitRed = 3;
int blinkCountRed = 0;

int secondCount = 0; // state var to count time

void
__interrupt_vec(WDT_VECTOR) WDT()   /* 250 interrupts/sec */
{
  // handle blinking
  blinkCountGreen ++;                    // inremented each interrupt (250 interrupts per sec)
  if (blinkCountGreen >= blinkLimitGreen) {   // on for 1 interrupt period
    blinkCountGreen = 0;                 // reset if it reaches blinkLimit
    P1OUT &= ~LED_GREEN;
  } else                            // off for blinkLimit - 1 interrupt periods
    P1OUT |= LED_GREEN;

  blinkCountRed += 2;
  if (blinkCountRed >= blinkLimitRed) {
    blinkCountRed = 0;
    P1OUT &= ~LED_RED;
  } else
    P1OUT |= LED_RED;


  // measure a second
  secondCount ++;            // incremented each interrupt
  if (secondCount >= 125) {  // once each 0.5 second (faster)
    secondCount = 0;         // reset when it reaches 125
    
    blinkLimitGreen ++;           // reduce duty cycle
    if (blinkLimitGreen >= 8)     // but don't let duty cycle go below 1/7.
      blinkLimitGreen = 0;

    blinkLimitRed ++;
    if (blinkLimitRed >= 6)
      blinkLimitRed = 0;
  }
} 
  

