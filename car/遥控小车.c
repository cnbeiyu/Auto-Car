#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit IN1=P1^1;	//置1是左边两个轮子向后转，置0停止                
sbit IN2=P1^2;	//置1是左边两个轮子向前转，置0停止	
sbit IN3=P1^3;	//置1是右边两个轮子向前转，置0停止 	
sbit IN4=P1^5;	//置1是右边两个轮子向后转，置0停止 
sbit Trig=P3^2;	//触发测距（发送）
sbit Echo=P3^3;	//回响信息（接收）
uchar i,num,temp,temp2;
uchar flag=0;
////////////////////延时函数（ms）//////////////////////
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
///////////////////	延时函数（us）/////////////////////
void delayus(uint xus)
{
	uint i;
	for(i=0;i<xus;i++)
	{
		_nop_();	//12MHZ晶振延时1us
	}
}
/////////////////////超声波初始化///////////////////
//void C_init()
//{
//	Trig=0;	//初始化为0
//	Echo=0;	//初始化为0
//	Trig=1;	//拉高Trig		
//	delayus(10);	//延时10us
//}
////////////////////超声波接收/////////////////////
//void C_output()
//{
//	
//}
////////////////////串口初始化///////////////////
void open()//打开串口通信
{
	SCON=0x50;//串口设置为工作方式2
	TMOD=0x21;//设置定时器1为工作方式2

	TH1=0xfd;//T1定时器装初值，波特率为9600
	TL1=0xfd;//T1定时器装初值，波特率为9600
	TR1=1;//打开定时器1
	//TR0=1;
	//EA=1;
	//ET0=1;
}

////////////////////小车移动/////////////////////
void go_on()//控制垃圾桶前进
{
	for(i=0;i<5;i++) //占空比，控制速度为半速
	{
		IN1=0;IN2=1;
		IN3=1;IN4=0;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}
}
void hou_on()//控制垃圾桶后退
{
	for(i=0;i<5;i++)//占空比，控制速度为半速
	{
		IN1=1;IN2=0;
		IN3=0;IN4=1;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}	
}
void youzhuan()//控制垃圾桶右转
{
	for(i=0;i<5;i++)//占空比，控制速度为半速
	{
		IN1=0;IN2=0;
		IN3=1;IN4=0;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}
}
void zuozhuan()//控制垃圾桶左转
{
	for(i=0;i<5;i++)//占空比，控制速度为半速
	{
		IN1=0;IN2=1;
		IN3=0;IN4=0;
		delay(10);
		IN1=0;IN2=0;
		IN3=0;IN4=0;
		delay(10);
	}
}
void stop()////控制垃圾桶停止
{
	IN1=0;IN2=0;
	IN3=0;IN4=0;
}

//////////////////////蓝牙接收信号//////////////
void get() //接收数据
{
	if(RI==1)//判断是否接收完成
	{
		RI=0; //接收完成，清零
		flag=1;
//		switch(SBUF)//根据信号选择操作
//		{
//			case 0x00:n=1;break;	 //前进
//			case 0x01:n;break;	 //后退
//			case 0x02:zuozhuan();break;//左转
//			case 0x03:youzhuan();break;//右转
//			case 0x04:stop();break;		 //停止
//		}	
			
	}	
	if(flag==1)
	{

			EA=1;ET0=1;TR0=1;
			switch(SBUF)//根据信号选择操作
			{
				case 0x00:go_on();break;	 //前进
				case 0x01:hou_on();break;	 //后退
				case 0x02:zuozhuan();break;//左转
				case 0x03:youzhuan();break;//右转
				case 0x04:stop();break;		 //停止
				//case 0x05:youzhuan();delay(1200);stop();break;
			}	
	}
}

///////////////////////主函数///////////////////
void main()
{
	stop();	//小车初始状态：停止
	SBUF=0x04;
	open();	//打开串口通信	
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	while(1)	
		get();//获取数据，小车移动
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
