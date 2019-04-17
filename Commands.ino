#include "stdlib.h"

void setupCommands() {
  analogReadResolution(14);
  commandsInit();
  left = 255;
  right = 255;
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

void forward() {
  digitalWrite(34, LOW);
  digitalWrite(36, LOW);
}

void backward() {
  digitalWrite(34, HIGH);
  digitalWrite(36, HIGH);
}

void spinl() {
  digitalWrite(34, LOW);
  digitalWrite(36, HIGH);
}

void spinr() {
  digitalWrite(34, HIGH);
  digitalWrite(36, LOW);
}

void full() {
  analogWrite(35, 16383);
  analogWrite(37, 16383);
}

void half() {
  analogWrite(35, 16383/2);
  analogWrite(37, 16383/2);
}

void stopp() {
  analogWrite(35, 0);
  analogWrite(37, 0);
}
void start() {
    setDuty(0, left);
    setDuty(1, right);
    runTime = 0;
    timer0.start();
}

// Linked list to hold commands
typedef struct commandListener { // command listener struct
    void (*func)(void); // pointer to command listener function
    String command; // command string
} commandListener;

commandListener Commands[11];

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
    registerCommand(&forward, "go");
    registerCommand(&backward, "back");
    registerCommand(&spinl, "spinl");
    registerCommand(&spinr, "spinr");
    registerCommand(&full, "full");
    registerCommand(&half, "half");
    registerCommand(&stopp, "stop");
    registerCommand(&start, "start");
}
