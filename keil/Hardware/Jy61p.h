#ifndef __JY61P_H
#define __JY61P_H
                  // Device header
#include <stdio.h>
#include "ti_msp_dl_config.h"
//基本串口功能
void JY901S_Init(void);
void Serial_SendByte1(uint8_t Byte);
void Serial_SendArray1(uint8_t *Array, uint16_t Length);
void Serial_SendString1(char *String);
void Serial_SendNumber1(uint32_t Number, uint8_t Length);

//陀螺仪
void JY61P_BAUD(void);
void JY61P_START(void);
void JY61P_XYZOK(void);
void JY61P_CHIOK1(void);
void JY61P_CHIOK2(void);
void JY61P_MODE1(void);
void JY61P_MODE2(void);
void JY61P_200KHZ(void);
float IMU180(float yaw);

float IMU_xianzhi(float yaw) ;
float convertAngle(float angle); //核心算法--陀螺仪数据处理;
	
#endif
