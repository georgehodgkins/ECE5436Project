#include <stdlib.h>
#ifndef COMMANDS_H_
#define COMMANDS_H_

extern void commandsInit(void);
extern void red(void);
extern void green(void);
extern void blue(void);
extern void ADC(void);
extern void reverse(void);
extern void spinl(void);
extern void spinr(void);
extern void start(void);
extern void stop(void);
extern void low(void);
extern void med(void);
extern void high(void);
extern void actuator(int);

#endif /* COMMANDS_H_ */
