/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for MotorB */
#define MotorB_INST                                                        TIMG6
#define MotorB_INST_IRQHandler                                  TIMG6_IRQHandler
#define MotorB_INST_INT_IRQN                                    (TIMG6_INT_IRQn)
#define MotorB_INST_CLK_FREQ                                             1000000
/* GPIO defines for channel 0 */
#define GPIO_MotorB_C0_PORT                                                GPIOB
#define GPIO_MotorB_C0_PIN                                         DL_GPIO_PIN_2
#define GPIO_MotorB_C0_IOMUX                                     (IOMUX_PINCM15)
#define GPIO_MotorB_C0_IOMUX_FUNC                    IOMUX_PINCM15_PF_TIMG6_CCP0
#define GPIO_MotorB_C0_IDX                                   DL_TIMER_CC_0_INDEX

/* Defines for MotorA */
#define MotorA_INST                                                        TIMG7
#define MotorA_INST_IRQHandler                                  TIMG7_IRQHandler
#define MotorA_INST_INT_IRQN                                    (TIMG7_INT_IRQn)
#define MotorA_INST_CLK_FREQ                                             1000000
/* GPIO defines for channel 0 */
#define GPIO_MotorA_C0_PORT                                                GPIOB
#define GPIO_MotorA_C0_PIN                                        DL_GPIO_PIN_15
#define GPIO_MotorA_C0_IOMUX                                     (IOMUX_PINCM32)
#define GPIO_MotorA_C0_IOMUX_FUNC                    IOMUX_PINCM32_PF_TIMG7_CCP0
#define GPIO_MotorA_C0_IDX                                   DL_TIMER_CC_0_INDEX

/* Defines for Buzzer */
#define Buzzer_INST                                                        TIMG8
#define Buzzer_INST_IRQHandler                                  TIMG8_IRQHandler
#define Buzzer_INST_INT_IRQN                                    (TIMG8_INT_IRQn)
#define Buzzer_INST_CLK_FREQ                                             1000000
/* GPIO defines for channel 0 */
#define GPIO_Buzzer_C0_PORT                                                GPIOA
#define GPIO_Buzzer_C0_PIN                                        DL_GPIO_PIN_21
#define GPIO_Buzzer_C0_IOMUX                                     (IOMUX_PINCM46)
#define GPIO_Buzzer_C0_IOMUX_FUNC                    IOMUX_PINCM46_PF_TIMG8_CCP0
#define GPIO_Buzzer_C0_IDX                                   DL_TIMER_CC_0_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (3124U)
/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMA1)
#define TIMER_1_INST_IRQHandler                                 TIMA1_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                          (1999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOB
#define GPIO_UART_0_TX_PORT                                                GPIOB
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM13)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM12)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM13_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM12_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_0_FBRD_32_MHZ_115200_BAUD                                      (23)
/* Defines for UART_WIT */
#define UART_WIT_INST                                                      UART1
#define UART_WIT_INST_FREQUENCY                                         32000000
#define UART_WIT_INST_IRQHandler                                UART1_IRQHandler
#define UART_WIT_INST_INT_IRQN                                    UART1_INT_IRQn
#define GPIO_UART_WIT_RX_PORT                                              GPIOA
#define GPIO_UART_WIT_RX_PIN                                       DL_GPIO_PIN_9
#define GPIO_UART_WIT_IOMUX_RX                                   (IOMUX_PINCM20)
#define GPIO_UART_WIT_IOMUX_RX_FUNC                    IOMUX_PINCM20_PF_UART1_RX
#define UART_WIT_BAUD_RATE                                              (115200)
#define UART_WIT_IBRD_32_MHZ_115200_BAUD                                    (17)
#define UART_WIT_FBRD_32_MHZ_115200_BAUD                                    (23)





/* Defines for DMA_WIT */
#define DMA_WIT_CHAN_ID                                                      (0)
#define UART_WIT_INST_DMA_TRIGGER                            (DMA_UART1_RX_TRIG)


/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED_PORT                                                    (GPIOB)

