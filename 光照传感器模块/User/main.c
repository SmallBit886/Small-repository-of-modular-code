#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "BH1750.h"
#include "MyIIc.h"
#include "Serial.h"

float Light = 0; //光照度


int main(void)
{
	Serial_Init();
	MyI2C_Init();	//初始化IIC通讯
	LED_Init();
	//OLED_Init();
	
	BH1750_Init();          //初始化BH1750

	if(i2c_CheckDevice(0x46)==0)	//检测设备是否正常
	{
		Serial_SendString("ok");//显示
	}
	else
	{
		Serial_SendString("err");//显示
	}	
	Delay_ms(500);
	while (1)
	{

		Serial_Printf("光照度：%lf\n",LIght_Intensity());//显示光照值		
		Delay_ms(1000);
	}
}
