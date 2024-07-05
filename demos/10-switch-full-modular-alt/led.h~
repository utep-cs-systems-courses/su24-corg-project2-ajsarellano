#ifndef led_included
#define  led_included

#include <msp430.h>

#define LED_GREEN BIT0          // P1.0
#define LED_RED BIT6            // P1.6
#define LEDS (BIT0 | BIT6)

void led_init();		/* initialize LEDs */
void led_update();		/* update LEDs */
void led_toggle();              /* changes LED based on pressCount*/

// these are boolean flags
extern unsigned char leds_changed, green_led_state, red_led_state;
extern unsigned int pressCount;   // keeps track of button presses

#endif
