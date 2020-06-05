// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123

// Uses SysTick interrupts to create a squarewave at 440Hz.
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
// connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//



#include "TExaS.h"
#include "..//tm4c123gh6pm.h"


// basic functions defined at the end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode


// global variables declaration
char switch_was_pressed  = 0;
char is_toggling = 0;


// Initialization of ports: input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ volatile unsigned long delay;
	// SysTick interrupts initialization
	NVIC_ST_CTRL_R = 0; 			// 0.1) disable SysTick during setup
	NVIC_ST_RELOAD_R = 90908;       // 0.2) reload value = period - 1 = 80 MHz/ 880Hz = 90909 - 1
	NVIC_ST_CURRENT_R = 0; 		    // 0.3) any write to CURRENT clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF)|0x10000000; // 0.4) set priority 1
	NVIC_ST_CTRL_R = 0x07; 		    // 0.5) enable SysTick with core clocks and interrupts
	
	// Regular port initialization
	SYSCTL_RCGC2_R |= 0x00000001;       // 1) Activate clock for port A
	delay = SYSCTL_RCGC2_R;			    // allow time fot clock to start
									    // 2) no need to unlock GPIO for port A
	GPIO_PORTA_AMSEL_R &= ~0x0C;	    // 3) disable analog on PA2 and PA3
	GPIO_PORTA_PCTL_R &= ~0x0000FF00;   // 4) PCTL GPIO on PA2 and PA3
	GPIO_PORTA_DIR_R = (0xF3 & GPIO_PORTA_DIR_R) | 0x04; // 5) input from PA3, ootput to PA2
	GPIO_PORTA_AFSEL_R &= ~0x0C;	    // 6) disable alternative function on PA2 and PA3
	GPIO_PORTA_DEN_R |= 0x0C;		    // 7) enable PA2 and PA3 as digital ports
	GPIO_PORTA_PDR_R |= 0x08;			// 8) activate pull-down on the input switch
} 


// called at 880 Hz
void SysTick_Handler(void){
	char now_pressed = GPIO_PORTA_DATA_R & 0x08;
	
	if (now_pressed && !switch_was_pressed){
		if (!is_toggling)
			is_toggling  = 1;
		else
			is_toggling = 0;
	}	
	 	
	if (is_toggling) 
		GPIO_PORTA_DATA_R ^= 0x04;
	else
		GPIO_PORTA_DATA_R &= 0xFC; // turn off the speaker	
	
	switch_was_pressed = now_pressed;
}

int main(void){
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); // set system clock to 80 MHz
  Sound_Init();         
  EnableInterrupts();   // enables interrupts after all initialization are done
  while(1){
  }
}
