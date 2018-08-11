#include "ADXL345.h"
#include "IIC.h"
#include "Delay.h"
#include "LCD1602.h"
uchar BUF[8];                         //�������ݻ�����   
void ADXL345_Write_Single(uchar REG_Address,uchar REG_data)  //���ֽ�д��
{
	Start_I2c();  //����I2C���ݴ���
	SendByte(0xA6); //�����豸��ַ+д�ź�,ȷ��I2C�ϵ��豸
	SendByte(REG_Address);  //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
	SendByte(REG_data);			//�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
	Stop_I2c();  //ֹͣI2C���ݴ���
}
void ADXL345_Init()																					//��ʼ��ADXL345
{
	ADXL345_Write_Single(0x31,0x0B);   //������Χ,����16g��13λģʽ
	ADXL345_Write_Single(0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
	ADXL345_Write_Single(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
	ADXL345_Write_Single(0x2E,0x80);   //ʹ�� DATA_READY �ж�
	ADXL345_Write_Single(0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
	ADXL345_Write_Single(0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
	ADXL345_Write_Single(0x20,0x05);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}

void ADXL345_Read_Multiple(void)										//�����Ķ�ȡ�ڲ��Ĵ�������
{   
	uchar i;
	Start_I2c();                          //��ʼ�ź�
	SendByte(0xA6);            						//�����豸��ַ+д�ź�
	SendByte(0x32);                   		//���ʹ洢��Ԫ��ַ����0x32��ʼ	
	Start_I2c();                          //��ʼ�ź�
	SendByte(0xA7);         							//�����豸��ַ+���ź�
	for(i=0; i<6; i++)                    //������ȡ6����ַ���ݣ��洢����BUF
	{
			BUF[i] = RcvByte();          			//BUF[0]�洢0x32��ַ�е�����
			if(i == 5)
			{
				 Ack_I2c(1);                		//���һ��������Ҫ��NOACK
			}
			else
			{
				 Ack_I2c(0);                			//��ӦACK
		  }
	}
	Stop_I2c();                           //ֹͣ�ź�
	Delay_ms(5);
	BUF[6] = '\0';												//�ַ�����β��־
}
void ADXL345_Display_x(int x)
{
	if(x<0)
	{
		x = -x;
    lcd1602_write_char(10,0,'-');      //��ʾ��������λ
	}
	else
		lcd1602_write_char(10,0,' '); //��ʾ�ո�
	
	x = (float)x * 3.9;
	lcd1602_write_char(8,0,'X');
	lcd1602_write_char(9,0,':'); 
	x = x%10000;   //ȡ������
	lcd1602_write_char(11,0,x/1000+'0'); 
	lcd1602_write_char(12,0,'.'); 
	x = x%1000;    //ȡ������
	lcd1602_write_char(13,0,x/100+'0'); 
	x = x%100;     //ȡ������
	lcd1602_write_char(14,0,x/10+'0'); 
	lcd1602_write_char(15,0,' '); 
}
void ADXL345_Display_y(int y)
{
	if(y<0)
	{
		y=-y;
    lcd1602_write_char(2,1,'-');      //��ʾ��������λ
	}
	else 
		lcd1602_write_char(2,1,' '); //��ʾ�ո�
	
	y = (float)y * 3.9;
	lcd1602_write_char(0,1,'Y');   //��1�У���0�� ��ʾy
	lcd1602_write_char(1,1,':'); 
	y = y%10000;   //ȡ������
	lcd1602_write_char(3,1,y/1000+'0'); 
	lcd1602_write_char(4,1,'.'); 
	y = y%1000;    //ȡ������
	lcd1602_write_char(5,1,y/100+'0'); 
	y = y%100;     //ȡ������
	lcd1602_write_char(6,1,y/10+'0');  
	lcd1602_write_char(7,1,' '); 
}
void ADXL345_Display_z(int z)
{
	if(z<0)
	{
		z=-z;
    lcd1602_write_char(10,1,'-');       //��ʾ������λ
	}
	else 
		lcd1602_write_char(10,1,' ');  //��ʾ�ո�
	
	z = (float)z * 3.9;
	lcd1602_write_char(8,1,'Z');  //��0�У���10�� ��ʾZ
	lcd1602_write_char(9,1,':');
	z = z%10000;     //ȡ������
	lcd1602_write_char(11,1,z/1000+'0'); 
	lcd1602_write_char(12,1,'.'); 
	z = z%1000;     //ȡ������
	lcd1602_write_char(13,1,z/100+'0'); 
	z = z%100;     //ȡ������
	lcd1602_write_char(14,1,z/10+'0'); 
	lcd1602_write_char(15,1,' ');  
}
void ADXL345_Display_xyz()					//��ʾADXL345��������
{
	ADXL345_Display_x((BUF[1]<<8)+BUF[0]);
	
	ADXL345_Display_y((BUF[3]<<8)+BUF[2]);

	ADXL345_Display_z((BUF[5]<<8)+BUF[4]);
}