/* Defines for Led1: GPIOB.22 with pinCMx 50 on package pin 21 */
#define GPIO_LED_Led1_PIN                                       (DL_GPIO_PIN_22)
#define GPIO_LED_Led1_IOMUX                                      (IOMUX_PINCM50)
/* Defines for Led2: GPIOB.18 with pinCMx 44 on package pin 15 */
#define GPIO_LED_Led2_PIN                                       (DL_GPIO_PIN_18)
#define GPIO_LED_Led2_IOMUX                                      (IOMUX_PINCM44)
/* Defines for PIN_motorA2: GPIOB.16 with pinCMx 33 on package pin 4 */
#define motor_PIN_motorA2_PORT                                           (GPIOB)
#define motor_PIN_motorA2_PIN                                   (DL_GPIO_PIN_16)
#define motor_PIN_motorA2_IOMUX                                  (IOMUX_PINCM33)
/* Defines for PIN_motorB2: GPIOB.3 with pinCMx 16 on package pin 51 */
#define motor_PIN_motorB2_PORT                                           (GPIOB)
#define motor_PIN_motorB2_PIN                                    (DL_GPIO_PIN_3)
#define motor_PIN_motorB2_IOMUX                                  (IOMUX_PINCM16)
/* Defines for PIN_motorA1: GPIOA.16 with pinCMx 38 on package pin 9 */
#define motor_PIN_motorA1_PORT                                           (GPIOA)
#define motor_PIN_motorA1_PIN                                   (DL_GPIO_PIN_16)
#define motor_PIN_motorA1_IOMUX                                  (IOMUX_PINCM38)
/* Defines for PIN_motorB1: GPIOA.2 with pinCMx 7 on package pin 42 */
#define motor_PIN_motorB1_PORT                                           (GPIOA)
#define motor_PIN_motorB1_PIN                                    (DL_GPIO_PIN_2)
#define motor_PIN_motorB1_IOMUX                                   (IOMUX_PINCM7)
/* Defines for OLED_SCL: GPIOB.7 with pinCMx 24 on package pin 59 */
#define OLED_OLED_SCL_PORT                                               (GPIOB)
#define OLED_OLED_SCL_PIN                                        (DL_GPIO_PIN_7)
#define OLED_OLED_SCL_IOMUX                                      (IOMUX_PINCM24)
/* Defines for OLED_SDA: GPIOA.13 with pinCMx 35 on package pin 6 */
#define OLED_OLED_SDA_PORT                                               (GPIOA)
#define OLED_OLED_SDA_PIN                                       (DL_GPIO_PIN_13)
#define OLED_OLED_SDA_IOMUX                                      (IOMUX_PINCM35)
/* Defines for Left_A: GPIOB.23 with pinCMx 51 on package pin 22 */
#define Encoder_Left_A_PORT                                              (GPIOB)
// pins affected by this interrupt request:["Left_A","Left_B","Right_B"]
#define Encoder_GPIOB_INT_IRQN                                  (GPIOB_INT_IRQn)
#define Encoder_GPIOB_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define Encoder_Left_A_IIDX                                 (DL_GPIO_IIDX_DIO23)
#define Encoder_Left_A_PIN                                      (DL_GPIO_PIN_23)
#define Encoder_Left_A_IOMUX                                     (IOMUX_PINCM51)
/* Defines for Left_B: GPIOB.27 with pinCMx 58 on package pin 29 */
#define Encoder_Left_B_PORT                                              (GPIOB)
#define Encoder_Left_B_IIDX                                 (DL_GPIO_IIDX_DIO27)
#define Encoder_Left_B_PIN                                      (DL_GPIO_PIN_27)
#define Encoder_Left_B_IOMUX                                     (IOMUX_PINCM58)
/* Defines for Right_A: GPIOA.12 with pinCMx 34 on package pin 5 */
#define Encoder_Right_A_PORT                                             (GPIOA)
// pins affected by this interrupt request:["Right_A"]
#define Encoder_GPIOA_INT_IRQN                                  (GPIOA_INT_IRQn)
#define Encoder_GPIOA_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define Encoder_Right_A_IIDX                                (DL_GPIO_IIDX_DIO12)
#define Encoder_Right_A_PIN                                     (DL_GPIO_PIN_12)
#define Encoder_Right_A_IOMUX                                    (IOMUX_PINCM34)
/* Defines for Right_B: GPIOB.6 with pinCMx 23 on package pin 58 */
#define Encoder_Right_B_PORT                                             (GPIOB)
#define Encoder_Right_B_IIDX                                 (DL_GPIO_IIDX_DIO6)
#define Encoder_Right_B_PIN                                      (DL_GPIO_PIN_6)
#define Encoder_Right_B_IOMUX                                    (IOMUX_PINCM23)
/* Defines for Up: GPIOA.17 with pinCMx 39 on package pin 10 */
#define Key_Up_PORT                                                      (GPIOA)
#define Key_Up_PIN                                              (DL_GPIO_PIN_17)
#define Key_Up_IOMUX                                             (IOMUX_PINCM39)
/* Defines for Left: GPIOA.15 with pinCMx 37 on package pin 8 */
#define Key_Left_PORT                                                    (GPIOA)
#define Key_Left_PIN                                            (DL_GPIO_PIN_15)
#define Key_Left_IOMUX                                           (IOMUX_PINCM37)
/* Defines for Right: GPIOA.14 with pinCMx 36 on package pin 7 */
#define Key_Right_PORT                                                   (GPIOA)
#define Key_Right_PIN                                           (DL_GPIO_PIN_14)
#define Key_Right_IOMUX                                          (IOMUX_PINCM36)
/* Defines for Down: GPIOB.24 with pinCMx 52 on package pin 23 */
#define Key_Down_PORT                                                    (GPIOB)
#define Key_Down_PIN                                            (DL_GPIO_PIN_24)
#define Key_Down_IOMUX                                           (IOMUX_PINCM52)
/* Defines for Central: GPIOB.25 with pinCMx 56 on package pin 27 */
#define Key_Central_PORT                                                 (GPIOB)
#define Key_Central_PIN                                         (DL_GPIO_PIN_25)
#define Key_Central_IOMUX                                        (IOMUX_PINCM56)
/* Port definition for Pin Group Track */
#define Track_PORT                                                       (GPIOB)

