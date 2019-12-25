#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
#include "pt100.h"

#define uchar unsigned char
	    
int PWM_Count=0;		                                    //PWM波计数器				
	
void init_timer0( void )					                      //定时器0初始化					                 
   {                        									      
		PWM=1;                                              //开始时处于不加热状态		 
		PT0=1;       
		PT1=0;       
		TMOD=0x20;                                          //定时器0于方式1,定时器1于方式2		
		TH0=0xB1;                                           //定时器0装初值
    TL0=0xE0;                                           //定时20ms
	  ET0=1;                                              //允许定时器0中断
    TR0=1;                                              //启动定时器0中断	 
		SCON = 0x5a;                                        //8 bit data ,no parity bit
	  TH1 = TL1 = 0xff;                                   //Set Uart baudrate
	  TR1 = 1;                                            //T1 start running		
		EA=1; 	                                            //总中断开关	 	
   } 

void timer0 () interrupt 1 
{   
		TR0=0;
		TH0=0xB1;                                            //定时器重新装初值
		TL0=0xE0;    
		PWM_Count++;                                         //PWM波计数器加1
		if(PWM_Count>=Time_On)	                             //当PWM波计数器大于计算值时停止加热
			      PWM=1;
		else					                                       //当PWM波计数器小于计算值时加热
			      PWM=0;							
		if(PWM_Count>=100)                                   //当PWM波计数器大于设定周期时将PWM计数器清零
			 {
				    PWM_Count=0;
			 }
		TR0=1;
}