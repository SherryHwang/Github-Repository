#ifndef __IIC_H__
#define __IIC_H__
#define uchar unsigned char
#define uint unsigned int
void IDefine_Bus(bit SDA_NEW,SDL_NEW);  //����ʱ�Ӻ���������
void Start_I2c(); //�����ߺ���
void Stop_I2c(); //�������ߺ���  
void Ack_I2c(bit a);//Ӧ���Ӻ���
void  SendByte(unsigned char  c);//�ֽ����ݷ��ͺ���            
unsigned char RcvByte();//���ӵ�ַ���ֽ����ݺ��� 
#endif