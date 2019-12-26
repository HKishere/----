#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
//#include "pid.h"
#include "pt100.h"
#include "Uart.h"

#define uint unsigned int
#define uchar unsigned char

uchar Buffer[5];
uchar i=0,flag;
int set_temperature=80;                            //�¶��趨�ھ���
float gxlb_wendu=0;                                //�����˲�����¶�
float av_wendu=0;                                  //����50�κ���ƽ��ֵ
float gc_wendu=0;                                  //���̼������¶�
unsigned int fd_wendu=0;                           //�����̼������¶ȷŴ�100��
unsigned int lvbo[50]={0};                         //ƽ���˲�����
unsigned char L=0;                                 //ƽ���˲��������
unsigned int sum=0;                                //�������
char count;                                        //�������
float  KP0=30;                                     //PID�㷨�б���ϵ��
float  KI0=0.7;                                    //PID�㷨�л���ϵ��
float  KD0=80;                                     //PID�㷨��΢��ϵ��
float  Error=0;                                    //PID�㷨�е�ǰƫ��
float  PreError=0;                                 //PID�㷨���ϴ�ƫ��
float  PPreError=0;                                //PID�㷨�����ϴ�ƫ��
float  Det=0;                                      //PID�㷨������
int Time_On=50;                                    //PWM��������
uint  send_count;
/*******************************************************************************
* ������         : junzhi_lvbo_10
* ��������		   : AD�����¶���ֵƽ��ֵ�˲��������˲���������ת������
* ����           : AD�����¶���ֵ
* ���         	 : ��ʾ������ֵ�����϶�
* ע��		       ������ʾ�¶���ʵ���¶������ʱ������ڹ�����ת������
*******************************************************************************/
void junzhi_lvbo_10(unsigned int A)
{
	lvbo[L++]=A;
	if(L==50)
	{
	L=0;
	for(count=0,sum=0;count<50;count++)
		{
			sum+=lvbo[count];
		}
		  av_wendu = sum/50;                                        //��ƽ����
		  gxlb_wendu	=av_wendu * 0.7 + gxlb_wendu *0.3;          //�����˲�
		  gc_wendu=(gxlb_wendu*100/1024-30)/0.72;                   //������ת��1
		  fd_wendu=gc_wendu * 100 ;                                 //�Ŵ�100��
	}
}

/*******************************************************************************
* ������         : PID
* ��������		 : PID�㷨��
* ����           : Time_OnԭʼPWM����
* ���         	 : Time_On������PWM����
*******************************************************************************/		
void PID()
{
	PPreError=PreError;                                                  //���ϴ�ƫ������ϴ�ƫ��     
	PreError=Error;                                                      //����ǰƫ����ϴ�ƫ��
	Error=set_temperature-gc_wendu;                                      //����ƫ��
	Det=KP0*(Error-PreError)+KI0*Error+KD0*(Error-2*PreError+PPreError); //����ʽPID���㹫ʽ����u(k)=Kp*(e(k)-e(k-1))+Ki*e(k)+Kd*(e(k)-2*e(k-1)+e(k-2))
	Time_On=Time_On+Det;                                                 //u(k)=u(k)+��u(k)
	if(Time_On>100)	Time_On=100;                                         //�������������趨����               
	if(Time_On<0)	Time_On=0;                                           //��������������Сռ�ձ�
}



/*******************************************************************************
* ������         : main
* ��������		 : ���������
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void main()
{
		LcdInit();                                                          //Һ������ʼ��                          
		DisplayChar(0,0,"set:");
		Delay1ms(10);                                                       //��ʱ5����
		DisplayChar(1,0,"now:");
		Delay1ms(10);                                                       //��ʱ5����
		DisplayChar(0,8,"kishere");
		Delay1ms(10);                                                       //��ʱ5����
		LCD_dis_data3(1,4,set_temperature);                                 //���ִ����һ�е����У���ʾ�¶ȸ���ֵ
		Delay1ms(10);                                                       //��ʱ5����
		init_timer0();	                                                    //��Ƭ����ʱ����ʼ��
		InitADC();                                                          //AD������ʼ��
		EA=1;                                                               //���жϿ���
		EADC=1;	                                                            //ADC�ж�ʹ��λ
		while(1)
		{
			send_count++;
			junzhi_lvbo_10(ADC);	  //ADת�������˲�
			Display1(2, 4, fd_wendu); //��ʾ��ǰ�¶�
			Delay1ms(1);
			PID();					  //����PID����
			//��һ���¶�����
			//Init_Uart();
			//if (send_count > 10000)
			//{
			//	send_str(fd_wendu);
			//}
			//Delay1ms(1);
		}	
}

