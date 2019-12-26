#ifndef __lcd_H_
#define __lcd_H_

#include <STC12C5A60S2.h>
#define uint unsigned int 
#define uchar unsigned char

/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^3;
sbit LCD1602_RW=P2^4;
sbit LCD1602_RS=P2^5;
/**********************************
函数声明
**********************************/
/*在51单片机12MHZ时钟下的延时函数*/
/*LCD1602写入8位命令子函数*/
void LcdWriteCom(uchar CMD,uchar Attribc);
/*LCD1602写入8位数据子函数*/	
void LcdWriteData(uchar dataW);
/*LCD1602初始化子程序*/		
void LcdInit();	
void WaitForEnable(void);
/******显示字符串********/
void DisplayChar(uchar Y,uchar X,uchar *s);
/******显示3位整数，1位小数********/
void Display(uchar x,uchar y,uint datas);
/******显示2位整数，两位小数********/
void Display1(uchar x,uchar y,uint datas);

void LCD_dis_data3(uchar x,uchar y,uint data3);

void Delay1ms(unsigned int y);

#endif
