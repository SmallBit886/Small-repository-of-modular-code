#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"                  // Device header

#include <stdio.h>		//头文件里有printf的声明

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

#endif
