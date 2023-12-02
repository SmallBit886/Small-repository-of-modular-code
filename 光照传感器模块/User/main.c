#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "BH1750.h"
#include "MyIIc.h"
#include "Serial.h"

float Light = 0; //���ն�


int main(void)
{
	Serial_Init();
	MyI2C_Init();	//��ʼ��IICͨѶ
	LED_Init();
	//OLED_Init();
	
	BH1750_Init();          //��ʼ��BH1750

	if(i2c_CheckDevice(0x46)==0)	//����豸�Ƿ�����
	{
		Serial_SendString("ok");//��ʾ
	}
	else
	{
		Serial_SendString("err");//��ʾ
	}	
	Delay_ms(500);
	while (1)
	{

		Serial_Printf("���նȣ�%lf\n",LIght_Intensity());//��ʾ����ֵ		
		Delay_ms(1000);
	}
}
