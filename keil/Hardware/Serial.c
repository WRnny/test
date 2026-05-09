#include "Serial.h"
#include <stdlib.h>
#include <math.h>

void Serial_Sendstr(char* str)
{
	while(*str != 0){
		
//		//检测串口状态是否忙碌
//		while(DL_UART_isBusy(UART_0_INST) == true);
//		//如果状态忙碌，可能会导致堵塞，堵塞之后可能只会发一个字符
//		DL_UART_transmitData(UART_0_INST, *str);DL_UART_transmitData
		
		//这个可以避免阻塞情况
		DL_UART_transmitDataBlocking(UART_0_INST, *str);
		str++;
	}
}

int Serial_Pow(int X, int Y)
{
	int Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}


void Serial_SendNum(int Number, int Length)
{
	int i = 0;
	for(i = 0; i < Length; i++)
	{
		DL_UART_transmitDataBlocking(UART_0_INST, Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

void Serial_SendByte(uint8_t Byte)
{
	DL_UART_transmitDataBlocking(UART_0_INST, Byte);
}

void VOFA_SendData(float* data, int count)
{
    // 发送数据	
	for(int i = 0; i < count; i++)
	{
		unsigned char *bytes = (unsigned char *)&data[i];
		for(int j = 0; j < 4; j++)
		{
			Serial_SendByte(bytes[j]);	
		}
	}
	
	//发送帧尾
	unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};
	for(int i = 0; i < 4; i++)
	{
		Serial_SendByte(tail[i]);
	}
}


