#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
//#include "pid.h"
#include "pt100.h"

unsigned int   ADC = 0;                                             //ADC����ֵ

/*******************************************************************************
* ������         : Delay
* ��������		   : ��ʱ����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Delay(unsigned int n)                                        //��ʱ����               
{
	unsigned int x;
	while (n--)
		{
				x = 5000;
				while (x--);
		}
}

/*******************************************************************************
* ������         : InitADC
* ��������		   : ADC��ʼ��
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void InitADC( )                                                  //ADC��ʼ��
{
	P1ASF = 0x01;                                                  //��P1�����ó�ģ���������
	ADC_RES = 0;                                                   //����ADCֵ����λ��
	ADC_RESL =0;                                                   //ADCֵ����λ��
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START ;              //����A/Dת���������⹦�ܼĴ���
	Delay(2);                                                      //ADC������ʱ
}


/*******************************************************************************
* ������         : SendData
* ��������		   : ��ADC�ɼ�ֵ����SBUF�Ĵ�����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void SendData(unsigned int dat)                                  //��ADC�ɼ�ֵ����SBUF�Ĵ�����
{
	while (!TI);                                                   //�ȴ����ݵ���
	TI = 0;                                                        //�����־λ
	SBUF = dat;                                                    //��ADCֵ����SBUF��
}

/*******************************************************************************
* ������         : adc_isr
* ��������		   : ADC�жϷ�����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void adc_isr() interrupt 5 using 1                               //ADC�жϷ�����
{
	unsigned int ADCH;                                             //ADC��8λ                                              
	unsigned int ADCL;                                             //ADC��2λ
	ADC_CONTR &= !ADC_FLAG;                                        //���ADC�жϱ�־λ
	SendData(ADC_RES);                                             //��ADC�ĸ�8λ����SBUF��
  SendData(ADC_RESL);                                            //��ADC��2λ����SBUF��	
	ADCH=ADC_RES;                                                  //��ADC�ĸ�8λ����ADCH�� 
	ADCL=ADC_RESL;                                                 //��ADC�ĵ�2λ����ADCL��
	ADCH=ADCH<<2;                                                  //��ADCde1��8λ����2λ
	ADC=ADCH+ADCL;                                                 //����ADCֵ    
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START ;              //����A/Dת���������⹦�ܼĴ���
}
