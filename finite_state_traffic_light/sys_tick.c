<<<<<<< HEAD:finite_state_traffic_light/sys_tick.c
#include "sys_tick.h"
#include "tm4c123gh6pm.h"

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0; 							// 1)disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;		// 2)max reload value
	NVIC_ST_CURRENT_R = 0;						// 3)any write to current value cleans it 
	NVIC_ST_CTRL_R = 0x00000005;			// 4)enable systick core clock
}

void delay_msecs(unsigned long msecs){
	while (msecs>0){
		NVIC_ST_RELOAD_R = 80000 - 1;
		NVIC_ST_CURRENT_R = 0;
		while((NVIC_ST_CTRL_R & 0x00010000) == 0){}
		msecs -= 1;
	}
}
=======
#include "sys_tick.h"
#include "tm4c123gh6pm.h"

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0; 			// 1)disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;		// 2)max reload value
	NVIC_ST_CURRENT_R = 0;			// 3)any write to current value cleans it 
	NVIC_ST_CTRL_R = 0x00000005;		// 4)enable systick core clock
}

void delay_msecs(unsigned long msecs){
	while (msecs>0){
		NVIC_ST_RELOAD_R = 80000 - 1;
		NVIC_ST_CURRENT_R = 0;
		while((NVIC_ST_CTRL_R & 0x00010000) == 0){}
		msecs -= 1;
	}
}
>>>>>>> 5552bf336242f574d50eac4d5e66e04bfc62ec91:sys_tick.c
