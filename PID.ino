#include <Galaxia.h>
#include <HWI.h>
#include <SWI.h>
#include "utils.h"

Timer timer0;
SWI int0;

void setupPeriodic () {
  timer0.begin(pwmEvent, 10);//10 ms = 10000 us
  int0.begin(calculatePid);
  timer0.start();
}

void loopPeriodic () {}

void pwmEvent () {
  int1.post();//this SWI calls calculatePid
}

void calculatePid(void) {
    uint16_t R;             // right sensor
    uint16_t F;             // front sensor
    int E;                  // error for calculating PID
    int U;                  // for storing the final PID value
    runTime++;              // pidH controls whether or not PID is running or not(found in command.c - start()/stop())
    R = AnalogRead(PLACEHOLDER);  // get the right sensors reading
    F = AnalogRead(PLACEHOLDER);  // get the front sensors reading
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
