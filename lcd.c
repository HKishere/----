#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
//#include "pid.h"
#include "pt100.h"	



void Delay1ms(unsigned int y)
{
	unsigned int x;
	for(y;y>0;y--)
		for(x=110;x>0;x--);
}

/*******************************************************************************
* 函数名         : LcdWriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输入           : com
* 输出         	 : 无
*******************************************************************************/
void LcdWriteCom(uchar CMD,uchar Attribc)//写入命令
{
	if(Attribc)WaitForEnable();	
	LCD1602_RS=0;LCD1602_RW=0;
	_nop_();
	LCD1602_DATAPINS=CMD;
	_nop_();	
	LCD1602_E=1;
	_nop_();_nop_();
	LCD1602_E=0;
}

/*******************************************************************************
* 函数名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输入           : dat
* 输出         	 : 无
*******************************************************************************/		   
void LcdWriteData(uchar dataW)//写入数据
{
	WaitForEnable();		
  	LCD1602_RS=1;LCD1602_RW=0;
	_nop_();
	LCD1602_DATAPINS=dataW;
	_nop_();	
	LCD1602_E=1;
	_nop_();_nop_();
	LCD1602_E=0;
}
/*******************************************************************************
* 函数名         : LcdInit()
* 函数功能		 : 初始化LCD屏
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/		   
void LcdInit()						  //LCD初始化子程序
{
	LcdWriteCom(0x32,1);	//将8位总线转为4位总线
	LcdWriteCom(0x38,1);	//在四位线下的初始化
	LcdWriteCom(0x08,1);	//设置数据指针起点
	LcdWriteCom(0x01,1);	//清屏
	LcdWriteCom(0x06,1);	//写一个指针加1
	LcdWriteCom(0x0c,1);  //开显示不显示光标
	Delay1ms(30);
	LcdWriteCom(0x01,1);
	Delay1ms(300);
}
/*******************************************************************************
* 函数名         : WaitForEnable()
* 函数功能		 : 
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void WaitForEnable(void)	
{					
	LCD1602_DATAPINS=0xff;		
	LCD1602_RS=0;LCD1602_RW=1;
	_nop_();
	LCD1602_E=1;
	_nop_();_nop_();
	while(LCD1602_DATAPINS&0x80);	
	LCD1602_E=0;				
}

/********************************************************************************
输入X:0~15		 第几列
	Y:0~1		 第几行
	s:"Light"	
*********************************************************************************/
//void DisplayChar(uchar Y,uchar X,uchar *s)                         //如果想显示姓名可将该函数打开调用即可
//{						
//    Y&=1;						
//    X&=15;						
//    if(Y)	X|=0xC0;					
//    else 	X|=0x80;
//    while(*s!='\0')
//	{
//		LcdWriteCom(X++,0);
//		LcdWriteData(*s);
//		Delays(1);
//		s++;
//	}					
//}



/******显示3位数********/
void LCD_dis_data3(uchar x,uchar y,uint data4)
{
   uchar pos,i=0,str[4];
	switch(x%2)
	{
		case 1:pos=0x80+y;break; //第一行
		case 0:pos=0xc0+y;break; //第二行
	}
	LcdWriteCom(pos,0);		   		 //写入定位命令

     str[0]=data4/100%10+0x30;
     str[1]=data4/10%10+0x30;
     str[2]=data4%10+0x30;
	 str[3]='\0';

	while(str[i] != '\0')
    {
        LcdWriteData(str[i++]);	//写一个字符，指向下一个
    }

}


/******显示3位整数，两位小数********/
void Display1(uchar x,uchar y,uint datas)
{
	uchar pos,i=0,str[8];
	switch(x%2)
	{
		case 1:pos=0x80+y;break; //第一行
		case 0:pos=0xc0+y;break; //第二行
	}
	LcdWriteCom(pos,0);		   		 //写入定位命令
     
	   str[0]=datas/10000%10+0x30;
	   str[1]=datas/1000%10+0x30;
     str[2]=datas/100%10+0x30;
     str[3]='.';
     str[4]=datas/10%10+0x30;
		 str[5]=datas%10+0x30;
	   str[6]='\0';
	
	while(str[i] != '\0')
    {
        LcdWriteData(str[i++]);	//写一个字符，指向下一个
    }
		Delay1ms(5); 
}