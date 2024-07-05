#include <msp430.h>
#include "led.h"
#include "switches.h"

/*led.c controls the LEDs based on state vars */

unsigned int pressCount = 0;

void led_init()
{
  P1DIR |= LEDS;		// bits attached to leds are output
  switch_state_changed = 1;     // set switch_state_changed to 1
  led_update();                 // call led_update
}

void led_update(){
  if (switch_state_changed) {   // check if switch state has changed
    char ledFlags = 0;          /* by default, no LEDs on */

    ledFlags |= switch_state_down ? LED_GREEN : 0; // (switch is pressed) turn on green LED
    ledFlags |= switch_state_down ? 0 : LED_RED;   // (switch is not pressed) turn on red LED

    P1OUT &= (0xff - LEDS) | ledFlags;   // clear bits for off leds
    P1OUT |= ledFlags;                   // set bits for on leds

    if (pressCount >= 0 && pressCount < 3) {
      P1OUT |= LED_RED;    // turn on red LED
    } else if (pressCount >= 3 && pressCount < 6) {
      P1OUT &= ~LED_RED;   // turn off red LED
    } else if (pressCount >= 6 && pressCount < 9) {
      P1OUT |= LED_GREEN;  // turn on both LEDs
      P1OUT |= LED_RED;
    }
    //P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    //P1OUT |= ledFlags;                 // set bits for on leds
  }
  switch_state_changed = 0;    // set switch_state_changed to 0
}

