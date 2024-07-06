#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

#define SW1 BIT3         /* switch1 is p1.3 */
#define SWITCHES SW1     /* only 1 switch on this board */

int currentTone = 1000;
int secondCount = 0;
int tone_state = 0;

void switch_init() {
  P1REN |= SWITCHES;     /* enables resistors for switches */
  P1IE |= SWITCHES;      /* enable interrupts from switches */
  P1OUT |= SWITCHES;     /* pull-ups for switches */
  P1DIR &= ~SWITCHES;    /* set switches' bits for input */
}

void
__interrupt_vec(PORT1_VECTOR) Port_1() {
  if (P1IFG & SWITCHES) {  // did a button cause this interrupt?
    if (currentTone == 1000) { //change tones
      currentTone = 2000;
    } else if (currentTone == 2000){
      currentTone = 3000;
    } else {
      currentTone = 1000;
    }

    buzzer_set_period(currentTone); // update new tone
    P1IFG &= ~SWITCHES;    // clear pending sw interrupts
  }
}

int main() {
  configureClocks();              // setup master osci
  enableWDTInterrupts();          // enable periodic interrupt

  buzzer_init();
  switch_init();

  buzzer_set_period(currentTone); /* start buzzing!!! 2MHz/2000 = 1kHz,  2MHz/1000 = 2kHz*/

  or_sr(0x18);                    // CPU off, GIE on
}

void
__interrupt_vec(WDT_VECTOR) WDT() //250 interrupts/sec
{
  if (secondCount >= 250) {
    secondCount = 0;
    if (tone_state == 0) {
      currentTone = 1000;
      tone_state++;
    } else if (tone_state == 1) {
      currentTone = 2000;
      tone_state++;
    } else if (tone_state == 2) {
      currentTone = 3000;
      tone_state = 0;
    }
    buzzer_set_period(currentTone);
  } else {
    secondCount++;
  }
}
