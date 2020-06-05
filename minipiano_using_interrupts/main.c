// Lab13.c
// Runs on LM4F120 or TM4C123
// Uses SysTick interrupts to implement a 4-key digital piano
// edX Lab 13
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "DAC.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void piano_keys_to_sound(unsigned long piano_input);

unsigned long last_pressed = 0;


int main(void){
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
  // PortE used for piano keys, PortB used for DAC
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1){
		unsigned long piano_input = Piano_In();
		if (last_pressed != piano_input) {
			piano_keys_to_sound(piano_input);
		}
		last_pressed = piano_input;
	}
}

// Sound_freq = 1/(12.5ns*(period)*32)
void piano_keys_to_sound(unsigned long piano_input){
	switch(piano_input) {
		case 0x1:
				Sound_Tone(4778);
				break;
		case 0x2:
				Sound_Tone(4257);
				break;
		case 0x4:
				Sound_Tone(3792);
				break;
		case 0x8:
				Sound_Tone(3189);
				break;
		default:
				Sound_Off();
	}
}



