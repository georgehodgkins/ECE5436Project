void setupCommands() {
  commandsInit();
}

void loopCommands() {
  
}

//void printSem() {//REWRITE
    //Semaphore_post(printSema);
//}

void red() {
    //pinToggle(2,0);
}

void green() {
    //pinToggle(2,1);
}

void blue() {
    //pinToggle(2,2);
}
/*

void ADC() {
    int i = 0;
    char test[10];
    while(1) {
        uint16_t a = 0, b = 0;
        ADC_convert(adc0, &a);
        ADC_convert(adc1, &b);
        sprintf(test, "%d\t%d\n\r", a, b);
        putString(test);
        for(i = 0; i < 1000000; i++){}
    }
}

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
    struct commandListener *next; // linked list structure
    String command; // command string
} commandListener;

commandListener *baseCommand;

/**
 * Adds a command to our command list which is a linked list to allow for greater expandability
 *
 * @param function - a pointer to the function to run when a command is ran
 * @param command - a string that contains the function name
 * @return void
 */
void registerCommand(void (*function)(void), String command) {
    int i = 0;
    commandListener *current;
    if(baseCommand != NULL) {
        current = baseCommand;
        while(current->next != NULL) {
            i++;
            current = current->next;
        }
        current->next = (commandListener *) malloc(sizeof(commandListener));
        current = current->next;
    } else {
        baseCommand = (commandListener *) malloc(sizeof(commandListener));
        current = baseCommand;
    }
    current->func = function;
    current->next = NULL;
    i = 0;
    current->command = command;
}

/**
 * Cycles through out linked-list in an attempt to find the command requested
 *
 * @param command - a string with the command to be resolved
 * @return -1 - on unknown command
 * @return int - the command number to allow the command to be ran using that number
 */
int resolveCommand(String command) {
    int i = 0;
    commandListener *current = baseCommand;
    while(current->command != command) {
        if(current->next == NULL) {
            return -1;
        }
        current = current->next;
        i++;
    }
    return i;
}

/**
 * A function to run the command requested
 *
 * @param a - the number of the command requested
 * @return void
 */
void runCommand(int a) {
    int i;
    commandListener *current = baseCommand;
    for(i = 0; i < a; i++) {
        current = current->next;
    }
    current->func();
}



void commandsInit() {
    // create and initialize command queue fifo
    //commandQueue = (Queue *) malloc(sizeof(Queue));
    //fifo_init(commandQueue);

    //registerCommand(&red, "red");
    //registerCommand(&green, "green");
    registerCommand(&blue, "blue");
    /*registerCommand(&ADC, "adc");
    registerCommand(&start, "start");
    registerCommand(&stop, "stop");
    registerCommand(&reverse, "reverse");
    registerCommand(&low, "low");
    registerCommand(&med, "med");
    registerCommand(&high, "high");
    registerCommand(&spinl, "spinl");
    registerCommand(&spinr, "spinr");*/
}
