#ifndef __lcd_H_
#define __lcd_H_

#include <STC12C5A60S2.h>
#define uint unsigned int 
#define uchar unsigned char

/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^3;
sbit LCD1602_RW=P2^4;
sbit LCD1602_RS=P2^5;
/**********************************
��������
**********************************/
/*��51��Ƭ��12MHZʱ���µ���ʱ����*/
/*LCD1602д��8λ�����Ӻ���*/
void LcdWriteCom(uchar CMD,uchar Attribc);
/*LCD1602д��8λ�����Ӻ���*/	
void LcdWriteData(uchar dataW);
/*LCD1602��ʼ���ӳ���*/		
void LcdInit();	
void WaitForEnable(void);
/******��ʾ�ַ���********/
void DisplayChar(uchar Y,uchar X,uchar *s);
/******��ʾ3λ������1λС��********/
void Display(uchar x,uchar y,uint datas);
/******��ʾ2λ��������λС��********/
void Display1(uchar x,uchar y,uint datas);

void LCD_dis_data3(uchar x,uchar y,uint data3);

void Delay1ms(unsigned int y);

#endif
