#include "bh1750.h"
#include "MyIIc.h"
#include "Delay.h"
/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t Address)
{
	uint8_t Ack;	//�����������Ӧ��
	MyI2C_Start();	//������ʼ�ź�
	MyI2C_SendByte(Address);//����д��ַ
	Ack = MyI2C_ReceiveAck();	//���մӻ�Ӧ��
	MyI2C_Stop();			//����ֹͣ�ź�
	return Ack;
}

//BH1750дһ���ֽ�
//����ֵ	�ɹ���0		ʧ�ܣ�1 
uint8_t BH1750_Byte_Write(uint8_t data)
{
	MyI2C_Start();	//������ʼ�ź�
	MyI2C_SendByte(0x46);//����д��ַ
	if(MyI2C_ReceiveAck()==1)return 1;	//���Ӧ��Ϊ1����ʧ�ܷ���1
	MyI2C_SendByte(data);//���Ϳ�������
	if(MyI2C_ReceiveAck()==1)return 1;//���Ӧ��Ϊ1����ʧ�ܷ���1
	MyI2C_Stop();	//����ֹͣ�ź�
	return 0;
}

//BH1750��ȡ��������
//����ֵ �ɹ������ع���ǿ�� 	ʧ�ܣ�����0
uint16_t BH1750_Read_Measure(void)
{
	uint16_t receive_data=0; 
	MyI2C_Start();	//������ʼ�ź�
	MyI2C_SendByte(0x47);//���Ͷ���ַ
	if(MyI2C_ReceiveAck()==1)return 0;//���Ϊ1����ʧ��
	receive_data=MyI2C_ReceiveByte();//��ȡ�߰�λ
	MyI2C_SendAck(0);	//��������0���������ȡ
	receive_data=(receive_data<<8)+MyI2C_ReceiveByte();//��ȡ�Ͱ�λ
	MyI2C_SendAck(1);	//��������1���������ȡ
	MyI2C_Stop();	//����ֹͣ�ź�
	return receive_data;	//���ض�ȡ��������
}


//BH1750�ϵ�
void BH1750_Power_ON(void)
{
	BH1750_Byte_Write(POWER_ON);
}

//BH1750�ϵ�
void BH1750_Power_OFF(void)
{
	BH1750_Byte_Write(POWER_OFF);
}

//BH1750��λ (�����ϵ�ʱ��Ч)
void BH1750_RESET(void)
{
	BH1750_Byte_Write(MODULE_RESET);
}

//BH1750��ʼ��
void BH1750_Init(void)
{
	
	BH1750_Power_ON();	//BH1750s�ϵ�
	BH1750_RESET();
	BH1750_Byte_Write(Measure_Mode);//����BH1750ģʽ
	Delay_ms(150);
}

//��ȡ����ǿ��
float LIght_Intensity(void)
{
	return (float)(BH1750_Read_Measure()/1.1f*Resolurtion);
}

