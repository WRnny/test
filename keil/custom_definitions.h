/*
 * custom_definitions.h
 *
 * 自定义定义文件 - 这个文件不会被SysConfig覆盖
 * 包含所有需要手动添加的硬件定义
 */

#ifndef CUSTOM_DEFINITIONS_H
#define CUSTOM_DEFINITIONS_H

#include "ti_msp_dl_config.h"

/* 电机控制端口定义 */
#define motor_PIN_motorA1_PORT                                   (GPIOB)
#define motor_PIN_motorA2_PORT                                   (GPIOB)
#define motor_PIN_motorB1_PORT                                   (GPIOB)
#define motor_PIN_motorB2_PORT                                   (GPIOB)

/* DMA channel definition for WIT sensor */
#define DMA_WIT_CHAN_ID                                                      (0)

#endif /* CUSTOM_DEFINITIONS_H */