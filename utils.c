// define which board we are using so the gpio package doesn't break everything
#define __MSP432P401R__


#include <stdlib.h>
#include "utils.h"
#include "os.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>


/* Example/Board Header files */
#include "Board.h"

/**
 * Set up GPIO pins needed for the team 6 robot
 *
 * @param none
 * @return void
 */
void GPIOInit() {
    /* Configure the LED pin */
	//REWRITE
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // onboard red LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0); // onboard RGB LED red pin
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1); // onboard RGB LED green pin
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2); // onboard RGB LED blue pin
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4); // GPIO that controls the left motor direction
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6); // GPIO that control the right motor direction

    //initialize all pins to off
	//REWRITE
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
}

/**
 * Initialize PWM for the team 6 robot
 *
 * @param none
 * @return void
 */
void PWMInit() {
    /* Period and duty in microseconds */
    pwm0 = NULL;
    pwm1 = NULL;
    PWM_Params params;

    /* Call driver init functions. */
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
}

/**
 * Initialize UART for the team 6 robot
 *
 * @param none
 * @return void
 */
void UARTInit() {
    // uart initialization
    UART_Params uartParams;
    uart = NULL;
	//REWRITE
    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    // sets global uart variable
    do {
        uart = UART_open(Board_UART1, &uartParams); // set to uart0 for usb or uart1 for bluetooth (helpful for setting the system up)
    } while(uart == NULL);
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
	//REWRITE
    UART_write(uart, &a, 1);
}

/**
 * Function to simplify writing to the UART, allows us to easily write a full string.
 *
 * @param a - string to be writtento UART
 * @return void
 */
void putString(char *a) {
	//REWRITE
    UART_write(uart, a, length(a));
}

/**
 * Function to allow us to easily read one character from the UART
 *
 * @param out - pointer to char to output string to
 * @return void - outputs through pointers
 */
void getChar(char *out) {
	//REWRITE
    UART_read(uart, out, 1);
}

/**
 * Function to simplify pin toggling, we recognized that the Ti RTOS toggles use pre-defined
 * values and saw how to convert from a port/pin number to those values to make writing to pins
 * look nicer
 *
 * @param port - the port of the pin you want to change
 * @param pin - the pin number of the pin you want to change
 * @return void
 */
void pinToggle(int port, int pin) {
	//REWRITE
    GPIO_toggleOutputOnPin(port, 1 << pin);
}


/**
 * Function to simplify turning on pins, we recognized that the Ti RTOS toggles use pre-defined
 * values and saw how to convert from a port/pin number to those values to make writing to pins
 * look nicer
 *
 * @param port - the port of the pin you want to change
 * @param pin - the pin number of the pin you want to change
 * @return void
 */
void pinOff(int port, int pin) {
	//REWRITE
    GPIO_setOutputLowOnPin(port, 1 << pin);
}


/**
 * Function to simplify turning off pins, we recognized that the Ti RTOS toggles use pre-defined
 * values and saw how to convert from a port/pin number to those values to make writing to pins
 * look nicer
 *
 * @param port - the port of the pin you want to change
 * @param pin - the pin number of the pin you want to change
 * @return void
 */
void pinOn(int port, int pin) {
	//REWRITE
    GPIO_setOutputHighOnPin(port, 1 << pin);
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

/**
   * Sets the PWM value that was sent from the PID calculation
   *
   * @param pwmIncrement - comes from the PID calculation.
   * @return void - calls the actuator function to set the PWM value before returning
   */
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

/**
   * Sets the duty cycle
   *
   * @param num - decides whether to control left or right 0 = left 1  = right
   * @param duty - PWM value to be set to the actuators
   * @return void - calls the actuator function to set the PWM value before returning
   */
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

/**
 * Prints the amount of time the system ran in the format minutes:seconds.milliseconds
 *
 * @param none
 * @return none - outputs on UART terminal
 */
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
