#include <stdlib.h>
#ifndef COMMANDS_H_
#define COMMANDS_H_

extern void commandsInit();
extern void red();
extern void green();
extern void blue();
extern void ADC();
extern void reverse();
extern void spinl();
extern void spinr();
extern void start();
extern void stop();
extern void low();
extern void med();
extern void high();
extern void actuator(int);

#endif /* COMMANDS_H_ */
