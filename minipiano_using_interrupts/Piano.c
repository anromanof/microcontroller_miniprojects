// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano


// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "..//tm4c123gh6pm.h"


// **************Piano_Init*********************
// Initializes piano key inputs
// Input: none
// Output: none
void Piano_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010; 		    // 1) activate clock for port E
	delay = SYSCTL_RCGC2_R; 				// allow time for clock to start
	GPIO_PORTE_AMSEL_R &= ~0x0F; 			// 2) disable analog on PE0-3
	GPIO_PORTE_PCTL_R &= ~0x0000FFFF;       // 3) PCTL GPIO on PE0-3
	GPIO_PORTE_DIR_R &= ~0x0F;				// 4) direction PE0-3 input
	GPIO_PORTE_AFSEL_R &= ~0x0F; 			// 5) disable alternative on PE0-3
	GPIO_PORTE_DEN_R |= 0x0F;				// 6) enable digital on PE0-3
}

// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 8 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
  switch(GPIO_PORTE_DATA_R & 0xF) {
		case 0x1:
				return 0x01;
		case 0x2:
				return 0x02;
		case 0x4:
				return 0x04;
		case 0x8:
				return 0x08;
		default:
				return 0;
	}
}
