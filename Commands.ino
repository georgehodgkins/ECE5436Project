#include "stdlib.h"

void setupCommands() {
  analogReadResolution(14);
  commandsInit();
}

void loopCommands() {
  
}

//void printSem() {//REWRITE
    //Semaphore_post(printSema);
//}

void red() {
    pinToggle(75);
}

void green() {
    pinToggle(76);
}

void blue() {
    pinToggle(77);
}

void ADC() {
  Serial1.println(analogRead(A3)); // right
  Serial1.println(analogRead(A5)); // front
}
/*

void reverse() {
    pinToggle(2, 4);
    pinToggle(5, 6);
}

void spinl() {
    pinToggle(2, 4);
    //pinToggle(5, 6);
}

void spinr() {
    //pinToggle(2, 4);
    pinToggle(5, 6);
}

void start() {
    setDuty(0, left);
    setDuty(1, right);
    runTime = 0;
  //rewrite
    Timer_start(PIDTimer);
}

void stop() {
  //rewrite
    Timer_stop(PIDTimer);
    setDuty(0, 0);
    setDuty(1, 0);
}

void low() {
    setDuty(0, 2500);
    setDuty(1, 2500);
}

void med() {
    setDuty(0, 5000);
    setDuty(1, 5000);
}

void high() {
    setDuty(0, 10000);
    setDuty(1, 10000);
}*/


// Linked list to hold commands
typedef struct commandListener { // command listener struct
    void (*func)(void); // pointer to command listener function
    String command; // command string
} commandListener;

commandListener Commands[4];

/**
 * Adds a command to our command list which is a linked list to allow for greater expandability
 *
 * @param function - a pointer to the function to run when a command is ran
 * @param command - a string that contains the function name
 * @return void
 */
int commandCount;
void registerCommand(void (*function)(void), String command) {
    commandListener *current = &Commands[commandCount];
    current->func = function;
    current->command = command;
    commandCount++;
    //Serial1.println(command);
}

/**
 * Cycles through out linked-list in an attempt to find the command requested then runs the command
 *
 * @param command - a string with the command to be resolved
 * @return -1 - on unknown command
 * @return int - the number of the command ran
 */
int runCommand(String command) {
    int i = commandCount;
    while(!Commands[i].command.equals(command) && i >= 0) {
        i--;
    }
    if(i >= 0) {
      Commands[i].func();
    }
    return i;
}



void commandsInit() {
    // create and initialize command queue fifo
    //commandQueue = (Queue *) malloc(sizeof(Queue));
    //fifo_init(commandQueue);
    commandCount = 0;
    registerCommand(&red, "red");
    registerCommand(&green, "green");
    registerCommand(&blue, "blue");
    registerCommand(&ADC, "adc");
    /*registerCommand(&start, "start");
    registerCommand(&stop, "stop");
    registerCommand(&reverse, "reverse");
    registerCommand(&low, "low");
    registerCommand(&med, "med");
    registerCommand(&high, "high");
    registerCommand(&spinl, "spinl");
    registerCommand(&spinr, "spinr");*/
}
