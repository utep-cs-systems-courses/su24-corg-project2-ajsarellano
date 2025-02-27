//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;          // turn off green LED
  P1OUT &= ~LED_RED;             // turn off red LED

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

void greenControl(int on)     // Turn green LED on or off based on the value of 'on' 
{
  if (on) {
    P1OUT |= LED_GREEN;       //on = 1, turn on green LED
  } else {
    P1OUT &= ~LED_GREEN;      //on = 0, turn off green LED
  }
}

void redControl(int on)       // Turn red LED on or off based on the value of 'on'
{
  if (on) {
    P1OUT |= LED_RED;
  } else {
    P1OUT &= ~LED_RED;
  }
}

// green LED blink state machine
static int blinkLimitG = 5;   //  state var representing reciprocal of duty cycle 
void blinkUpdateG() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  static int blinkCountG = 0; // state var representing blink state
  blinkCountG ++;
  if (blinkCountG >= blinkLimitG) {
    blinkCountG = 0;
    greenControl(1);
  } else
    greenControl(0);
}

// red LED blink state machine
static int blinkLimitR = 3;
void blinkUpdateR() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  static int blinkCountR = 0; // state var representing blink state
  blinkCountR ++;
  if (blinkCountR >= blinkLimitR) {
    blinkCountR = 0;
    redControl(1);
  } else {
    redControl(0);
  }
}

void oncePerSecond() // repeatedly start bright and gradually lower duty cycle, one step/sec
{
  blinkLimitG ++;  // reduce duty cycle
  if (blinkLimitG >= 8)  // but don't let duty cycle go below 1/7.
    blinkLimitG = 0;

  blinkLimitR ++;  // reduce duty cycle
  if (blinkLimitR >= 6) // but don't let duty cycle go below 1/5.
    blinkLimitR = 0;
}

void secondUpdate()  // called every 1/250 sec to call oncePerSecond once per second
{
  static int secondCount = 0; // state variable representing repeating time 0…1s
  secondCount ++;
  if (secondCount >= 250) { // once each second
    secondCount = 0;
    oncePerSecond();
  }
}

void timeAdvStateMachines() // called every 1/250 sec
{
  blinkUpdateG();
  blinkUpdateR();
  secondUpdate();
}


void __interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking   
  timeAdvStateMachines();
} 

