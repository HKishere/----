#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
#include "pt100.h"

#define uchar unsigned char
	    
int PWM_Count=0;		                                    //PWM��������				
	
void init_timer0( void )					                      //��ʱ��0��ʼ��					                 
   {                        									      
		PWM=1;                                              //��ʼʱ���ڲ�����״̬		 
		PT0=1;       
		PT1=0;       
		TMOD=0x20;                                          //��ʱ��0�ڷ�ʽ1,��ʱ��1�ڷ�ʽ2		
		TH0=0xB1;                                           //��ʱ��0װ��ֵ
    TL0=0xE0;                                           //��ʱ20ms
	  ET0=1;                                              //����ʱ��0�ж�
    TR0=1;                                              //������ʱ��0�ж�	 
		SCON = 0x5a;                                        //8 bit data ,no parity bit
	  TH1 = TL1 = 0xff;                                   //Set Uart baudrate
	  TR1 = 1;                                            //T1 start running		
		EA=1; 	                                            //���жϿ���	 	
   } 

void timer0 () interrupt 1 
{   
		TR0=0;
		TH0=0xB1;                                            //��ʱ������װ��ֵ
		TL0=0xE0;    
		PWM_Count++;                                         //PWM����������1
		if(PWM_Count>=Time_On)	                             //��PWM�����������ڼ���ֵʱֹͣ����
			      PWM=1;
		else					                                       //��PWM��������С�ڼ���ֵʱ����
			      PWM=0;							
		if(PWM_Count>=100)                                   //��PWM�������������趨����ʱ��PWM����������
			 {
				    PWM_Count=0;
			 }
		TR0=1;
}