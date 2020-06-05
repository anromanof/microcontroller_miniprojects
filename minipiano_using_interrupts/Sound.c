// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

unsigned short Index = 0;
const unsigned long SineWave[32] = {8, 9, 10, 12, 13, 14, 14, 15, 15, 15, 14, 14, 13, 12, 10, 9, 8, 6, 5, 3, 2, 1, 1, 0, 0, 0, 1, 1, 2, 3, 5, 6};
char on = 0;


// **************Sound_Init*********************
// Initializes Systick periodic interrupts with priority 2
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
	NVIC_ST_CTRL_R = 0; 					//disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF; 			// max reload value
	NVIC_ST_CURRENT_R = 0;					// any write to current clear it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF)|0x40000000;	
	NVIC_ST_CTRL_R = 0x07; 					// enable SysTick with core clocks
  DAC_Init();
}


// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Sound_freq = 1/(12.5ns*(period)*32)
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period){
	on = 1;
	NVIC_ST_RELOAD_R = period - 1; 			// reload value
	NVIC_ST_CURRENT_R = 0;					// any write to current clear it
	NVIC_ST_CTRL_R = 0x07; 					// enable SysTick with core clocks
}


// **************Sound_Off*********************
// stop outputting to DAC
// Output: none
void Sound_Off(void){
	on = 0;
	NVIC_ST_CTRL_R = 0x00; 					// disable SysTick 
	DAC_Out(0); 
}


// Interrupt service routine
// Executed every 12.5ns*(period)
// Sound_freq = 1/(12.5ns*(period)*32)
void SysTick_Handler(void){
	if (on){
		Index = (Index + 1) & 0x1F;
		DAC_Out(SineWave[Index]);		
	}
}
