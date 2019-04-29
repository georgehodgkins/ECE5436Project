#include <Galaxia.h>
#include <HWI.h>
#include <SWI.h>
#include <Semaphore.h>
#include "utils.h"

extern Semaphore printSema;
int counter; //counts the number of calls to the PID fcn since the robot started, proxy for system time
int last;//tracks the last result of the line detection (0=line, 1=no line)

/*
 * Initializes periodic sensor readings using a timer
 * 
 * @param none
 * @return void
 *
 */
void setupPeriodic () {
  timer0.begin(pwmEvent, 12500);
  //timer0.start();
  front = 12000;
  kp = 500;                  // gain value P
  ki = 500;                   // integral value I
  kd = 6000;                 // derivative value D
  last = 1;
  finished = 0;
}

//empty loop fcn required by Energia
void loopPeriodic () {}

boolean period;

/*
 * Periodic event called by the timer set up in setupPeriodic(),
 * calls LightSensor() every 25 ms and calculatePid() every 50 ms
 * 
 * @param none
 * @return void
 */
void pwmEvent () {
  LightSensor();
  if (period == 1) {
    calculatePid();
    period = 0;
  } else {
    period = 1;
  }
  //int0.post();//this SWI calls calculatePid
}


/*
 * Periodic PID calculation function,
 * which calls the actuator function to actually set the new motor speeds
 * 
 * @param none
 * @return void
 */
void calculatePid(void) {
  uint16_t R;             // right sensor
  uint16_t F;             // front sensor
  int E;                  // error for calculating PID
  int U;                  // for storing the final PID value
  runTime++;              // pidH controls whether or not PID is running or not(found in command.c - start()/stop())
  R = analogRead(A3);  // get the right sensors reading
  F = analogRead(A5);  // get the front sensors reading
  if (F > front) {
    pinOff(RIGHTDIR);
    pinOn(LEFTDIR);
    analogWrite(LEFTPWM, 255);
    analogWrite(RIGHTPWM, 255);
    //Serial1.println("AAAAAAAAAAAAAA");
    front = 8000;
    lF = 1;
  } else {
    if (lF) {
      pinOff(LEFTDIR);
      lF = 0;
      front = 12000;
    }
    E = R - IDEAL;                  // calculate the error
    if(print) {
        two = (two + 1) % 2;
        //print stuff in here
        if(two == 1) {
            if(queueAddress[queueNum] == 20) {
                queueNum = (queueNum + 1) % 2;
                printSema.post();
            }
            queue[queueNum][queueAddress[queueNum]] = E;
            queueAddress[queueNum]++;
        }
    }
    if (E < 600 && E > -600) {
      E = 0;
    }
    P = kp * E / MOTORPERIOD;             // calculate P
    I = (ki * (E + lastE)) / MOTORPERIOD;        // calculate I
    //if(I > 1000) I = 1000;          // limit I
    //if(I < -1000) I = -1000;
    D = (kd * (E - lastE)) / MOTORPERIOD; // calculate D
    lastE = E;                      // update previous error
    U = P +  I + D;                 // final PID value
    if(finished == 0) {
      actuator(U);                    // set the PID value
    }
  }
}

/*
 * Periodic function to read light sensor input and detect lines
 * 
 * @param none
 * @return void
 */
void LightSensor() {
  // 1.Configure GPIO pin as output
  // 2.Set output to HIGH
  // 3.Allow at least 10[µs] for the sensor output voltage to rise
  // 4.Reconfigure GPIO pin as input
  // 5.Measure time elapsed until input is LOW
  // 6.Use the measured time to determine whether on black or white surface
  int countPin6 = 0;
  int threshold = 100;
  //while(1){
  pinMode(23, OUTPUT);    // configure port 3 pin 6 as an output pin
  digitalWrite(23, 1);//set the port 3 pin 6 to high
  //GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);    // configure port 3 pin 7 as an output pin
  //GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);//set the port 3 pin 7 to GPIO_INPUT_PIN_HIGH
  //start a timer
  for (int i = 0; i < 1000; i++) {}
  pinMode(23, INPUT);     // reconfigure this to an input pin
  //GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN7);     // reconfigure this to an input pin
  countPin6 = 0;
  //countPin7 = 0;
  while (digitalRead(23) == HIGH) {
    countPin6 +=  1;
    //countPin7 +=  GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7);
  }                      // read from port 3 pin 6 and 7

  //Serial1.println(countPin6);
  //sprintf(a, "countPin6 %d \n\r", countPin6); // then output command unknown
  if (countPin6 > threshold && last) {
    //Serial1.println("Black");
    pinToggle(77);
    last = 0;
    counter++;
  } else if (countPin6 < threshold && last == 0) {
    if (counter > 8) {
      pinOff(77);
      timer0.stop();
      setDuty(0, 0);
      setDuty(1, 0);
      queueNum = (queueNum + 1) % 2;
      printSema.post();
      finished = 1;
      timer0.stop();
      setDuty(0, 0);
      setDuty(1, 0);
      
    } else {
      print = (print + 1) % 2;
    }
    counter = 0;
    last = 1;
  } else if (countPin6 > threshold) counter++;
}
