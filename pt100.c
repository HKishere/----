#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
//#include "pid.h"
#include "pt100.h"

unsigned int   ADC = 0;                                             //ADC采样值

/*******************************************************************************
* 函数名         : Delay
* 函数功能		   : 延时函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Delay(unsigned int n)                                        //延时函数               
{
	unsigned int x;
	while (n--)
		{
				x = 5000;
				while (x--);
		}
}

/*******************************************************************************
* 函数名         : InitADC
* 函数功能		   : ADC初始化
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void InitADC( )                                                  //ADC初始化
{
	P1ASF = 0x01;                                                  //将P1口设置成模拟量输入口
	ADC_RES = 0;                                                   //清零ADC值（高位）
	ADC_RESL =0;                                                   //ADC值（低位）
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START ;              //配置A/D转换控制特殊功能寄存器
	Delay(2);                                                      //ADC采样延时
}


/*******************************************************************************
* 函数名         : SendData
* 函数功能		   : 将ADC采集值放入SBUF寄存器内
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void SendData(unsigned int dat)                                  //将ADC采集值放入SBUF寄存器内
{
	while (!TI);                                                   //等待数据到来
	TI = 0;                                                        //清除标志位
	SBUF = dat;                                                    //将ADC值放入SBUF中
}

/*******************************************************************************
* 函数名         : adc_isr
* 函数功能		   : ADC中断服务函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void adc_isr() interrupt 5 using 1                               //ADC中断服务函数
{
	unsigned int ADCH;                                             //ADC高8位                                              
	unsigned int ADCL;                                             //ADC低2位
	ADC_CONTR &= !ADC_FLAG;                                        //清除ADC中断标志位
	SendData(ADC_RES);                                             //将ADC的高8位放入SBUF中
  SendData(ADC_RESL);                                            //将ADC低2位放入SBUF中	
	ADCH=ADC_RES;                                                  //将ADC的高8位放入ADCH中 
	ADCL=ADC_RESL;                                                 //将ADC的低2位放入ADCL中
	ADCH=ADCH<<2;                                                  //将ADCde1高8位左移2位
	ADC=ADCH+ADCL;                                                 //计算ADC值    
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START ;              //配置A/D转换控制特殊功能寄存器
}
