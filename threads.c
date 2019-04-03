#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "commands.h"


/* BIOS Header files */


/**
 * This function was a keep-alive function but it was from the example code,
 * we aren't able to remove it though and it seems to do nothing
 *
 * @param none
 * @return void
 */
void fxn0() {
    while(1) {

    }
}

/**
 * Keep alive function to make sure the RTOS never shuts off or
 * hard faults due to all threads being blocked
 *
 * @param arg0 set in the config file, unused
 * @param arg1 set in the config file, unused
 * @return void
 */
void life(UArg arg0, UArg arg1) { // keep alive function
    while(1) {
        Task_sleep(1);
    }
}

/**
 * Heartbeat task to blink LED1 approximately every second
 *
 * @param arg0 set in config, amount of time to sleep
 * @param arg1 set in config, unused
 * @return void
 */
void heartbeat(UArg arg0, UArg arg1) {
    while (1) {
        pinToggle(1,0);
        Task_sleep(arg0);
    }
}

/**
   * SWI which runs the command corresponding to the user input,
   * commands are passed to this function via a FIFO queue
   *
   * @param none
   * @return void - calls the command after finishing
   */
void runCommands(void) {// run commands from queue
    int temp;
    temp = fifo_get(commandQueue); // get command number from queue
    if(temp == -1) { // if fifo is empty do nothing, this may be able to be optimized by using blocking semaphores
    } else {
        runCommand(temp);           // if command queue isn't empty, run command
    }
}

/**
   * Thread takes input from UART, processes the commands inputted,
   * and some things to the terminal what the user sees when typing in commands
   *
   * @param none
   * @return void
   */
void bluetooth(UArg arg0, UArg arg1) {    // get commands from bluetooth UART
    char        input;
    char        in[11];                   // 11 characters so a 10-character string can be null-terminated
    int i, temp;
    while (1) {
        i = -1;                           // set to -1 to make the do while loop work better
        putString("\r\n");
        do {
            input = getChar();              // read input
            putChar(input);               // print input (uart doesn't do that by default)
            if(input == '\b' && i >= 0) { // if input is backspace, move string position back 1 (-2 + 1) to allow it to overwrite the last character inputted
                putString(" \b");         // put a space in place of the last character so it actually goes away then backspace again so the cursor is in the right spot
                i--;                      // move backward in string
            } else if(input == '\b') {    // keep user from backspacing into the prompt and keep i from going below 0 (string[-1] == death or something)
                i = -1;
            } else {                      // if input isn't a backspace, write character to the string
                i++;
                in[i] = input;
            }
        } while ((input != '\r') && (i < 10)); // keeps commands under 10 characters and ends command entry when user presses enter (\r = carriage return)
        in[i] = '\0'; // null-terminate the string
        temp = resolveCommand(in);             // resolve command string to command number
        if (temp != -1) {                      // if it could be resolved, put it in command queue
            i = fifo_put(commandQueue, &temp);
            Swi_post(commandsSWI);
            if(i == -1)
                putString("FIFO full");   // prints command if the fifo is full
        } else {
            putString("\n\r");            // if command couldn't be resolved, go to new line
            putString("Unknown command"); // then output command unknown
        }
    }
}

/**
 * HWI which posts a SWI to run our PID calculation
 *
 * @param none
 * @return none
 */
void peidiDC() {
    Swi_post(pidSWI);
}

/**
   * SWI used to follow the wall,
   * calculates the PWM value to send to the actuator using a PID calculation
   *
   * @param none
   * @return void - calls the actuator function to set the PWM value before returning
   */
