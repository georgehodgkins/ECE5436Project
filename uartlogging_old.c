/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uartlogging.c ========
 */
// define which board we are using so the gpio package doesn't break everything
#define __MSP432P401R__

#include <stdlib.h>
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Driver Header files */
#include <ti/drivers/UART.h>
#include <ti/drivers/Power.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/* Example/Board Header files */
#include "Board.h"

/* Team 6 Header files */
#include "utils.h"
#include "commands.h"

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    UART_init();
    ADC_init();

    /*
     *  Disable Power management.  For this example, we need the idle
     *  loop to run sufficiently often to upload Log records to CCS.
     */
    Power_disablePolicy();

    // Setup for the team 6 robot
    UARTInit();
    commandsInit();
    GPIOInit();
    ADCInit();
    PWMInit();

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

    /* Start BIOS */
    BIOS_start();

    return (0);
}
