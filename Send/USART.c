#include <msp430x16x.h>
#include "system.h"

extern uchar RBuff[8];
extern uchar Receive_Flag; 
void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5为UART的收发端口
  U0CTL|=CHAR;     //8_bit的数据
  U0TCTL|=SSEL1;   //时钟源选择：smclk
  UBR00=0x41;   //波特率是指为9600
  UBR10=0x03;                             
  UMCTL0=0x00;
  UCTL0&= ~SWRST;  // 初始化USART0的状态
  ME1 |= URXE0;   //打开收发开关
  IE1 |= URXIE0;       //打开接受使能中断
}

#pragma vector=UART0RX_VECTOR //串口接收中断
__interrupt void USART0_RXIRQ (void) 
{ 
  static uchar count =0;
  uchar temp;
  temp = RXBUF0;
  if(count<7)
  {
    RBuff[count] = temp;
    count++;
  }
  
  else
  {
    RBuff[count] = temp;
    count=0;
    Receive_Flag=1;//置位接收标志位
  }
  
}