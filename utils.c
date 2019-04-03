// define which board we are using so the gpio package doesn't break everything
#define __MSP432P401R__



#include "utils.h"
#include "os.h"
//#include "msp432p401r_classic.h"
#include "msp432p401r.h"
#include "configurePin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* Example/Board Header files */
#include "Board.h"

//ping-pong queues
uint8_t queueNum;
uint8_t queueAddress[2];
int queue[2][20];

commandListener *baseCommand;

Queue *commandQueue;

/**
 * Set up GPIO pins needed for the team 6 robot
 *
 * @param none
 * @return void
 */
void GPIOInit() {
    /* Configure the LED pin */
    configurePin(1, 0, 0, _OUT, NO_INTERRUPT); // onboard red LED
    configurePin(2, 0, 0, _OUT, NO_INTERRUPT); // onboard RGB LED red pin
    configurePin(2, 1, 0, _OUT, NO_INTERRUPT); // onboard RGB LED green pin
    configurePin(2, 2, 0, _OUT, NO_INTERRUPT);// onboard RGB LED blue pin
    configurePin(2, 4, 0, _OUT, NO_INTERRUPT); // GPIO that controls the left motor direction
    configurePin(5, 6, 0, _OUT, NO_INTERRUPT); // GPIO that control the right motor direction

    //initialize all pins to off
    pinOff(1, 0);
    pinOff(2, 0);
    pinOff(2, 1);
    pinOff(2, 2);
    pinOff(2, 4);
    pinOff(5, 6);
}

/*
 * String compare so we didn't need to include string.h,
 * we wound up importing it later but we didn't want to
 * change all of our code
 *
 * @param a - pointer to initial string
 * @param b - pointer to string to compare to initial string
 * @return 1 - if matched strings
 * @return 0 - if strings don't match
 */
int equals(char *a, char *b) { // string compare for command interpreter
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0' && a[i] == b[i] && i < 11) { // ends if string is null-terminated or exceeds 11 characters (10 character command maximum)
        i++;
    }
    if(a[i] != b[i]) { // if the strings are not the same return false
        return 0;
    }
    return 1; // if the strings are the same return true
}

/**
 * Set up analog to digital conversion for the team 6 robot
 *
 * @param none
 * @return void
 */
/*
void ADCInit() {
    ADC_Params   params;

    adc0 = NULL;
    adc1 = NULL;
	//REWRITE
    ADC_Params_init(&params);
    params.isProtected = false;
    do {
        adc0 = ADC_open(Board_ADC9, &params);
    } while (adc0 == NULL);

    ADC_Params_init(&params);
    params.isProtected = false;
    do {
        adc1 = ADC_open(Board_ADC10, &params);
    } while (adc1 == NULL);
}*/

/**
 * Initialize PWM for the team 6 robot
 *
 * @param none
 * @return void
 */
 /*
void PWMInit() {
    // Period and duty in microseconds
    pwm0 = NULL;
    pwm1 = NULL;
    PWM_Params params;

    // Call driver init functions.
    PWM_init();
	//REWRITE
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = MOTORPERIOD;

    do {
        pwm0 = PWM_open(Board_PWM2, &params);
    } while (pwm0 == NULL);

    PWM_start(pwm0);
    do {
        pwm1 = PWM_open(Board_PWM3, &params);
    } while (pwm1 == NULL);

    PWM_start(pwm1);
}*/

/**
 * Initialize UART for the team 6 robot
 *
 * @param none
 * @return void
 */
