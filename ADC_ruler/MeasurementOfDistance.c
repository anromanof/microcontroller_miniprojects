// MeasurementOfDistance.c
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to periodically initiate a software-
// triggered ADC conversion, convert the sample to a fixed-
// point decimal distance, and store the result in a mailbox.
// The foreground thread takes the result from the mailbox,
// converts the result to a string, and prints it to the
// Nokia5110 LCD.


// Slide pot pin 3 connected to +3.3V
// Slide pot pin 2 connected to PE2(Ain1) and PD3
// Slide pot pin 1 connected to ground


#include "ADC.h"
#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void EnableInterrupts(void);  // Enable interrupts

unsigned char String[10]; // null-terminated ASCII string
unsigned long Distance;   // units 0.001 cm
unsigned long ADCdata;    // 12-bit 0 to 4095 sample
unsigned long Flag = 0;       // 1 means valid Distance, 0 means Distance is empty


//********Convert****************
// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.  
// Overflow and dropout should be considered 
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
unsigned long Convert(unsigned long sample){
  return (2879*sample)/4095 + 86;  // replace this line with real code
}


// Initialize SysTick interrupts to trigger at 40 Hz, 25 ms
// Input: period - number of 12.5 ns in this period
void SysTick_Init(unsigned long period){
	NVIC_ST_CTRL_R = 0; 			        // 0.1) disable SysTick during setup
	NVIC_ST_RELOAD_R = period-1;            // 0.2) reload value = period - 1
	NVIC_ST_CURRENT_R = 0; 		            // 0.3) any write to CURRENT clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF)|0x10000000; // 0.4) set priority 1
	NVIC_ST_CTRL_R = 0x07; 		            // 0.5) enable SysTick with core clocks and interrupts
}


// executes every 25 ms, collects a sample, converts and stores in mailbox
void SysTick_Handler(void){ 
	ADCdata = ADC0_In();
	Distance = Convert(ADCdata);
	Flag = 1;
}



//-----------------------UART_ConvertDistance-----------------------
// Converts a 32-bit distance into an ASCII string
// Input: 32-bit number to be converted (resolution 0.001cm)
// Output: store the conversion in global variable String[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//    4 to "0.004 cm"  
//   31 to "0.031 cm" 
//  102 to "0.102 cm" 
// 2210 to "2.210 cm"
//10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void UART_ConvertDistance(unsigned long n){
	short i = 3;
		if (n >= 10000) {
			strcpy(String, "*.*** cm");
	} else {
		strcpy(String, "0.000 cm");
		while (n != 0) {
			unsigned short remainder = n % 10;
			n = n / 10;
			if (i > 0) {
				String[i+1] = '0' + remainder;
			} else {
				String[i] = '0' + remainder;
			}
			i -= 1;
		}
	}	
}


int main(void){ 
  volatile unsigned long delay;
  TExaS_Init(ADC0_AIN1_PIN_PE2, SSI0_Real_Nokia5110_Scope); //set the clock to 80// MHz
  ADC0_Init();              // initialize ADC0, channel 1, sequencer 3
  Nokia5110_Init();         // initialize Nokia5110 LCD
  SysTick_Init(2000000);    //80 MHz / 40Hz
  EnableInterrupts();
  Nokia5110_Clear();
  Nokia5110_OutString("Welcome!");
  while(1){ 
		Flag = 0;
		while(!Flag);
		UART_ConvertDistance(Distance);
		Nokia5110_SetCursor(0, 1);
		Nokia5110_OutString(String);
  }
}
