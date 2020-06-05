// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

#include <stdio.h> 
#include <assert.h>

// **************DAC_Init*********************
// Initializes 4-bit DAC
// Input: none
// Output: none
void DAC_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000002; 		        // 1) activate clock for port B
	delay = SYSCTL_RCGC2_R; 					// allow time for clock to start
	GPIO_PORTB_AMSEL_R &= ~0x0F; 			    // 2) disable analog on PB0-3
	GPIO_PORTB_PCTL_R &= ~0x0000FFFF;           // 3) PCTL GPIO on PB0-3
	GPIO_PORTB_DIR_R |= 0x0F;					// 4) direction PB0-3 output
	GPIO_PORTB_AFSEL_R &= ~0x0F; 			    // 5) disable alternative on PB0-3
	GPIO_PORTB_DEN_R |= 0x0F;					// 6) enable digital on PB0-3
	GPIO_PORTB_DR8R_R |= 0x0F;				    // 7) set drive strength 8mA on PB0-3
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
	assert(data < 16);
  GPIO_PORTB_DATA_R = data;
}
