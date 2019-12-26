#ifndef __Uart__H
#define __Uart__H
#include "STC12C5A60S2.h"
#include "main.h"

void Init_Uart();
void uart_send_chars( unsigned char ch[],unsigned char size);
unsigned int CRC_Check(unsigned char *CRC_data,unsigned char CRC_length);
void SendData(unsigned int dat);
void send_str(int tem);

#endif
