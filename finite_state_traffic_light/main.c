<<<<<<< HEAD:finite_state_traffic_light/main.c
// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// anromanof
// January 5, 2020

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "sys_tick.h"

// ***** 2. Global Declarations Section *****
	
#define SENSOR 			(*((volatile unsigned long*)0x4002401C))
#define VEH_LIGHT 		(*((volatile unsigned long*)0x400050FC))
#define PED_LIGHT 		(*((volatile unsigned long*)0x40025028))
	
// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Ports_Init(void);


// Linked Data structure
struct State {
	unsigned long Veh_Out; // 16 bit Output for vehicles
	unsigned long Ped_Out; // 16 bit Output for pedestrians
	unsigned long Delay;   // SysTick delay in msecs
	unsigned long Next[8]; // Next states of the FSM
};

typedef const struct State STyp;

#define GoWest 0
#define WaitWest 1
#define GoNorth 2
#define WaitNorth 3
#define GoPed 4
#define WaitPed_1 5
#define WaitPed_2 6
#define WaitPed_3 7
#define WaitPed_4 8

STyp SFM[9] = {
	{0x0C, 0x2, 1000, {GoWest, GoWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest}},
	{0x14, 0x2, 500, {GoPed, GoPed, GoNorth, GoNorth, GoPed, GoPed, GoPed, GoPed}},
	{0x21, 0x2, 1000, {GoNorth, WaitNorth, GoNorth, WaitNorth, WaitNorth, WaitNorth, WaitNorth, WaitNorth}},
	{0x22, 0x2, 500, {GoWest, GoWest, GoWest, GoWest, GoPed, GoWest, GoPed, GoWest}},
	{0x24, 0x8, 1000, {GoPed, WaitPed_1, WaitPed_1, WaitPed_1, GoPed, WaitPed_1, WaitPed_1, WaitPed_1}},
	{0x24, 0x0, 500, {WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2}},
	{0x24, 0x2, 500, {WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3}},
	{0x24, 0x0, 500, {WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4}},
	{0x24, 0x2, 500, {GoNorth, GoWest, GoNorth, GoNorth, GoNorth, GoWest, GoNorth, GoNorth}}
};

unsigned long S;
unsigned long Input;

// ***** 3. Subroutines Section *****

int main(void){ 
    TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
    Ports_Init();
    SysTick_Init();
    S = GoNorth;

    EnableInterrupts();
    while(1){
        VEH_LIGHT = SFM[S].Veh_Out;     // Set lights for vehicles
        PED_LIGHT = SFM[S].Ped_Out;     // Set lights for pedestrians
        delay_msecs(SFM[S].Delay);      // Wait
        Input = SENSOR;
        S = SFM[S].Next[Input];         // Update the state
    }
}

void Ports_Init(void){volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x32; 				// 1)enable clock for PE,PB,PF
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_AMSEL_R = 0x00; 				// 2)disable analog on PF
	GPIO_PORTB_AMSEL_R = 0x00; 				// 	 disable analog on PB
	GPIO_PORTE_AMSEL_R = 0x00; 				// 	 disable analog on PE
	GPIO_PORTF_PCTL_R = 0x00000000; 	    // 3)PCTL GPIO PF
	GPIO_PORTB_PCTL_R = 0x00000000; 	    // 	 PCTL GPIO PB
	GPIO_PORTE_PCTL_R = 0x00000000; 	    // 	 PCTL GPIO PE
	GPIO_PORTF_DIR_R = 0x0A;				// 4)PF1,PF3 output
	GPIO_PORTB_DIR_R = 0x3F; 				//   PB0-5 output
	GPIO_PORTE_DIR_R = 0x00;  				//   PE0-2 input
	GPIO_PORTF_AFSEL_R = 0x00; 				// 5)disable alternative function on PF
	GPIO_PORTB_AFSEL_R = 0x00; 				// 	 disable alternative function on PB
	GPIO_PORTE_AFSEL_R = 0x00; 				// 	 disable alternative function on PE
	GPIO_PORTF_DEN_R = 0x0A;				// 6)PF1,PF3 digital I/O enable
	GPIO_PORTB_DEN_R = 0x3F; 				//   PB0-5 digital I/O enable
	GPIO_PORTE_DEN_R = 0x07;  				//   PE0-2 digital I/O enable
}

=======
// ***** 0. Documentation Section *****
// TableTrafficLight.c
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// anromanof
// January 5, 2020

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "sys_tick.h"

