#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit IN1=P1^1;	//��1����������������ת����0ֹͣ                
sbit IN2=P1^2;	//��1���������������ǰת����0ֹͣ	
sbit IN3=P1^3;	//��1���ұ�����������ǰת����0ֹͣ 	
sbit IN4=P1^5;	//��1���ұ������������ת����0ֹͣ 
sbit Trig=P3^2;	//������ࣨ���ͣ�
sbit Echo=P3^3;	//������Ϣ�����գ�
uchar i,num,temp,temp2;
uchar flag=0;
////////////////////��ʱ������ms��//////////////////////
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
///////////////////	��ʱ������us��/////////////////////
void delayus(uint xus)
{
	uint i;
	for(i=0;i<xus;i++)
	{
		_nop_();	//12MHZ������ʱ1us
	}
}
/////////////////////��������ʼ��///////////////////
//void C_init()
//{
//	Trig=0;	//��ʼ��Ϊ0
//	Echo=0;	//��ʼ��Ϊ0
//	Trig=1;	//����Trig		
//	delayus(10);	//��ʱ10us
//}
////////////////////����������/////////////////////
//void C_output()
//{
//	
//}
////////////////////���ڳ�ʼ��///////////////////
void open()//�򿪴���ͨ��
{
	SCON=0x50;//��������Ϊ������ʽ2
	TMOD=0x21;//���ö�ʱ��1Ϊ������ʽ2

	TH1=0xfd;//T1��ʱ��װ��ֵ��������Ϊ9600
	TL1=0xfd;//T1��ʱ��װ��ֵ��������Ϊ9600
	TR1=1;//�򿪶�ʱ��1
	//TR0=1;
	//EA=1;
	//ET0=1;
}

////////////////////С���ƶ�/////////////////////
void go_on()//��������Ͱǰ��
{
	for(i=0;i<5;i++) //ռ�ձȣ������ٶ�Ϊ����
	{
		IN1=0;IN2=1;
		IN3=1;IN4=0;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}
}
void hou_on()//��������Ͱ����
{
	for(i=0;i<5;i++)//ռ�ձȣ������ٶ�Ϊ����
	{
		IN1=1;IN2=0;
		IN3=0;IN4=1;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}	
}
void youzhuan()//��������Ͱ��ת
{
	for(i=0;i<5;i++)//ռ�ձȣ������ٶ�Ϊ����
	{
		IN1=0;IN2=0;
		IN3=1;IN4=0;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}
}
void zuozhuan()//��������Ͱ��ת
{
	for(i=0;i<5;i++)//ռ�ձȣ������ٶ�Ϊ����
	{
		IN1=0;IN2=1;
		IN3=0;IN4=0;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}
}
void stop()////��������Ͱֹͣ
{
	IN1=0;IN2=0;
	IN3=0;IN4=0;
}

//////////////////////���������ź�//////////////
void get() //��������
{
	if(RI==1)//�ж��Ƿ�������
	{
		RI=0; //������ɣ�����
		flag=1;
//		switch(SBUF)//�����ź�ѡ�����
//		{
//			case 0x00:n=1;break;	 //ǰ��
//			case 0x01:n;break;	 //����
//			case 0x02:zuozhuan();break;//��ת
//			case 0x03:youzhuan();break;//��ת
//			case 0x04:stop();break;		 //ֹͣ
//		}	
			
	}	
	if(flag==1)
	{

			EA=1;ET0=1;TR0=1;
			switch(SBUF)//�����ź�ѡ�����
			{
				case 0x00:go_on();break;	 //ǰ��
				case 0x01:hou_on();break;	 //����
				case 0x02:zuozhuan();break;//��ת
				case 0x03:youzhuan();break;//��ת
				case 0x04:stop();break;		 //ֹͣ
				//case 0x05:youzhuan();delay(1200);stop();break;
			}	
	}
}

///////////////////////������///////////////////
void main()
{
	stop();	//С����ʼ״̬��ֹͣ
	SBUF=0x04;
	open();	//�򿪴���ͨ��	
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	while(1)	
		get();//��ȡ���ݣ�С���ƶ�
}
////////////////////////////////////////////////
void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	num++;
	if(num==20)
	{
		flag=0;
		num=0;
	}
}
