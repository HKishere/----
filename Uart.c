#include "Uart.h"
#include "STC12C5A60S2.h"

#define uint unsigned int
#define uchar unsigned char
uchar RecBuf[10] = {0};  					//串口接收Buff
uchar RX_Flag=0;      						//串口接收完成标志
uchar receive_cont;         				//接收Buff计数

//串口初始化
  void Init_Uart()
{
    PCON &= 0x7F;		
    SCON = 0x50;		 //8位数据,可变波特率
    AUXR &= 0xBF;		 //定时器1时钟为Fosc/12,即12T
    AUXR &= 0xFE;		 //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F;		 //清除定时器1模式位
    TMOD |= 0x20;		 //设定定时器1为8位自动重装方式
    TL1 = 0xFD;		   //设定定时初值
    TH1 = 0xFD;		   //设定定时器重装值
    ET1 = 0;		     //禁止定时器1中断
    TR1 = 1;		     //启动定时器1
}
	

		
/*******************************************************************************
* 函数名         : uart_send_chars( unsigned char ch[],unsigned char size)
* 函数功能		 : 向串口发送多个字节
* 输入           : ch[] 待发送数据 		  size 要发送的字节数
* 输出         	 : 无
*******************************************************************************/
void uart_send_chars( unsigned char ch[],unsigned char size)
{	 
	int i;
	for(i=0;i<size;i++)
	{
	 SBUF=ch[i];  				//发送一个字节
	 while (TI == 0 ); 		//等待发送成功
	 TI= 0 ;          		//清除发送成功标志位;
	}	
}

/*******************************************************************************
* 函数名         : send_str(int tem)
* 函数功能		 : 向上位机发送一个数据
* 输入           : 需要发送的数据
* 输出         	 : 无
*******************************************************************************/
void send_str(int tem)
{
    uchar str[6];   //一帧数据包含6个字节
    str[0] = 0x01;  //帧头
    str[1] = 0xff;
    str[2] = tem;   //数据帧
    str[3] = tem>>8;
    str[4] = 0xff;  //帧尾
    str[5] = 0x01;
	  uart_send_chars(str,6); //发送数据
}
