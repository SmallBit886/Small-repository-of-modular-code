#ifndef __BH1750_H__
#define __BH1750_H__
#include "stm32f10x.h"
//BH1750�ĵ�ַ//0100 0110
#define BH1750_Addr			0x46//ADDR��GNDʱ��������ַ

//BH1750ָ����
#define POWER_OFF			0x00//�ر�ģ��
#define POWER_ON			0x01//��ģ��ȴ���������
#define MODULE_RESET		0x07//�������ݼĴ���ֵ��PowerOnģʽ����Ч
#define	CONTINUE_H_MODE		0x10// �����߷ֱ���ģʽ,1lx,120ms
#define CONTINUE_H_MODE2	0x11// �����߷ֱ���ģʽ.5lx,120ms
#define CONTINUE_L_MODE		0x13// �����ͷֱ���ģʽ,4lx,16ms
#define ONE_TIME_H_MODE		0x20// һ�θ߷ֱ���ģʽ,1lx,120ms,������ģ��ת��PowerDownģʽ
#define ONE_TIME_H_MODE2	0x21// һ�θ߷ֱ���ģʽ,0.5lx,120ms,������ģ��ת��PowerDownģʽ
#define ONE_TIME_L_MODE		0x23// һ�εͷֱ���ģʽ,4lx,16ms,������ģ��ת��PowerDownģʽ

//����ģʽ
#define Measure_Mode			CONTINUE_H_MODE

//�ֱ���	����ǿ�ȣ���λlx��=��High Byte  + Low Byte��/ 1.2 * ��������
#if ((Measure_Mode==CONTINUE_H_MODE2)|(Measure_Mode==ONE_TIME_H_MODE2))
	#define Resolurtion		0.5
#elif ((Measure_Mode==CONTINUE_H_MODE)|(Measure_Mode==ONE_TIME_H_MODE))
	#define Resolurtion		1
#elif ((Measure_Mode==CONTINUE_L_MODE)|(Measure_Mode==ONE_TIME_L_MODE))
	#define Resolurtion		4
#endif

#define BH1750_I2C_WR	0		/* д����bit */
#define BH1750_I2C_RD	1		/* ������bit */


uint8_t i2c_CheckDevice(uint8_t Address);
void BH1750_Init(void);			//δ����IIC��ʼ��
float LIght_Intensity(void);	//��ȡ����ǿ�ȵ�ֵ
uint8_t BH1750_Byte_Write(uint8_t data);
uint16_t BH1750_Read_Measure(void);
void BH1750_Power_ON(void);
void BH1750_Power_OFF(void);
void BH1750_RESET(void);


#endif

