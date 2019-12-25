#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
//#include "pid.h"
#include "pt100.h"

int set_temperature=50;                            //温度设定在经济
float gxlb_wendu=0;                                //惯性滤波后的温度
float av_wendu=0;                                  //采样50次后求平均值
float gc_wendu=0;                                  //工程计算后的温度
unsigned int fd_wendu=0;                           //将工程计算后的温度放大100倍
unsigned int lvbo[50]={0};                         //平均滤波数组
unsigned char L=0;                                 //平均滤波数组变量
unsigned int sum=0;                                //数组求和
char count;                                        //数组计数
float  KP0=30;                                     //PID算法中比例系数
float  KI0=0.7;                                    //PID算法中积分系数
float  KD0=80;                                     //PID算法中微分系数
float  Error=0;                                    //PID算法中当前偏差
float  PreError=0;                                 //PID算法中上次偏差
float  PPreError=0;                                //PID算法中上上次偏差
float  Det=0;                                      //PID算法计算结果
int Time_On=50;                                    //PWM波的周期

void My_USART1_Init(void)
{

//GPIO结构体定义
GPIO_InitTypeDef ?GPIO_InitStructure;

//串口结构体定义
USART_InitTypeDef USART_InitStructure;

//中断结构体定义
NVIC_InitTypeDef NVIC_InitStructure;
//使能时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//使能GPIOA时钟

//引脚映射

GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

 //端口初始化

 //GPIOA9端口初始化结构体参数设置

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

 //GPIOA9端口初始化
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 //GPIOA10端口初始化结构体参数设置

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

 //GPIOA10端口初始化
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 //串口初始化结构体参数设置

 USART_InitStructure.USART_BaudRate=115200;
USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
USART_InitStructure.USART_Parity=USART_Parity_No;
USART_InitStructure.USART_StopBits=USART_StopBits_1;
USART_InitStructure.USART_WordLength=USART_WordLength_8b;

//串口初始化
USART_Init(USART1,&USART_InitStructure);

//串口使能
USART_Cmd(USART1 ,ENABLE);
//中断使能
USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接收非空使能

//中断初始化结构体参数设置
NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;

//中断初始化
NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void)
{
	//判断串口1
	if (USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		//从串口1是否发生接收中断
		res = USART_ReceiveData(USART1);
		//通过串口1发送数据
		USART_SendData(USART1, fd_wendu);
	}
}
/*******************************************************************************
* 函数名         : junzhi_lvbo_10
* 函数功能		   : AD采样温度数值平均值滤波，惯性滤波，工程量转换函数
* 输入           : AD采样温度数值
* 输出         	 : 显示工程量值，摄氏度
* 注意		       ：当显示温度与实际温度有误差时，请调节工程量转换参数
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
		  av_wendu = sum/50;                                        //求平均数
		  gxlb_wendu	=av_wendu * 0.7 + gxlb_wendu *0.3;          //惯性滤波
		  gc_wendu=(gxlb_wendu*100/1024-30)/0.72;                   //工程量转换1
		  fd_wendu=gc_wendu * 100 ;                                 //放大100倍
	}
}


/*******************************************************************************
* 函数名         : PID
* 函数功能		 : PID算法，
* 输入           : Time_On原始PWM开度
* 输出         	 : Time_On调整后PWM开度
*******************************************************************************/		
void PID()
{
	PPreError=PreError;                                                  //将上次偏差赋给上上次偏差     
	PreError=Error;                                                      //将当前偏差赋给上次偏差
	Error=set_temperature-gc_wendu;                                      //计算偏差
	Det=KP0*(Error-PreError)+KI0*Error+KD0*(Error-2*PreError+PPreError); //增量式PID计算公式：△u(k)=Kp*(e(k)-e(k-1))+Ki*e(k)+Kd*(e(k)-2*e(k-1)+e(k-2))
	Time_On=Time_On+Det;                                                 //u(k)=u(k)+△u(k)
	if(Time_On>100)	Time_On=100;                                         //防计算结果超出设定周期               
	if(Time_On<0)	Time_On=0;                                           //防计算结果低于最小占空比
}


/*******************************************************************************
* 函数名         : main
* 函数功能		 : 主程序计算
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void main()
{
		LcdInit();                                                          //液晶屏初始化                          
		DisplayChar(0,0,"set:");
		DisplayChar(1,0,"now:");
		DisplayChar(0,8,"kishere");
		LCD_dis_data3(1,4,set_temperature);                                 //数字代表第一行第四列，显示温度给定值
		Delay1ms(10);                                                       //延时5毫秒
		init_timer0();	                                                    //单片机定时器初始化
		InitADC();                                                          //AD采样初始化
		EA=1;                                                               //总中断开关
		EADC=1;	                                                            //ADC中断使能位
		while(1)
		{ 
				junzhi_lvbo_10(ADC);                                            //AD转换数据滤波
				Display1(2,4,fd_wendu);                                         //显示当前温度
				PID();															//进入PID计算
				//中断分组使能
				NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
				//串口初始化
				My_USART1_Init();
				USART1_IRQHandler();
		}	
}