void UARTInit() {
		configurePin(3,2,1,_IN,NO_INTERRUPT); // Pin 3.2 to RX
		configurePin(3,3,1,_OUT,NO_INTERRUPT); // Pin 3.3 to TX
		
		UCA2CTLW0 = 0x0001; // hold logic
		
		// bit15=0,      no parity bits
		// bit14=x,      not used when parity is disabled
		// bit13=0,      LSB first
		// bit12=0,      8-bit data length
		// bit11=0,      1 stop bit
		// bits10-8=000, asynchronous UART mode
		// bits7-6=11,   clock source to SMCLK
		// bit5=0,       reject erroneous characters and do not set flag
		// bit4=0,       do not set flag for break characters
		// bit3=0,       not dormant
		// bit2=0,       transmit data, not address (not used here)
		// bit1=0,       do not transmit break (not used here)
		// bit0=1,       hold logic in reset state while configuring
		
		UCA2CTLW0 = 0x00C1;	// set control
		
		//Use to determine Buad settings ->>> http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html//
		UCA2BRW = 26;	//assume CLK=3Mhz and BaudRate=115200 (CLK specified in startup)
		UCA2MCTLW &= ~0xFFF1;
		
		UCA2IE &= ~0x000F; // disable interrupts
		UCA2CTLW0 &= ~0x0001; // resume logic
		
}


/*
 * String length function so we didn't need to include string.h,
 * we wound up importing it later but we didn't want to
 * change all of our code
 *
 * @param a - pointer to string to get length of
 * @return int - size of string
 */
int length(char *a) {
    int i = 0;
    while (a[i] != '\0') {
        i++;
    }
    return i;
}

/**
 * Function to simplify writing to the UART, allows us to easily write one character.
 *
 * @param a - character to be written
 * @return void
 */
void putChar(char a) {
    while((UCA2IFG&0x0002)==0) {} //Wait for TXBUF to be empty
		UCA2TXBUF = a;
		UCA2IFG &= ~0002;
}

/**
 * Function to simplify writing to the UART, allows us to easily write a full string.
 *
 * @param a - pointer to string being written to UART
 * @return void
 */
void putString(char *a) {
	 while(*a != '\0') {
		 putChar(*a);
		 a++;
	 }
}

/**
 * Function to allow us to easily read one character from the UART
 *
 * @param void
 * @return char - return char from UART RXBuffer
 */
char getChar() {
    while((UCA2IFG&0x0001)==0) {} //Wait for RXBUF to be full
		UCA2IFG &= ~0001;
		return ((char)UCA2RXBUF);
}

/**
 * Adds a command to our command list which is a linked list to allow for greater expandability
 *
 * @param function - a pointer to the function to run when a command is ran
 * @param command - a string that contains the function name
 * @return void
 */
void registerCommand(void (*function)(void), char command[COMMANDLENGTH + 1]) {
    int i = 0;
    commandListener *current;
    if(baseCommand != NULL) {
        current = baseCommand;
        while(current->next != NULL) {
            i++;
            current = current->next;
        }
        current->next = (commandListener *) malloc(sizeof(commandListener));
        current = current->next;
    } else {
        baseCommand = (commandListener *) malloc(sizeof(commandListener));
        current = baseCommand;
    }
    current->func = function;
    current->next = NULL;
    i = 0;
    while(i < length(command) + 1) {
        current->command[i] = command[i];
        i++;
    }
}

/**
 * Cycles through out linked-list in an attempt to find the command requested
 *
 * @param command - a string with the command to be resolved
 * @return -1 - on unknown command
 * @return int - the command number to allow the command to be ran using that number
 */
int resolveCommand(char command[COMMANDLENGTH + 1]) {
    int i = 0;
    commandListener *current = baseCommand;
    while(!equals(current->command, command)) {
        if(current->next == NULL) {
            return -1;
        }
        current = current->next;
        i++;
    }
    return i;
}

/**
 * A function to run the command requested
 *
 * @param a - the number of the command requested
 * @return void
 */
void runCommand(int a) {
    int i;
    commandListener *current = baseCommand;
    for(i = 0; i < a; i++) {
        current = current->next;
    }
    current->func();
}


/**
   * Initializes a queue
   *
   * @param *a - The queue passed by reference
   * @return void - the queue gets initialized and can be used from there by reference.
   */
void fifo_init(Queue *a) {
    a->size = 0;
    a->getI = 0;
    a->putI = 0;
}

