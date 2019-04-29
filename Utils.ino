#include "utils.h"
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
    if(right < 100) right = 100;
    if(right > MOTORPERIOD) right = MOTORPERIOD;
    if(left < 100) left = 100;
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
void printTime(void) {
    uint32_t minutes;
    uint32_t seconds;
    uint32_t milliSeconds;
    char a[15];
    milliSeconds = runTime * 25;
    seconds = milliSeconds / 1000;
    milliSeconds -= seconds * 1000;
    minutes = seconds / 60;
    seconds -= minutes * 60;
    Serial1.print(minutes);
    Serial1.print(":");
    Serial1.print(seconds);
    Serial1.print(".");
    Serial1.println(milliSeconds);
}
