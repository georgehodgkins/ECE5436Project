#include <Galaxia.h>
#include <HWI.h>
#include <SWI.h>
#include "utils.h"

SWI int0;

void setupPeriodic () {
  timer0.begin(pwmEvent, 12500);//50 ms
  int0.begin(calculatePid);
  //timer0.start();
  front = 12000;
  kp = 4000;                  // gain value P
  ki = 700;                   // integral value I
  kd = 50000;                 // derivative value D
}

void loopPeriodic () {}

boolean counter;

void pwmEvent () {
  //pinToggle(76);
  //counter += 1;
  if(counter == 1) {
    calculatePid();
    counter = 0;
  } else {
    counter = 1;
  }
  //int0.post();//this SWI calls calculatePid
}

void calculatePid(void) {
    uint16_t R;             // right sensor
    uint16_t F;             // front sensor
    int E;                  // error for calculating PID
    int U;                  // for storing the final PID value
    runTime++;              // pidH controls whether or not PID is running or not(found in command.c - start()/stop())
    R = analogRead(A3);  // get the right sensors reading
    F = analogRead(A5);  // get the front sensors reading
    if(F > front) {
        pinOff(RIGHTDIR);
        pinOn(LEFTDIR);
        analogWrite(LEFTPWM, 255);
        analogWrite(RIGHTPWM, 255);
        //Serial1.println("AAAAAAAAAAAAAA");
        front = 8000;
        lF = 1;
    } else {
        if(lF) {
            pinOff(LEFTDIR);
            lF = 0;
            front = 12000;
        }
        E = R - IDEAL;                  // calculate the error
        if(E < 300 && E > -300) {E = 0;}
        // milestone 8 & 9 stuff
        //if(print) {
        //    two = (two + 1) % 2;
        //    //print stuff in here
        //    if(two == 1) {
        //        if(queueAddress[queueNum] == 20) {
        //            queueNum = (queueNum + 1) % 2;
        //            Semaphore_post(printSema);
        //        }
        //        queue[queueNum][queueAddress[queueNum]] = E;
        //        queueAddress[queueNum]++;
        //    }
        //}
        P = kp * E / MOTORPERIOD;             // calculate P
        I = (ki * (E + lastE)) / MOTORPERIOD;        // calculate I
        //if(I > 1000) I = 1000;          // limit I
        //if(I < -1000) I = -1000;
        D = (kd * (E - lastE)) / MOTORPERIOD; // calculate D
        lastE = E;                      // update previous error
        U = P +  I + D;                 // final PID value
        actuator(U);                    // set the PID value
    }
}