/* Defines for PIN_0: GPIOB.4 with pinCMx 17 on package pin 52 */
#define Track_PIN_0_PIN                                          (DL_GPIO_PIN_4)
#define Track_PIN_0_IOMUX                                        (IOMUX_PINCM17)
/* Defines for PIN_1: GPIOB.12 with pinCMx 29 on package pin 64 */
#define Track_PIN_1_PIN                                         (DL_GPIO_PIN_12)
#define Track_PIN_1_IOMUX                                        (IOMUX_PINCM29)
/* Defines for PIN_2: GPIOB.17 with pinCMx 43 on package pin 14 */
#define Track_PIN_2_PIN                                         (DL_GPIO_PIN_17)
#define Track_PIN_2_IOMUX                                        (IOMUX_PINCM43)
/* Defines for PIN_3: GPIOB.5 with pinCMx 18 on package pin 53 */
#define Track_PIN_3_PIN                                          (DL_GPIO_PIN_5)
#define Track_PIN_3_IOMUX                                        (IOMUX_PINCM18)
/* Defines for PIN_4: GPIOB.13 with pinCMx 30 on package pin 1 */
#define Track_PIN_4_PIN                                         (DL_GPIO_PIN_13)
#define Track_PIN_4_IOMUX                                        (IOMUX_PINCM30)
/* Defines for PIN_5: GPIOB.19 with pinCMx 45 on package pin 16 */
#define Track_PIN_5_PIN                                         (DL_GPIO_PIN_19)
#define Track_PIN_5_IOMUX                                        (IOMUX_PINCM45)
/* Defines for PIN_6: GPIOB.21 with pinCMx 49 on package pin 20 */
#define Track_PIN_6_PIN                                         (DL_GPIO_PIN_21)
#define Track_PIN_6_IOMUX                                        (IOMUX_PINCM49)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_MotorB_init(void);
void SYSCFG_DL_MotorA_init(void);
void SYSCFG_DL_Buzzer_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_WIT_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
