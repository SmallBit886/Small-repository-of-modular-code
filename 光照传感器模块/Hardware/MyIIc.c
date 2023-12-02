#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MyIIc.h"
void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);//BitAction:ö��
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);
    Delay_us(10);
}

/***********************************************************
*@fuction	:MyI2C_R_SDA
*@brief		:��SDA�ߵĵ�ƽ
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
*@brief		:��ʼ��
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//��©���(�������Ҳ������)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_8| GPIO_Pin_9);		//�ߵ�ƽ��IIC���ߴ��ڿ���״̬
}

/***********************************************************
*@fuction	:MyI2C_Start
*@brief		:��ʼ����
*@param		:--
*@return	:void
*@author	:--�Ȱ�SCL��SDA���ͷţ����1��������SDA��������SCL��������ʼ�������ظ���ʼ����
*@date		:2023-07-24
***********************************************************/

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);		//������SDA
    MyI2C_W_SCL(0);		//������SCL
}

/***********************************************************
*@fuction	:MyI2C_Stop
*@brief		:��ֹ����
*@param		:--
*@return	:void
*@author	:--��������SDA�����ͷ�SCL�����ͷ�SDA
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
*@brief		:����һ���ֽ�
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)	//ѭ��8��
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));//ȡ����Byte�����λxxxx xxxx & 1000 0000 = x000 0000 = 0x80(1) | 0x00(0)
        MyI2C_W_SCL(1);		
        MyI2C_W_SCL(0);
    }
	 MyI2C_W_SCL(1);		
     MyI2C_W_SCL(0);
}

/***********************************************************
*@fuction	:MyI2C_ReceiveByte
*@brief		:����һ���ֽ�
*@param		:--
*@return	:uint8_t Byte
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);	//�����ͷ�SDA//�ӻ������ݷŵ�SDA
    for (i = 0; i < 8; i ++)	//ѭ��8��
    {
        MyI2C_W_SCL(1);		//�ߵ�ƽ��ȡ
        if (MyI2C_R_SDA() == 1)//�ж�SDA�����Ƿ�Ϊ�ߵ�ƽ
        {
            Byte |= (0x80 >> i);//���Ϊ�ߵ�ƽ�����ߵ�ƽд��Byte�����Ǹߵ�ƽ����Ҫ�޸ģ�Byte��ʼֵĬ�ϵ͵�ƽ��
        }
        MyI2C_W_SCL(0);		//����SCL���ӻ�����д��
    }
    return Byte;
}

/***********************************************************
*@fuction	:MyI2C_SendAck
*@brief		:����һ��Ӧ���ֽ�
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);//������Ӧ��λ�ŵ�SDA��
    MyI2C_W_SCL(1);//����SCL���ӻ���ȡ
    MyI2C_W_SCL(0);//����SCL��������һ��ʱ��Ԫ
}

/***********************************************************
*@fuction	:MyI2C_ReceiveAck
*@brief		:����Ӧ��
*@param		:--
*@return	:uint8_t AckBit
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);		//�����ͷ�SDA
    MyI2C_W_SCL(1);		//����SCL����ȡSDA����
    AckBit = MyI2C_R_SDA();//��ȡ���ݷŵ�AckBit��
    MyI2C_W_SCL(0);		//����SCL��������һ��ʱ��Ԫ
    return AckBit;
}

