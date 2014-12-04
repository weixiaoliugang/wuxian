#include <msp430x16x.h>
#include "system.h"

extern uchar RBuff[8];
uchar Flag_Receive = 0;

void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5为UART的收发端口
  U0CTL|=CHAR;     //8_bit的数据
  U0TCTL|=SSEL0;   //时钟源选择：AClk
  UBR00=0x03;   //波特率是指为9600
  UBR10=0x00;                             
  UMCTL0=0x4A;
  UCTL0&= ~SWRST;  // 初始化USART0的状态
  ME1 |= URXE0;   //打开收发开关
  IE1 |= URXIE0;       //打开接受使能中断
}

#pragma vector=UART0RX_VECTOR //串口接收中断
__interrupt void USART0_RXIRQ (void) 
{ 
//  暂存接受数据
  uchar temp = RXBUF0;
  // 8 字节接受缓冲队列
  RBuff[0] = RBuff[1]; 
  RBuff[1] = RBuff[2]; 
  RBuff[2] = RBuff[3]; 
  RBuff[3] = RBuff[4]; 
  RBuff[4] = RBuff[5]; 
  RBuff[5] = RBuff[6]; 
  RBuff[6] = RBuff[7]; 
  RBuff[7] = temp;
  //  判断数据有效(有效则设置标志位，等待处理) 
  if (RBuff[0] == 0xd0)
  {
    Flag_Receive = 1;
    return; 
  }
  else
    Flag_Receive = 0; 
}