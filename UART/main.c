// main.c
// Runs on LM4F120/TM4C123


// this connection occurs in the USB debugging cable
// U0Rx (PA0) connected to serial port on PC
// U0Tx (PA1) connected to serial port on PC
// Ground connected ground in the USB cable

#include "UART.h"
#include "TExaS.h"


void EnableInterrupts(void);  // Enable interrupts


int main(void){ unsigned long n;
  TExaS_Init();             // set system clock to 80 MHz
  UART_Init();              // initialize UART

  UART_OutString("Running Lab 11");

  while(1){
    UART_OutString("\n\rInput:");
    n = UART_InUDec();
    UART_OutString(" UART_OutUDec = ");
    UART_OutUDec(n);     // your function
    UART_OutString(",  UART_OutDistance ~ ");
    UART_OutDistance(n); // your function
  }
}

