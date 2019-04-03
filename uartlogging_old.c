/* Team 6 Header files */
#include "utils.h"
#include "commands.h"
#include "threads.h"

int main(void)
{
    // Setup for the team 6 robot
    UARTInit();
    commandsInit();
    GPIOInit();
    //ADCInit();
    //PWMInit();

    // Initialize variables for team 6 robot
    left = 9000;
    right = 9000;
    P = 0;
    I = 0;
    queueNum = 0;
    queueAddress[0] = 0;
    queueAddress[1] = 0;
    front = 12000;
    two = 0;
    kp = 4000;                  // gain value P
    ki = 700;                   // integral value I
    kd = 50000;                 // derivative value D

		OS_AddThread(&heartbeat, 0);
		OS_AddThread(&bluetooth, 0);

    /* Start BIOS */
    //BIOS_start();
		OS_Launch(48000);

    return (0);
}