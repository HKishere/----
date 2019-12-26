#include "Uart.h"
#include "STC12C5A60S2.h"

#define uint unsigned int
#define uchar unsigned char
uchar RecBuf[10] = {0};  					//���ڽ���Buff
uchar RX_Flag=0;      						//���ڽ�����ɱ�־
uchar receive_cont;         				//����Buff����

//���ڳ�ʼ��
  void Init_Uart()
{
    PCON &= 0x7F;		
    SCON = 0x50;		 //8λ����,�ɱ䲨����
    AUXR &= 0xBF;		 //��ʱ��1ʱ��ΪFosc/12,��12T
    AUXR &= 0xFE;		 //����1ѡ��ʱ��1Ϊ�����ʷ�����
    TMOD &= 0x0F;		 //�����ʱ��1ģʽλ
    TMOD |= 0x20;		 //�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
    TL1 = 0xFD;		   //�趨��ʱ��ֵ
    TH1 = 0xFD;		   //�趨��ʱ����װֵ
    ET1 = 0;		     //��ֹ��ʱ��1�ж�
    TR1 = 1;		     //������ʱ��1
}
	

		
/*******************************************************************************
* ������         : uart_send_chars( unsigned char ch[],unsigned char size)
* ��������		 : �򴮿ڷ��Ͷ���ֽ�
* ����           : ch[] ���������� 		  size Ҫ���͵��ֽ���
* ���         	 : ��
*******************************************************************************/
void uart_send_chars( unsigned char ch[],unsigned char size)
{	 
	int i;
	for(i=0;i<size;i++)
	{
	 SBUF=ch[i];  				//����һ���ֽ�
	 while (TI == 0 ); 		//�ȴ����ͳɹ�
	 TI= 0 ;          		//������ͳɹ���־λ;
	}	
}

/*******************************************************************************
* ������         : send_str(int tem)
* ��������		 : ����λ������һ������
* ����           : ��Ҫ���͵�����
* ���         	 : ��
*******************************************************************************/
void send_str(int tem)
{
    uchar str[6];   //һ֡���ݰ���6���ֽ�
    str[0] = 0x01;  //֡ͷ
    str[1] = 0xff;
    str[2] = tem;   //����֡
    str[3] = tem>>8;
    str[4] = 0xff;  //֡β
    str[5] = 0x01;
	  uart_send_chars(str,6); //��������
}
