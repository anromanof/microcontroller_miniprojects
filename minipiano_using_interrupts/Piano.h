// Piano.h
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano


// **************Piano_Init*********************
// Initializes piano key inputs
// Input: none
// Output: none
void Piano_Init(void); 
  

// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 1 to 8 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void);
