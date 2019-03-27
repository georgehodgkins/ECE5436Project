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

#ifndef __BOARD_H
#define __BOARD_H

#define Board_MSP_EXP432P401R

#ifdef __cplusplus
extern "C" {
#endif

#include "MSP_EXP432P401R.h"

#define Board_initGeneral           MSP_EXP432P401R_initGeneral

#define Board_ADC0                  MSP_EXP432P401R_ADC0
#define Board_ADC1                  MSP_EXP432P401R_ADC1
#define Board_ADC2                  MSP_EXP432P401R_ADC2
#define Board_ADC3                  MSP_EXP432P401R_ADC3
#define Board_ADC4                  MSP_EXP432P401R_ADC4
#define Board_ADC5                  MSP_EXP432P401R_ADC5
#define Board_ADC6                  MSP_EXP432P401R_ADC6
#define Board_ADC7                  MSP_EXP432P401R_ADC7
#define Board_ADC8                  MSP_EXP432P401R_ADC8
#define Board_ADC9                  MSP_EXP432P401R_ADC9
#define Board_ADC10                 MSP_EXP432P401R_ADC10
#define Board_ADC11                 MSP_EXP432P401R_ADC11
#define Board_ADC12                 MSP_EXP432P401R_ADC12
#define Board_ADC13                 MSP_EXP432P401R_ADC13
#define Board_ADC14                 MSP_EXP432P401R_ADC14
#define Board_ADC15                 MSP_EXP432P401R_ADC15

#define Board_ADCBUF0               MSP_EXP432P401R_ADCBUF0
#define Board_ADCBUF0CHANNEL0       MSP_EXP432P401R_ADCBUF0CHANNEL0
#define Board_ADCBUF0CHANNEL1       MSP_EXP432P401R_ADCBUF0CHANNEL1
#define Board_ADCBUF0CHANNEL2       MSP_EXP432P401R_ADCBUF0CHANNEL2
#define Board_ADCBUF0CHANNEL3       MSP_EXP432P401R_ADCBUF0CHANNEL3

#define Board_CAPTURE0              MSP_EXP432P401R_CAPTURE_TA1
#define Board_CAPTURE1              MSP_EXP432P401R_CAPTURE_TA2
#define Board_CAPTURE2              MSP_EXP432P401R_CAPTURE_TA3

#define Board_GPIO_LED_ON           MSP_EXP432P401R_GPIO_LED_ON
#define Board_GPIO_LED_OFF          MSP_EXP432P401R_GPIO_LED_OFF

#define Board_GPIO_LED0             MSP_EXP432P401R_GPIO_LED1
#define Board_GPIO_LED1             MSP_EXP432P401R_GPIO_LED_RED
/*
 * MSP_EXP432P401R_GPIO_LED_GREEN & MSP_EXP432P401R_GPIO_LED_BLUE are used for
 * PWM examples.  Uncomment the following lines if you would like to control
 * the LEDs with the GPIO driver.
 */
/* #define Board_GPIO_LED2           MSP_EXP432P401R_GPIO_LED_GREEN */
/* #define Board_GPIO_LED3           MSP_EXP432P401R_GPIO_LED_BLUE */

#define Board_GPIO_BUTTON0          MSP_EXP432P401R_GPIO_S1
#define Board_GPIO_BUTTON1          MSP_EXP432P401R_GPIO_S2

#define Board_I2C0                  MSP_EXP432P401R_I2CB0
#define Board_I2C_TPL0401           MSP_EXP432P401R_I2CB0
#define Board_I2C_TMP               MSP_EXP432P401R_I2CB1

#define Board_I2CSLAVE0             MSP_EXP432P401R_I2CSLAVEB0

#define Board_NVSINTERNAL           MSP_EXP432P401R_NVSMSP4320

#define Board_PWM0                  MSP_EXP432P401R_PWM_TA1_1
#define Board_PWM1                  MSP_EXP432P401R_PWM_TA1_2
#define Board_PWM2                  MSP_EXP432P401R_PWM_TA0_3
#define Board_PWM3                  MSP_EXP432P401R_PWM_TA0_4

#define Board_SD0                   MSP_EXP432P401R_SDSPI0

#define Board_SDFatFS0              MSP_EXP432P401R_SDSPI0

#define Board_SPI0                  MSP_EXP432P401R_SPIB0
#define Board_SPI1                  MSP_EXP432P401R_SPIB2
#define Board_SPI2                  MSP_EXP432P401R_SPIB3
#define Board_SPI3                  MSP_EXP432P401R_SPIB4
#define Board_SPI_CS1               MSP_EXP432P401R_SPI_CS1
#define Board_SPI_CS2               MSP_EXP432P401R_SPI_CS2

#define Board_SPI_MASTER            MSP_EXP432P401R_SPIB3
#define Board_SPI_SLAVE             MSP_EXP432P401R_SPIB3
#define Board_SPI_MASTER_READY      MSP_EXP432P401R_SPI_MASTER_READY
#define Board_SPI_SLAVE_READY       MSP_EXP432P401R_SPI_SLAVE_READY

#define Board_TIMER0                MSP_EXP432P401R_TIMER_T32_0
#define Board_TIMER1                MSP_EXP432P401R_TIMER_T32_1
#define Board_TIMER2                MSP_EXP432P401R_TIMER_TA_1
#define Board_TIMER3                MSP_EXP432P401R_TIMER_TA_2
#define Board_TIMER4                MSP_EXP432P401R_TIMER_TA_3

#define Board_UART0                 MSP_EXP432P401R_UARTA0
#define Board_UART1                 MSP_EXP432P401R_UARTA2

#define Board_WATCHDOG0             MSP_EXP432P401R_WATCHDOG

/* Board specific I2C addresses */
#define Board_TMP_ADDR              (0x40)
#define Board_SENSORS_BP_TMP_ADDR   Board_TMP_ADDR
#define Board_TPL0401_ADDR          (0x40)

// Mason added this stuff
#define P1_0    0x101    /*!< @hideinitializer */
#define P1_1    0x102    /*!< @hideinitializer */
#define P1_2    0x104    /*!< @hideinitializer */
#define P1_3    0x108    /*!< @hideinitializer */
#define P1_4    0x110    /*!< @hideinitializer */
#define P1_5    0x120    /*!< @hideinitializer */
#define P1_6    0x140    /*!< @hideinitializer */
#define P1_7    0x180    /*!< @hideinitializer */

#define P2_0    0x201    /*!< @hideinitializer */
#define P2_1    0x202    /*!< @hideinitializer */
#define P2_2    0x204    /*!< @hideinitializer */
#define P2_3    0x208    /*!< @hideinitializer */
#define P2_4    0x210    /*!< @hideinitializer */
#define P2_5    0x220    /*!< @hideinitializer */
#define P2_6    0x240    /*!< @hideinitializer */
#define P2_7    0x280    /*!< @hideinitializer */

#define P3_0    0x301    /*!< @hideinitializer */
#define P3_1    0x302    /*!< @hideinitializer */
#define P3_2    0x304    /*!< @hideinitializer */
#define P3_3    0x308    /*!< @hideinitializer */
#define P3_4    0x310    /*!< @hideinitializer */
#define P3_5    0x320    /*!< @hideinitializer */
#define P3_6    0x340    /*!< @hideinitializer */
#define P3_7    0x380    /*!< @hideinitializer */

#define P4_0    0x401    /*!< @hideinitializer */
#define P4_1    0x402    /*!< @hideinitializer */
#define P4_2    0x404    /*!< @hideinitializer */
#define P4_3    0x408    /*!< @hideinitializer */
#define P4_4    0x410    /*!< @hideinitializer */
#define P4_5    0x420    /*!< @hideinitializer */
#define P4_6    0x440    /*!< @hideinitializer */
#define P4_7    0x480    /*!< @hideinitializer */

#define P5_0    0x501    /*!< @hideinitializer */
#define P5_1    0x502    /*!< @hideinitializer */
#define P5_2    0x504    /*!< @hideinitializer */
#define P5_3    0x508    /*!< @hideinitializer */
#define P5_4    0x510    /*!< @hideinitializer */
#define P5_5    0x520    /*!< @hideinitializer */
#define P5_6    0x540    /*!< @hideinitializer */
#define P5_7    0x580    /*!< @hideinitializer */

#define P6_0    0x601    /*!< @hideinitializer */
#define P6_1    0x602    /*!< @hideinitializer */
#define P6_2    0x604    /*!< @hideinitializer */
#define P6_3    0x608    /*!< @hideinitializer */
#define P6_4    0x610    /*!< @hideinitializer */
#define P6_5    0x620    /*!< @hideinitializer */
#define P6_6    0x640    /*!< @hideinitializer */
#define P6_7    0x680    /*!< @hideinitializer */

/** @} */

/**
 *  @name Ports 7-10 and 'J' do NOT support interrupts.
 *  @{
 */
#define P7_0    0x701    /*!< @hideinitializer */
#define P7_1    0x702    /*!< @hideinitializer */
#define P7_2    0x704    /*!< @hideinitializer */
#define P7_3    0x708    /*!< @hideinitializer */
#define P7_4    0x710    /*!< @hideinitializer */
#define P7_5    0x720    /*!< @hideinitializer */
#define P7_6    0x740    /*!< @hideinitializer */
#define P7_7    0x780    /*!< @hideinitializer */

#define P8_0    0x801    /*!< @hideinitializer */
#define P8_1    0x802    /*!< @hideinitializer */
#define P8_2    0x804    /*!< @hideinitializer */
#define P8_3    0x808    /*!< @hideinitializer */
#define P8_4    0x810    /*!< @hideinitializer */
#define P8_5    0x820    /*!< @hideinitializer */
#define P8_6    0x840    /*!< @hideinitializer */
#define P8_7    0x880    /*!< @hideinitializer */

#define P9_0    0x901    /*!< @hideinitializer */
#define P9_1    0x902    /*!< @hideinitializer */
#define P9_2    0x904    /*!< @hideinitializer */
#define P9_3    0x908    /*!< @hideinitializer */
#define P9_4    0x910    /*!< @hideinitializer */
#define P9_5    0x920    /*!< @hideinitializer */
#define P9_6    0x940    /*!< @hideinitializer */
#define P9_7    0x980    /*!< @hideinitializer */

#define P10_0   0xa01    /*!< @hideinitializer */
#define P10_1   0xa02    /*!< @hideinitializer */
#define P10_2   0xa04    /*!< @hideinitializer */
#define P10_3   0xa08    /*!< @hideinitializer */
#define P10_4   0xa10    /*!< @hideinitializer */
#define P10_5   0xa20    /*!< @hideinitializer */
#define P10_6   0xa40    /*!< @hideinitializer */
#define P10_7   0xa80    /*!< @hideinitializer */

#define PJ_0    0xb01    /*!< @hideinitializer */
#define PJ_1    0xb02    /*!< @hideinitializer */
#define PJ_2    0xb04    /*!< @hideinitializer */
#define PJ_3    0xb08    /*!< @hideinitializer */
#define PJ_4    0xb10    /*!< @hideinitializer */
#define PJ_5    0xb20    /*!< @hideinitializer */
#define PJ_6    0xb40    /*!< @hideinitializer */
#define PJ_7    0xb80    /*!< @hideinitializer */
// end of Mason's stuff


#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
