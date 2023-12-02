#ifndef __BH1750_H__
#define __BH1750_H__
#include "stm32f10x.h"
//BH1750的地址//0100 0110
#define BH1750_Addr			0x46//ADDR接GND时的器件地址

//BH1750指令码
#define POWER_OFF			0x00//关闭模块
#define POWER_ON			0x01//打开模块等待测量命令
#define MODULE_RESET		0x07//重置数据寄存器值在PowerOn模式下有效
#define	CONTINUE_H_MODE		0x10// 连续高分辨率模式,1lx,120ms
#define CONTINUE_H_MODE2	0x11// 连续高分辨率模式.5lx,120ms
#define CONTINUE_L_MODE		0x13// 连续低分辨率模式,4lx,16ms
#define ONE_TIME_H_MODE		0x20// 一次高分辨率模式,1lx,120ms,测量后模块转到PowerDown模式
#define ONE_TIME_H_MODE2	0x21// 一次高分辨率模式,0.5lx,120ms,测量后模块转到PowerDown模式
#define ONE_TIME_L_MODE		0x23// 一次低分辨率模式,4lx,16ms,测量后模块转到PowerDown模式

//测量模式
#define Measure_Mode			CONTINUE_H_MODE

//分辨率	光照强度（单位lx）=（High Byte  + Low Byte）/ 1.2 * 测量精度
#if ((Measure_Mode==CONTINUE_H_MODE2)|(Measure_Mode==ONE_TIME_H_MODE2))
	#define Resolurtion		0.5
#elif ((Measure_Mode==CONTINUE_H_MODE)|(Measure_Mode==ONE_TIME_H_MODE))
	#define Resolurtion		1
#elif ((Measure_Mode==CONTINUE_L_MODE)|(Measure_Mode==ONE_TIME_L_MODE))
	#define Resolurtion		4
#endif

#define BH1750_I2C_WR	0		/* 写控制bit */
#define BH1750_I2C_RD	1		/* 读控制bit */


uint8_t i2c_CheckDevice(uint8_t Address);
void BH1750_Init(void);			//未包含IIC初始化
float LIght_Intensity(void);	//读取光照强度的值
uint8_t BH1750_Byte_Write(uint8_t data);
uint16_t BH1750_Read_Measure(void);
void BH1750_Power_ON(void);
void BH1750_Power_OFF(void);
void BH1750_RESET(void);


#endif

