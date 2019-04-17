#include "utils.h"
/**
 * Set up GPIO pins needed for the team 6 robot
 *
 * @param none
 * @return void
 */
/*void GPIOInit() {
    /* Configure the LED pin */
    /*configurePinbyNum(1, 0, 0, OUT, NO_INTERRUPT); // onboard red LED
    configurePinbyNum(2, 0, 0, OUT, NO_INTERRUPT); // onboard RGB LED red pin
    configurePinbyNum(2, 1, 0, OUT, NO_INTERRUPT); // onboard RGB LED green pin
    configurePinbyNum(2, 2, 0, OUT, NO_INTERRUPT);// onboard RGB LED blue pin
    configurePinbyNum(2, 4, 0, OUT, NO_INTERRUPT); // GPIO that controls the left motor direction
    configurePinbyNum(5, 6, 0, OUT, NO_INTERRUPT); // GPIO that control the right motor direction

    //initialize all pins to off
  //REWRITE
    pinOff(1, 0);
    pinOff(2, 0);
    pinOff(2, 1);
    pinOff(2, 2);
    pinOff(2, 4);
    pinOff(5, 6);
}*/

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
/*void ADCInit() {
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
/*void PWMInit() {
    /* Period and duty in microseconds */
    /*pwm0 = NULL;
    pwm1 = NULL;
    PWM_Params params;

    /* Call driver init functions. */
    /*PWM_init();
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
   * Initializes a queue
   *
   * @param *a - The queue passed by reference
   * @return void - the queue gets initialized and can be used from there by reference.
   */
//void fifo_init(Queue *a) {
//    a->size = 0;
//    a->getI = 0;
//    a->putI = 0;
//}

/**
   * Adds an integer to the back of the queue. If the FIFO is full, returns -1
   *
   * @param *q - The queue passed by reference
   * @param *a - The integer to be written to the queue passed by reference
   * @return int - error status as an integer. returns 0 if it succeeded and -1 if there was an error
   */
//int fifo_put(Queue *q, int *a) {
//    if(q->size == FIFOSIZE) {               // if fifo is full, tell the bluetooth task it failed.
//        return -1;
//    } else {
//        q->queue[q->putI] = *a;             // put command number in fifo
//        q->putI = (q->putI + 1) % FIFOSIZE; // increment put pointer, also wraps around to to make fifo circular
//        q->size += 1;                       // increment fifo size counter
//        return 0;                           // return success
//    }
//}

/**
   * Returns the integer from the front of the queue.
   * If there is no data, it returns -1
   *
   * @param *q - The queue passed by reference
   * @return int - the number from the front of the queue
   */
//int fifo_get(Queue *q) {
//    int data;
//    if(q->size == 0) {                      // return failure if fifo is empty
//        return -1;
//    } else {
//        data = q->queue[q->getI];           // get from fifo
//        q->getI = (q->getI + 1) % FIFOSIZE; // increment get pointer and wrap around to make fifo circular
//        q->size -= 1;                       // decrement fifo size counter
//        return data;                        // return data
//    }
//}

/**
   * Sets the PWM value that was sent from the PID calculation
   *
   * @param pwmIncrement - comes from the PID calculation.
   * @return void - calls the actuator function to set the PWM value before returning
   */
void actuator(int pwmIncrement) {
    pwmIncrement = pwmIncrement * 255 / 10000;
    if(pwmIncrement < 0) {
        if(left < MOTORPERIOD) {  //
            left -= pwmIncrement;
            right = MOTORPERIOD;
        } else {
            right += pwmIncrement;
        }
    } else if(pwmIncrement > 0) {
        if(right < MOTORPERIOD) {
            right += pwmIncrement;
            left = MOTORPERIOD;
        } else {
            left -= pwmIncrement;
        }
    }
    if(right < 51) right = 51;
    if(right > MOTORPERIOD) right = MOTORPERIOD;
    if(left < 51) left = 51;
    if(left > MOTORPERIOD) left = MOTORPERIOD;
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
    if (duty > MOTORPERIOD) {
        duty = MOTORPERIOD;
    } else if (duty < -MOTORPERIOD) {
        duty = -MOTORPERIOD;
    }
    if (num == 0) {
        if (duty < 0) {
            pinOn(LEFTDIR);
            analogWrite(LEFTPWM, -duty);
        } else {
            pinOff(LEFTDIR);
            analogWrite(LEFTPWM, duty);
        }
    } else if (num == 1) {
        if (duty < 0) {
            pinOn(RIGHTDIR);
            analogWrite(RIGHTPWM, -duty);
        } else {
            pinOff(RIGHTDIR);
            analogWrite(RIGHTPWM, duty);
        }
    }
}

/**
 * Prints the amount of time the system ran in the format minutes:seconds.milliseconds
 *
 * @param none
 * @return none - outputs on UART terminal
 */
/*void printTime(void) {
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
}*/