/**
   * Adds an integer to the back of the queue. If the FIFO is full, returns -1
   *
   * @param *q - The queue passed by reference
   * @param *a - The integer to be written to the queue passed by reference
   * @return int - error status as an integer. returns 0 if it succeeded and -1 if there was an error
   */
int fifo_put(Queue *q, int *a) {
    if(q->size == FIFOSIZE) {               // if fifo is full, tell the bluetooth task it failed.
        return -1;
    } else {
        q->queue[q->putI] = *a;             // put command number in fifo
        q->putI = (q->putI + 1) % FIFOSIZE; // increment put pointer, also wraps around to to make fifo circular
        q->size += 1;                       // increment fifo size counter
        return 0;                           // return success
    }
}

/**
   * Returns the integer from the front of the queue.
   * If there is no data, it returns -1
   *
   * @param *q - The queue passed by reference
   * @return int - the number from the front of the queue
   */
int fifo_get(Queue *q) {
    int data;
    if(q->size == 0) {                      // return failure if fifo is empty
        return -1;
    } else {
        data = q->queue[q->getI];           // get from fifo
        q->getI = (q->getI + 1) % FIFOSIZE; // increment get pointer and wrap around to make fifo circular
        q->size -= 1;                       // decrement fifo size counter
        return data;                        // return data
    }
}
/*
void actuator(int pwmIncrement) {
    if(pwmIncrement < 0) {
        if(left < 10000) {  //
            left -= pwmIncrement;
            right = 10000;
        } else {
            right += pwmIncrement;
        }
    } else if(pwmIncrement > 0) {
        if(right < 10000) {
            right += pwmIncrement;
            left = 10000;
        } else {
            left -= pwmIncrement;
        }
    }
    if(right < 2000) right = 2000;
    if(right > 10000) right = 10000;
    if(left < 2000) left = 2000;
    if(left > 10000) left = 10000;
    setDuty(0, left);
    setDuty(1, right);
}


void setDuty(int num, int duty) {
	//REWRITE
    if (duty > MOTORPERIOD) {
        duty = MOTORPERIOD;
    } else if (duty < -MOTORPERIOD) {
        duty = -MOTORPERIOD;
    }
    if (num == 0) {
        if (duty < 0) {
            pinOff(2, 4);
            PWM_setDuty(pwm0, -duty);
        } else {
            pinOn(2, 4);
            PWM_setDuty(pwm0, duty);
        }
    } else if (num == 1) {
        if (duty < 0) {
            pinOff(5, 6);
            PWM_setDuty(pwm1, -duty);
        } else {
            pinOn(5, 6);
            PWM_setDuty(pwm1, duty);
        }
    }
}


void printTime(void) {
    uint32_t minutes;
    uint32_t seconds;
    uint32_t milliSeconds;
    char a[15];
    milliSeconds = runTime * 50;
    seconds = milliSeconds / 1000;
    milliSeconds -= seconds * 1000;
    minutes = seconds / 60;
    seconds -= minutes * 60;
    sprintf(a, "%lu:%lu.%lu\n\r", minutes, seconds, milliSeconds);
    putString(a);
}
*/

/**
	* Clock initializer fcn taken from Valvano's BSP
	*
	* @param none
	* @return none
	*
	*/
