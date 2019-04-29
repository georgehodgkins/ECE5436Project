#ifndef UTILS_H_
#define UTILS_H_
#include <stdint.h>

#define MOTORPERIOD       255
#define IDEAL             6000      // used for calculating error in the PID controller
#define RIGHTDIR          34
#define RIGHTPWM          35
#define LEFTDIR           36
#define LEFTPWM           37

//clock stuff
//Timer_Handle timer0;
Timer timer0;

uint32_t runTime;

//PID stuff
int left; // left motor pwm value
int right; // right motor pwm value
int P; // Calculated P
int I; // Calculate I
int D; // Calculated D
int lastE; // last error
int kp; // proportional gain
int kd; // differential gain
int ki; // integral gain
int print; // for keeping track of priint queues
int front; // distance from front
int lF; // last front value
uint8_t two; // used in printing queues
boolean finished; // used to flag that the maze is done

//ping-pong queues
uint8_t queueNum;
uint8_t queueAddress[2];
int queue[2][20];

#endif /* UTILS_H_ */
