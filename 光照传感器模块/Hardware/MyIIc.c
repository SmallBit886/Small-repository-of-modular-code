#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MyIIc.h"
void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);//BitAction:枚举
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);
    Delay_us(10);
}

/***********************************************************
*@fuction	:MyI2C_R_SDA
*@brief		:读SDA线的电平
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
    Delay_us(10);
    return BitValue;
}

/***********************************************************
*@fuction	:MyI2C_Init
*@brief		:初始化
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//开漏输出(既能输出也能输入)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_8| GPIO_Pin_9);		//高电平：IIC总线处于空闲状态
}

/***********************************************************
*@fuction	:MyI2C_Start
*@brief		:起始条件
*@param		:--
*@return	:void
*@author	:--先把SCL和SDA都释放：输出1；先拉低SDA；再拉低SCL：兼容起始条件和重复起始条件
*@date		:2023-07-24
***********************************************************/

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);		//先拉低SDA
    MyI2C_W_SCL(0);		//再拉低SCL
}

/***********************************************************
*@fuction	:MyI2C_Stop
*@brief		:终止条件
*@param		:--
*@return	:void
*@author	:--：先拉低SDA；再释放SCL；在释放SDA
*@date		:2023-07-24
***********************************************************/

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/***********************************************************
*@fuction	:MyI2C_SendByte
*@brief		:发送一个字节
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)	//循环8次
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));//取决于Byte的最高位xxxx xxxx & 1000 0000 = x000 0000 = 0x80(1) | 0x00(0)
        MyI2C_W_SCL(1);		
        MyI2C_W_SCL(0);
    }
	 MyI2C_W_SCL(1);		
     MyI2C_W_SCL(0);
}

/***********************************************************
*@fuction	:MyI2C_ReceiveByte
*@brief		:接收一个字节
*@param		:--
*@return	:uint8_t Byte
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);	//主机释放SDA//从机把数据放到SDA
    for (i = 0; i < 8; i ++)	//循环8次
    {
        MyI2C_W_SCL(1);		//高电平读取
        if (MyI2C_R_SDA() == 1)//判断SDA数据是否为高电平
        {
            Byte |= (0x80 >> i);//如果为高电平，将高电平写入Byte，不是高电平则不需要修改（Byte初始值默认低电平）
        }
        MyI2C_W_SCL(0);		//拉低SCL，从机继续写入
    }
    return Byte;
}

/***********************************************************
*@fuction	:MyI2C_SendAck
*@brief		:发送一个应答字节
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);//主机把应答位放到SDA上
    MyI2C_W_SCL(1);//拉高SCL，从机读取
    MyI2C_W_SCL(0);//拉低SCL，进入下一个时序单元
}

/***********************************************************
*@fuction	:MyI2C_ReceiveAck
*@brief		:接收应答
*@param		:--
*@return	:uint8_t AckBit
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);		//主机释放SDA
    MyI2C_W_SCL(1);		//拉高SCL，读取SDA数据
    AckBit = MyI2C_R_SDA();//读取数据放到AckBit上
    MyI2C_W_SCL(0);		//拉低SCL，进入下一个时序单元
    return AckBit;
}

