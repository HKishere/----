#ifndef __pt100_H_
#define __pt100_H_

#include <STC12C5A60S2.h>
#define   FOSC 18432000L
#define   BAUD 9600
#define   ADC_POWER   0x80     //ADC power control bit
#define   ADC_FLAG    0x10     //ADC complete flag
#define   ADC_START   0x08     //ADC start control bit
#define   ADC_SPEEDLL   0x00     //540 clocks
#define   ADC_SPEEDL   0x20     //360 clocks
#define   ADC_SPEEDH   0x40     //180 clocks
#define   ADC_SPEEDHH   0x60     //90 clocks
extern unsigned int ADC;
void Delay(unsigned int n);
void InitADC();
void SendData(unsigned int dat);

#endif 