void calculatePid(void) {
    uint16_t R;             // right sensor
    uint16_t F;             // front sensor
    int E;                  // error for calculating PID
    int U;                  // for storing the final PID value
    runTime++;              // pidH controls whether or not PID is running or not(found in command.c - start()/stop())
    ADC_convert(adc0, &R);  // get the right sensors reading
    ADC_convert(adc1, &F);  // get the front sensors reading
    if(F > front) {
        setDuty(0, 10000);
        setDuty(1, 10000);
        pinOff(2, 4);
        pinOn(5, 6);
        front = 8000;
        lF = 1;
    } else {
        if(lF) {
            pinOn(2, 4);
            lF = 0;
            front = 12000;
        }
        E = R - IDEAL;                  // calculate the error
        if(E < 300 && E > -300) {E = 0;}
        if(print) {
            two = (two + 1) % 2;
            //print stuff in here
            if(two == 1) {
                if(queueAddress[queueNum] == 20) {
                    queueNum = (queueNum + 1) % 2;
                    Semaphore_post(printSema);
                }
                queue[queueNum][queueAddress[queueNum]] = E;
                queueAddress[queueNum]++;
            }
        }
        P = kp * E / 10000;             // calculate P
        I = (ki * (E + lastE)) / 10000;        // calculate I
        //if(I > 1000) I = 1000;          // limit I
        //if(I < -1000) I = -1000;
        D = (kd * (E - lastE)) / 10000; // calculate D
        lastE = E;                      // update previous error
        U = P +  I + D;                 // final PID value
        actuator(U);                    // set the PID value
    }
}

/*
  * Task which initializes a timer and a GPIO port to use for the light sensors
  * and reads them to check for black lines.
  *
  * @param none
  * @return none
  */

void LightSensor(void) {
    // 1.Configure GPIO pin as output
    // 2.Set output to HIGH
    // 3.Allow at least 10[µs] for the sensor output voltage to rise
    // 4.Reconfigure GPIO pin as input
    // 5.Measure time elapsed until input is LOW
    // 6.Use the measured time to determine whether on black or white surface
    int last = 1; // if the last time this was ran the answer was black this will be a 0
    int countPin6 = 0;
    int counter = 0;
    int threshold = 700;
    while(1){
        GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);    // configure port 3 pin 6 as an output pin
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);//set the port 3 pin 6 to GPIO_INPUT_PIN_HIGH
        //GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);    // configure port 3 pin 7 as an output pin
        //GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);//set the port 3 pin 7 to GPIO_INPUT_PIN_HIGH
        //start a timer
        for(int i = 0; i < 1000; i++) {}
        GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN1);     // reconfigure this to an input pin
        //GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN7);     // reconfigure this to an input pin
        countPin6 = 0;
        //countPin7 = 0;
        while(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN1) != 0) {
            countPin6 +=  GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN1);
            //countPin7 +=  GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7);
        }                      // read from port 3 pin 6 and 7
        //sprintf(a, "countPin6 %d \n\r", countPin6); // then output command unknown
        if(countPin6 > threshold && last) {
            //putString("Black");
            pinToggle(2, 2);
            last = 0;
            counter++;
        } else if(countPin6 < threshold && last == 0) {
            if(counter > 3) {
                stop();
                queueNum = (queueNum + 1) % 2;
                Semaphore_post(printSema);
                Task_sleep(25);
                printTime();
            } else {
                print = (print + 1) % 2;
            }
            counter = 0;
            last = 1;
        } else if(countPin6 > threshold) counter++;

        // pass the time to a function that determines if the thing we just saw was black or white
        Task_sleep(25);
    }
}

/*
  * Task used to print the ping-pong queues,
  * pends on a semaphore so it only prints when it needs to.
  *
  * @param none
  * @return none
  */
void printQueues() {
    queueNum = 0;
    queueAddress[0] = 0;
    queueAddress[1] = 0;
    uint8_t temp;
    int i;
    char a[10];
    while (1) {
        Semaphore_pend(printSema, BIOS_WAIT_FOREVER);
        pinOff(2, 2);
        pinOn(2, 1);
        //putString("-------------------\n\r");
        temp = (queueNum + 1) % 2;
        for (i = 0; i < queueAddress[temp]; i++) {
            sprintf(a, "%d\n\r", queue[temp][i]);
            putString(a);
        }
        queueAddress[temp] = 0;
        Task_sleep(0);
        pinOff(2, 1);
        pinOn(2, 2);
    }
}
