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
* ������         : LcdWriteCom
* ��������		   : ��LCDд��һ���ֽڵ�����
* ����           : com
* ���         	 : ��
*******************************************************************************/
void LcdWriteCom(uchar CMD,uchar Attribc)//д������
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
* ������         : LcdWriteData
* ��������		   : ��LCDд��һ���ֽڵ�����
* ����           : dat
* ���         	 : ��
*******************************************************************************/		   
void LcdWriteData(uchar dataW)//д������
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
* ������         : LcdInit()
* ��������		 : ��ʼ��LCD��
* ����           : ��
* ���         	 : ��
*******************************************************************************/		   
void LcdInit()						  //LCD��ʼ���ӳ���
{
	LcdWriteCom(0x32,1);	//��8λ����תΪ4λ����
	LcdWriteCom(0x38,1);	//����λ���µĳ�ʼ��
	LcdWriteCom(0x08,1);	//��������ָ�����
	LcdWriteCom(0x01,1);	//����
	LcdWriteCom(0x06,1);	//дһ��ָ���1
	LcdWriteCom(0x0c,1);  //����ʾ����ʾ���
	Delay1ms(30);
	LcdWriteCom(0x01,1);
	Delay1ms(300);
}
/*******************************************************************************
* ������         : WaitForEnable()
* ��������		 : 
* ����           : ��
* ���         	 : ��
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
����X:0~15		 �ڼ���
	Y:0~1		 �ڼ���
	s:"Light"	
*********************************************************************************/
//void DisplayChar(uchar Y,uchar X,uchar *s)                         //�������ʾ�����ɽ��ú����򿪵��ü���
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



/******��ʾ3λ��********/
void LCD_dis_data3(uchar x,uchar y,uint data4)
{
   uchar pos,i=0,str[4];
	switch(x%2)
	{
		case 1:pos=0x80+y;break; //��һ��
		case 0:pos=0xc0+y;break; //�ڶ���
	}
	LcdWriteCom(pos,0);		   		 //д�붨λ����

     str[0]=data4/100%10+0x30;
     str[1]=data4/10%10+0x30;
     str[2]=data4%10+0x30;
	 str[3]='\0';

	while(str[i] != '\0')
    {
        LcdWriteData(str[i++]);	//дһ���ַ���ָ����һ��
    }

}


/******��ʾ3λ��������λС��********/
void Display1(uchar x,uchar y,uint datas)
{
	uchar pos,i=0,str[8];
	switch(x%2)
	{
		case 1:pos=0x80+y;break; //��һ��
		case 0:pos=0xc0+y;break; //�ڶ���
	}
	LcdWriteCom(pos,0);		   		 //д�붨λ����
     
	   str[0]=datas/10000%10+0x30;
	   str[1]=datas/1000%10+0x30;
     str[2]=datas/100%10+0x30;
     str[3]='.';
     str[4]=datas/10%10+0x30;
		 str[5]=datas%10+0x30;
	   str[6]='\0';
	
	while(str[i] != '\0')
    {
        LcdWriteData(str[i++]);	//дһ���ַ���ָ����һ��
    }
		Delay1ms(5); 
}