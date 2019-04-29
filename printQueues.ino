#include <Semaphore.h>

Semaphore printSema;

/** 
 * Initialize the print queues
 * 
 * @returns void
 **/
void setupPrintQueues() {
    queueNum = 0;
    queueAddress[0] = 0;
    queueAddress[1] = 0;
    printSema.begin(0);
}

/** 
 * Print the queues, hangs on the printSema semaphore
 * 
 * @returns void
 **/
void loopPrintQueues() {
    uint8_t temp;
    int i;
    printSema.waitFor();
    pinToggle(77);
    pinOn(76);
    temp = (queueNum + 1) % 2;
    for (i = 0; i < queueAddress[temp]; i++) {
        Serial1.println(queue[temp][i]);
    }
    queueAddress[temp] = 0;
    delay(50);
    pinToggle(76);
    pinOn(77);
    if(finished == 1) {
      printTime();
    }
}
