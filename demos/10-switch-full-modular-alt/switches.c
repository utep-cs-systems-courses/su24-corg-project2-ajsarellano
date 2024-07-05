#include <msp430.h>
#include "switches.h"
#include "led.h"

/* switches.c responds to the switch and sets the values of state vars */

char switch_state_down, switch_state_changed; /* effectively boolean (true, false) */

static char 
switch_update_interrupt_sense()
{
  char p1val = P1IN; // read current state of pins on port 1
  
  /* update switch interrupt to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down (not pressed) */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up (pressed) */
  return p1val; // return current state of port 1
}

void 
switch_init()			/* setup switch */
{  
  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense(); // call switch_update_interrupt_sense
  led_update();                    // call led_update
}

void
switch_interrupt_handler()
{
  char p1val = switch_update_interrupt_sense();
  switch_state_down = (p1val & SW1) ? 0 : 1; /* 0 when SW1 is up (not pressed) */

  if (!switch_state_down) { // count presses if button is released
    pressCount++;
    if (pressCount >= 9) {
      pressCount = 0;      // reset pressCount after 9 presses
    }
  }
  
  switch_state_changed = 1; // set switch_state_changed to be 1
  led_update(); // call led_update to update LED states based on new switch state
}
