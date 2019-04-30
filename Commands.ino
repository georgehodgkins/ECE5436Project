#include "stdlib.h"

/**
 * Setup command handler
 *
 * @param none
 * @return none
 */
void setupCommands() {
  commandsInit();
  left = 255;
  right = 255;
}

void loopCommands() {
  // Empty loop to please Energia sun the setup function. Loop never got used.
}

/**
 * toggle onboard RGB LED's Red filament
 *
 * @param none
 * @return none
 */
void red() {
    pinToggle(75);
}

/**
 * toggle onboard RGB LED's Green filament
 *
 * @param none
 * @return none
 */
void green() {
    pinToggle(76);
}

/**
 * toggle onboard RGB LED's Blue filament
 *
 * @param none
 * @return none
 */
void blue() {
    pinToggle(77);
}

/**
 * Take one reading from each ADC
 *
 * @param none
 * @return none
 */
void ADC() {
  Serial1.println(analogRead(A3)); // right
  Serial1.println(analogRead(A5)); // front
}

/**
 * Set motors to go forward
 *
 * @param none
 * @return none
 */
void forward() {
  digitalWrite(34, LOW);
  digitalWrite(36, LOW);
}

/**
 * Set motors to go backwards
 *
 * @param none
 * @return none
 */
void backward() {
  digitalWrite(34, HIGH);
  digitalWrite(36, HIGH);
}

/**
 * Set motors to spin left
 *
 * @param none
 * @return none
 */
void spinl() {
  digitalWrite(34, LOW);
  digitalWrite(36, HIGH);
}

/**
 * Set motors to spin right
 *
 * @param none
 * @return none
 */
void spinr() {
  digitalWrite(34, HIGH);
  digitalWrite(36, LOW);
}

/**
 * Set PWM power to full
 *
 * @param none
 * @return none
 */
void full() {
  analogWrite(35, 255);
  analogWrite(37, 255);
}

/**
 * Set PWM power to half
 *
 * @param none
 * @return none
 */
void half() {
  analogWrite(35, 128);
  analogWrite(37, 128);
}

/**
 * Set PWM power to 0
 *
 * @param none
 * @return none
 */
void stopp() {
  analogWrite(35, 0);
  analogWrite(37, 0);
}

/**
 * Start the maze-solving algorithm
 *
 * @param none
 * @return none
 */
void start() {
    setDuty(0, left);
    setDuty(1, right);
    runTime = 0;
    timer0.start();
    finished = 0;
}

// Struct to hold commands
typedef struct commandListener { // command listener struct
    void (*func)(void); // pointer to command listener function
    String command; // command string
} commandListener;

// Array of structs of commands
commandListener Commands[12];

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


// Initialize the commands we are using.
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
