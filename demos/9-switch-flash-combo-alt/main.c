#include <msp430.h>
#include "libTimer.h"

#define LED_GREEN BIT0           // P1.0
#define LED_RED BIT6             // P1.6
#define LEDS (LED_RED | LED_GREEN)

#define SW1 BIT3		/* switch1 is p1.3 */
#define SWITCHES SW1		/* only 1 switch on this board */

void switch_init() {
  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1DIR &= ~SWITCHES;		/* set switches' bits for input */
}

void led_init() {
  P1DIR |= LEDS;                /* set LEDs to output*/
  P1OUT &= ~LEDS;		/* LEDs initially off */
}

void wdt_init() {
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
}

void main(void) 
{  
  switch_init();
  led_init();
  wdt_init();
    
  or_sr(0x18);  // CPU off, GIE on
} 

static int buttonDown;
static int pressCount = 0;       // state variable representing amount of button presses
//static int secondCount = 0;      // state variable representing time
void
switch_interrupt_handler()
{
  char p1val = P1IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down (button press) */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up (button release) */

  if (p1val & SW1) {		/* button up, turn on green, buttonDown = 0 */
    P1OUT |= LED_GREEN;
    buttonDown = 0;
  } else {  /* button down, turn off green, buttonDown = 1 */
    P1OUT &= ~LED_GREEN;
    buttonDown = 1;

    if (pressCount == 8) { // check if pressCount is 8 (the 9th press)
      pressCount = 0;
    } else {
      pressCount++;
    }
    
    //pressCount++;
  }
}


/* Switch on P1 (S2) */
//handles interrupts from port 1
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P1IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

int secondCount = 0;
void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  static int blink_count = 0;
  switch (blink_count) { 
  case 6: 
    blink_count = 0;
    //P1OUT |= LED_RED;

    if (pressCount >= 0 && pressCount < 3) {           // turn on red LED
      P1OUT |= LED_RED;
    } else if (pressCount >= 3 && pressCount < 6) {    // turn off red LED
      P1OUT &= ~LED_RED;
    } else if (pressCount >= 6 && pressCount < 9) {    // repeatedly blink red LED
      if (secondCount >= 125) {
	secondCount = 0;
	P1OUT |= LED_RED;
      } else {
	secondCount += 4;
	P1OUT &= ~LED_RED;
      }
    }
    
    break;
  default:
    blink_count ++;
    if (!buttonDown) P1OUT &= ~LED_RED; /* don't blink off if button is down */
  }
} 
