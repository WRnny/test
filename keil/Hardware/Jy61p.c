#include "Jy61p.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

/*
 *输入解锁指令  0XFF 0XAA 0X69 0X88 0XB5
 *保存指令      0XFF 0XAA 0X00 0X00 0X00

*/


//指令集
uint8_t  JY61P_ULOCK_CMD[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5}; //解锁
uint8_t  JY61P_BAUD_CMD [5] = {0xFF, 0xAA, 0x04, 0x02, 0x00}; //波特率修改为115200-0X06  9600-0X02
uint8_t  JY61P_SAVE_CMD [5] = {0xFF, 0xAA, 0x00, 0x00, 0x00}; //保存
uint8_t  JY61P_XY0_CMD  [5] = {0xFF, 0xAA, 0x01, 0x08, 0x00}; //XY角度归零
uint8_t  JY61P_Z0_CMD   [5] = {0xFF, 0xAA, 0x01, 0x04, 0x00}; //Z轴归零
uint8_t  JY61P_XY0_XYZOK  [5] = {0xFF, 0xAA, 0x61, 0x00, 0x00}; //陀螺仪自动校准  00表示开启默认是开启的，01表示关闭陀螺仪自动校准。
uint8_t  JY61P_Z0_CHIOK1   [5] = {0xFF, 0xAA, 0x01, 0x07, 0x00}; //进入磁场校准
uint8_t  JY61P_Z0_CHIOK2   [5] = {0xFF, 0xAA, 0x01, 0x00, 0x00}; //退出磁场校准
uint8_t  JY61P_Z0_MODE1   [5] = {0xFF, 0xAA, 0x24, 0x01, 0x00}; //6轴
uint8_t  JY61P_Z0_MODE2   [5] = {0xFF, 0xAA, 0x24, 0x00, 0x00}; //9轴
uint8_t  JY61P_Z0_200KHZ   [5] = {0xFF, 0xAA, 0x03, 0x09, 0x00}; //速率  100-0X09 200-0X0B


float Pith,Roll,Yaw; //定义陀螺仪角度输出数据

//void JY901S_Init()
//{
//	NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
//	NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
//}

//void Serial_SendByte1(uint8_t Byte)
//{
//    DL_UART_Main_transmitData(UART_1_INST, Byte);        //将字节数据写入数据寄存器，写入后 USART 自动生成时序波形
//   // while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);   //等待发送完成
//    /*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
//}