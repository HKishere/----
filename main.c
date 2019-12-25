#include "STC12C5A60S2.h"
#include <INTRINS.H>
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "stdio.h"	
//#include "pid.h"
#include "pt100.h"

int set_temperature=50;                            //�¶��趨�ھ���
float gxlb_wendu=0;                                //�����˲�����¶�
float av_wendu=0;                                  //����50�κ���ƽ��ֵ
float gc_wendu=0;                                  //���̼������¶�
unsigned int fd_wendu=0;                           //�����̼������¶ȷŴ�100��
unsigned int lvbo[50]={0};                         //ƽ���˲�����
unsigned char L=0;                                 //ƽ���˲��������
unsigned int sum=0;                                //�������
char count;                                        //�������
float  KP0=30;                                     //PID�㷨�б���ϵ��
float  KI0=0.7;                                    //PID�㷨�л���ϵ��
float  KD0=80;                                     //PID�㷨��΢��ϵ��
float  Error=0;                                    //PID�㷨�е�ǰƫ��
float  PreError=0;                                 //PID�㷨���ϴ�ƫ��
float  PPreError=0;                                //PID�㷨�����ϴ�ƫ��
float  Det=0;                                      //PID�㷨������
int Time_On=50;                                    //PWM��������

void My_USART1_Init(void)
{

//GPIO�ṹ�嶨��
GPIO_InitTypeDef ?GPIO_InitStructure;

//���ڽṹ�嶨��
USART_InitTypeDef USART_InitStructure;

//�жϽṹ�嶨��
NVIC_InitTypeDef NVIC_InitStructure;
//ʹ��ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ��GPIOAʱ��

//����ӳ��

GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

 //�˿ڳ�ʼ��

 //GPIOA9�˿ڳ�ʼ���ṹ���������

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

 //GPIOA9�˿ڳ�ʼ��
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 //GPIOA10�˿ڳ�ʼ���ṹ���������

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

 //GPIOA10�˿ڳ�ʼ��
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 //���ڳ�ʼ���ṹ���������

 USART_InitStructure.USART_BaudRate=115200;
USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
USART_InitStructure.USART_Parity=USART_Parity_No;
USART_InitStructure.USART_StopBits=USART_StopBits_1;
USART_InitStructure.USART_WordLength=USART_WordLength_8b;

//���ڳ�ʼ��
USART_Init(USART1,&USART_InitStructure);

//����ʹ��
USART_Cmd(USART1 ,ENABLE);
//�ж�ʹ��
USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���շǿ�ʹ��

//�жϳ�ʼ���ṹ���������
NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;

//�жϳ�ʼ��
NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void)
{
	//�жϴ���1
	if (USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		//�Ӵ���1�Ƿ��������ж�
		res = USART_ReceiveData(USART1);
		//ͨ������1��������
		USART_SendData(USART1, fd_wendu);
	}
}
/*******************************************************************************
* ������         : junzhi_lvbo_10
* ��������		   : AD�����¶���ֵƽ��ֵ�˲��������˲���������ת������
* ����           : AD�����¶���ֵ
* ���         	 : ��ʾ������ֵ�����϶�
* ע��		       ������ʾ�¶���ʵ���¶������ʱ������ڹ�����ת������
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
		  av_wendu = sum/50;                                        //��ƽ����
		  gxlb_wendu	=av_wendu * 0.7 + gxlb_wendu *0.3;          //�����˲�
		  gc_wendu=(gxlb_wendu*100/1024-30)/0.72;                   //������ת��1
		  fd_wendu=gc_wendu * 100 ;                                 //�Ŵ�100��
	}
}


/*******************************************************************************
* ������         : PID
* ��������		 : PID�㷨��
* ����           : Time_OnԭʼPWM����
* ���         	 : Time_On������PWM����
*******************************************************************************/		
void PID()
{
	PPreError=PreError;                                                  //���ϴ�ƫ������ϴ�ƫ��     
	PreError=Error;                                                      //����ǰƫ����ϴ�ƫ��
	Error=set_temperature-gc_wendu;                                      //����ƫ��
	Det=KP0*(Error-PreError)+KI0*Error+KD0*(Error-2*PreError+PPreError); //����ʽPID���㹫ʽ����u(k)=Kp*(e(k)-e(k-1))+Ki*e(k)+Kd*(e(k)-2*e(k-1)+e(k-2))
	Time_On=Time_On+Det;                                                 //u(k)=u(k)+��u(k)
	if(Time_On>100)	Time_On=100;                                         //�������������趨����               
	if(Time_On<0)	Time_On=0;                                           //��������������Сռ�ձ�
}


/*******************************************************************************
* ������         : main
* ��������		 : ���������
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void main()
{
		LcdInit();                                                          //Һ������ʼ��                          
		DisplayChar(0,0,"set:");
		DisplayChar(1,0,"now:");
		DisplayChar(0,8,"kishere");
		LCD_dis_data3(1,4,set_temperature);                                 //���ִ����һ�е����У���ʾ�¶ȸ���ֵ
		Delay1ms(10);                                                       //��ʱ5����
		init_timer0();	                                                    //��Ƭ����ʱ����ʼ��
		InitADC();                                                          //AD������ʼ��
		EA=1;                                                               //���жϿ���
		EADC=1;	                                                            //ADC�ж�ʹ��λ
		while(1)
		{ 
				junzhi_lvbo_10(ADC);                                            //ADת�������˲�
				Display1(2,4,fd_wendu);                                         //��ʾ��ǰ�¶�
				PID();															//����PID����
				//�жϷ���ʹ��
				NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
				//���ڳ�ʼ��
				My_USART1_Init();
				USART1_IRQHandler();
		}	
}

