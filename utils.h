#ifndef UTILS_H_
#define UTILS_H_

// define which board we are using so the gpio package doesn't break everything
#define __MSP432P401R__

#define COMMANDLENGTH     10
#define NUMCOMMANDS       5
#define FIFOSIZE          10
#define MOTORPERIOD       10000
#define IDEAL             6000      // used for calculating error in the PID controller

// UART handle
UART_Handle uart;

//ADC
ADC_Handle adc0, adc1; // adc0 = right sensor

//PWM
PWM_Handle pwm0, pwm1; // pwm0 = left motor

//clock stuff
Timer_Handle timer0;

uint32_t runTime;

//PID stuff
int left;
int right;
int P;
int I;
int lastE;
int D;
int kp;
int kd;
int ki;
int print;
int front;
int lF;
uint8_t two;

//ping-pong queues
uint8_t queueNum;
uint8_t queueAddress[2];
int queue[2][20];

// FIFO int queue struct
typedef struct queue {
    int size;
    int getI;
    int putI;
    int queue[FIFOSIZE];
} Queue;

// Linked list to hold commands
typedef struct commandListener { // command listener struct
    void (*func)(void); // pointer to command listener function
    struct commandListener *next; // linked list structure
    char command[COMMANDLENGTH+1]; // command string
} commandListener;

commandListener *baseCommand;

Queue *commandQueue;

extern int equals(char*, char*);
extern int length(char*);
extern void putChar(char);
extern void putString(char*);
extern void getChar(char*);
extern void pinToggle(int port, int pin);
extern void pinOff(int port, int pin);
extern void pinOn(int port, int pin);
extern void registerCommand(void (*function)(void), char command[COMMANDLENGTH + 1]);
extern int resolveCommand(char command[COMMANDLENGTH + 1]);
extern void runCommand(int);
extern void fifo_init(Queue *a);
extern int fifo_put(Queue *q, int *a);
extern int fifo_get(Queue *q);
extern void UARTInit();
extern void GPIOInit();
extern void ADCInit();
extern void PWMInit();
extern void peidiDC();
extern void setDuty(int pwm, int period);
extern void printTime(void);

#endif /* UTILS_H_ */