uint32_t Prewait = 0;                   // loops between BSP_Clock_InitFastest() called and PCM idle (expect 0)
uint32_t CPMwait = 0;                   // loops between Power Active Mode Request and Current Power Mode matching requested mode (expect small)
uint32_t Postwait = 0;                  // loops between Current Power Mode matching requested mode and PCM module idle (expect about 0)
uint32_t IFlags = 0;                    // non-zero if transition is invalid
uint32_t Crystalstable = 0;             // loops before the crystal stabilizes (expect small)
void BSP_Clock_InitFastest(void){
  // wait for the PCMCTL0 and Clock System to be write-able by waiting for Power Control Manager to be idle
  while(PCMCTL1&0x00000100){
    Prewait = Prewait + 1;
    if(Prewait >= 100000){
      return;                           // time out error
    }
  }
  // request power active mode LDO VCORE1 to support the 48 MHz frequency
  PCMCTL0 = (PCMCTL0&~0xFFFF000F) |     // clear PCMKEY bit field and AMR bit field
            0x695A0000 |                // write the proper PCM key to unlock write access
            0x00000001;                 // request power active mode LDO VCORE1
  // check if the transition is invalid (see Figure 7-3 on p344 of datasheet)
  if(PCMIFG&0x00000004){
    IFlags = PCMIFG;                    // bit 2 set on active mode transition invalid; bits 1-0 are for LPM-related errors; bit 6 is for DC-DC-related error
    PCMCLRIFG = 0x00000004;             // clear the transition invalid flag
    // to do: look at CPM bit field in PCMCTL0, figure out what mode you're in, and step through the chart to transition to the mode you want
    // or be lazy and do nothing; this should work out of reset at least, but it WILL NOT work if Clock_Int32kHz() or Clock_InitLowPower() has been called
    return;
  }
  // wait for the CPM (Current Power Mode) bit field to reflect a change to active mode LDO VCORE1
  while((PCMCTL0&0x00003F00) != 0x00000100){
    CPMwait = CPMwait + 1;
    if(CPMwait >= 500000){
      return;                           // time out error
    }
  }
  // wait for the PCMCTL0 and Clock System to be write-able by waiting for Power Control Manager to be idle
  while(PCMCTL1&0x00000100){
    Postwait = Postwait + 1;
    if(Postwait >= 100000){
      return;                           // time out error
    }
  }
  // initialize PJ.3 and PJ.2 and make them HFXT (PJ.3 built-in 48 MHz crystal out; PJ.2 built-in 48 MHz crystal in)
  PJSEL0 |= 0x0C;
  PJSEL1 &= ~0x0C;                      // configure built-in 48 MHz crystal for HFXT operation
//  PJDIR |= 0x08;                        // make PJ.3 HFXTOUT (unnecessary)
//  PJDIR &= ~0x04;                       // make PJ.2 HFXTIN (unnecessary)
  CSKEY = 0x695A;                       // unlock CS module for register access
  CSCTL2 = (CSCTL2&~0x00700000) |       // clear HFXTFREQ bit field
           0x00600000 |                 // configure for 48 MHz external crystal
           0x00010000 |                 // HFXT oscillator drive selection for crystals >4 MHz
           0x01000000;                  // enable HFXT
  CSCTL2 &= ~0x02000000;                // disable high-frequency crystal bypass
  // wait for the HFXT clock to stabilize
  while(CSIFG&0x00000002){
    CSCLRIFG = 0x00000002;              // clear the HFXT oscillator interrupt flag
    Crystalstable = Crystalstable + 1;
    if(Crystalstable > 100000){
      return;                           // time out error
    }
  }
  // configure for 2 wait states (minimum for 48 MHz operation) for flash Bank 0
  FLCTL_BANK0_RDCTL = (FLCTL_BANK0_RDCTL&~0x0000F000)|FLCTL_BANK0_RDCTL_WAIT_2;
  // configure for 2 wait states (minimum for 48 MHz operation) for flash Bank 1
  FLCTL_BANK1_RDCTL = (FLCTL_BANK1_RDCTL&~0x0000F000)|FLCTL_BANK1_RDCTL_WAIT_2;
  CSCTL1 = 0x20000000 |                 // configure for SMCLK divider /4
           0x00100000 |                 // configure for HSMCLK divider /2
           0x00000200 |                 // configure for ACLK sourced from REFOCLK
           0x00000050 |                 // configure for SMCLK and HSMCLK sourced from HFXTCLK
           0x00000005;                  // configure for MCLK sourced from HFXTCLK
  CSKEY = 0;                            // lock CS module from unintended access
  //ClockFrequency = 48000000;
  //SubsystemFrequency = 12000000;
}
