#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>
#include <stdarg.h>
#include "ti_msp_dl_config.h"  // MSP430Çý¶¯¿â
 
void Serial_Sendstr(char* str);
int Serial_Pow(int X, int Y);
void Serial_SendNum(int Number, int Length);
void Serial_SendByte(uint8_t Byte);
void VOFA_SendData(float* data, int count);


#endif
