#include "utils.h"
#include "configurePin.h"
#include "commands.h"
#include <stdlib.h>
#include <stdio.h>

void commandsInit() {
    // create and initialize command queue fifo
    commandQueue = (Queue *) malloc(sizeof(Queue));
    fifo_init(commandQueue);

    registerCommand(&red, "red");
    registerCommand(&green, "green");
    registerCommand(&blue, "blue");
    registerCommand(&ADC, "adc");
    registerCommand(&start, "start");
    registerCommand(&stop, "stop");
    registerCommand(&reverse, "reverse");
    registerCommand(&low, "low");
    registerCommand(&med, "med");
    registerCommand(&high, "high");
    registerCommand(&spinl, "spinl");
    registerCommand(&spinr, "spinr");
}

void printSem() {//REWRITE
    Semaphore_post(printSema);
}

void red() {
    pinToggle(2,0);
}

void green() {
    pinToggle(2,1);
}

void blue() {
    pinToggle(2,2);
}

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
}
