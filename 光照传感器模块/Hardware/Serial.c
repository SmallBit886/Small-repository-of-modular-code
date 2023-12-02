#include "stm32f10x.h"                
#include <stdio.h>						//移植printf：默认输出到屏幕
#include <stdarg.h>

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_Init
  ** @brief         : 串口1的初始化
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启串口1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*******************初始化GPIO*********************/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART1_TX：输出脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*******************初始化串口*********************/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;						//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//硬件流控制：不使用流控模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx;					//发送模式
	USART_InitStructure.USART_Parity = USART_Parity_No;				//校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_SendByte
  ** @brief         : 串口发送1个字节
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待标志位（发送寄存器数据空）
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_SendArray
  ** @brief         : 串口发送1个数组
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_SendString
  ** @brief         : 串口发送字符串
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)	//等于0就跳出循环 【'\0'转义字符0】
	{
		Serial_SendByte(String[i]);
	}
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_Pow
  ** @brief         : 次方函数
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_SendNumber
  ** @brief         : 串口发送字符串形式的数字
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{	
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	
	}
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : fputc
  ** @brief         : 重定向到串口
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : fputc是pribtf的底层
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : Serial_Printf
  ** @brief         : 封装sprinf
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : sprinf只能接收直接写的参数
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;					//定义一个参数列表变量
	va_start(arg, format);			//从format位置开始接收参数列表，放在arg中
	vsprintf(String, format, arg);	//vsprintf针对封装格式
	va_end(arg);					//释放参数列表
	Serial_SendString(String);		//发送字符串
}
