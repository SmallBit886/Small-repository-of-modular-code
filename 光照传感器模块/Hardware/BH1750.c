#include "bh1750.h"
#include "MyIIc.h"
#include "Delay.h"
/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t Address)
{
	uint8_t Ack;	//定义变量接收应答
	MyI2C_Start();	//发送起始信号
	MyI2C_SendByte(Address);//发送写地址
	Ack = MyI2C_ReceiveAck();	//接收从机应答
	MyI2C_Stop();			//发送停止信号
	return Ack;
}

//BH1750写一个字节
//返回值	成功：0		失败：1 
uint8_t BH1750_Byte_Write(uint8_t data)
{
	MyI2C_Start();	//发送起始信号
	MyI2C_SendByte(0x46);//发送写地址
	if(MyI2C_ReceiveAck()==1)return 1;	//如果应答为1，则失败返回1
	MyI2C_SendByte(data);//发送控制命令
	if(MyI2C_ReceiveAck()==1)return 1;//如果应答为1，则失败返回1
	MyI2C_Stop();	//发送停止信号
	return 0;
}

//BH1750读取测量数据
//返回值 成功：返回光照强度 	失败：返回0
uint16_t BH1750_Read_Measure(void)
{
	uint16_t receive_data=0; 
	MyI2C_Start();	//发送起始信号
	MyI2C_SendByte(0x47);//发送读地址
	if(MyI2C_ReceiveAck()==1)return 0;//如果为1，则失败
	receive_data=MyI2C_ReceiveByte();//读取高八位
	MyI2C_SendAck(0);	//主机发送0，则继续读取
	receive_data=(receive_data<<8)+MyI2C_ReceiveByte();//读取低八位
	MyI2C_SendAck(1);	//主机发送1，则结束读取
	MyI2C_Stop();	//发送停止信号
	return receive_data;	//返回读取到的数据
}


//BH1750上电
void BH1750_Power_ON(void)
{
	BH1750_Byte_Write(POWER_ON);
}

//BH1750断电
void BH1750_Power_OFF(void)
{
	BH1750_Byte_Write(POWER_OFF);
}

//BH1750复位 (仅在上电时有效)
void BH1750_RESET(void)
{
	BH1750_Byte_Write(MODULE_RESET);
}

//BH1750初始化
void BH1750_Init(void)
{
	
	BH1750_Power_ON();	//BH1750s上电
	BH1750_RESET();
	BH1750_Byte_Write(Measure_Mode);//配置BH1750模式
	Delay_ms(150);
}

//获取光照强度
float LIght_Intensity(void)
{
	return (float)(BH1750_Read_Measure()/1.1f*Resolurtion);
}