// ***** 2. Global Declarations Section *****
	
#define SENSOR 			(*((volatile unsigned long*)0x4002401C))
#define VEH_LIGHT 		(*((volatile unsigned long*)0x400050FC))
#define PED_LIGHT 		(*((volatile unsigned long*)0x40025028))
	
// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Ports_Init(void);


// Linked Data structure
struct State {
	unsigned long Veh_Out; // 8 bit Output for vehicles
	unsigned long Ped_Out; // 4 bit Output for pedestrians
	unsigned long Delay;   // SysTick delay in msecs
	unsigned long Next[8]; // Next states of the FSM
};

typedef const struct State STyp;

#define GoWest 0
#define WaitWest 1
#define GoNorth 2
#define WaitNorth 3
#define GoPed 4
#define WaitPed_1 5
#define WaitPed_2 6
#define WaitPed_3 7
#define WaitPed_4 8

STyp SFM[9] = {
	{0x0C, 0x2, 1000, {GoWest, GoWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest}},
	{0x14, 0x2, 500, {GoPed, GoPed, GoNorth, GoNorth, GoPed, GoPed, GoPed, GoPed}},
	{0x21, 0x2, 1000, {GoNorth, WaitNorth, GoNorth, WaitNorth, WaitNorth, WaitNorth, WaitNorth, WaitNorth}},
	{0x22, 0x2, 500, {GoWest, GoWest, GoWest, GoWest, GoPed, GoWest, GoPed, GoWest}},
	{0x24, 0x8, 1000, {GoPed, WaitPed_1, WaitPed_1, WaitPed_1, GoPed, WaitPed_1, WaitPed_1, WaitPed_1}},
	{0x24, 0x0, 500, {WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2, WaitPed_2}},
	{0x24, 0x2, 500, {WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3, WaitPed_3}},
	{0x24, 0x0, 500, {WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4, WaitPed_4}},
	{0x24, 0x2, 500, {GoNorth, GoWest, GoNorth, GoNorth, GoNorth, GoWest, GoNorth, GoNorth}}
};

unsigned long S;
unsigned long Input;

// ***** 3. Subroutines Section *****

int main(void){ 
    TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // set system clock to 80 MHz
    Ports_Init();
    SysTick_Init();
    S = GoNorth;

    EnableInterrupts();
    while(1){
        VEH_LIGHT = SFM[S].Veh_Out;     // Set lights for vehicles
        PED_LIGHT = SFM[S].Ped_Out;     // Set lights for pedestrians
        delay_msecs(SFM[S].Delay);      // Wait
        Input = SENSOR;
        S = SFM[S].Next[Input];         // Update the state
    }
}

void Ports_Init(void){volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x32; 				// 1)enable clock for PE,PB,PF
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_AMSEL_R &= ~0x0A; 				// 2)disable analog on PF
	GPIO_PORTB_AMSEL_R &= ~0x3F; 				//   disable analog on PB
	GPIO_PORTE_AMSEL_R &= ~0x07; 				//   disable analog on PE
	GPIO_PORTF_PCTL_R &= ~0x0000F0F0; 	    		// 3)PCTL GPIO PF
	GPIO_PORTB_PCTL_R &= ~0x000FFFFF; 	    		//   PCTL GPIO PB
	GPIO_PORTE_PCTL_R &= ~0x00000FFF; 	    		//   PCTL GPIO PE
	GPIO_PORTF_DIR_R |= 0x0A;				// 4)PF1,PF3 output
	GPIO_PORTB_DIR_R |= 0x3F; 				//   PB0-5 output
	GPIO_PORTE_DIR_R &= ~0x07;  				//   PE0-2 input
	GPIO_PORTF_AFSEL_R &= ~0x0A; 				// 5)disable alternative function on PF
	GPIO_PORTB_AFSEL_R &= ~0x3F; 				//   disable alternative function on PB
	GPIO_PORTE_AFSEL_R &= ~0x07; 				//   disable alternative function on PE
	GPIO_PORTF_DEN_R |= 0x0A;				// 6)PF1,PF3 digital I/O enable
	GPIO_PORTB_DEN_R |= 0x3F; 				//   PB0-5 digital I/O enable
	GPIO_PORTE_DEN_R |= 0x07;  				//   PE0-2 digital I/O enable
}

>>>>>>> 5552bf336242f574d50eac4d5e66e04bfc62ec91:TableTrafficLight.c
