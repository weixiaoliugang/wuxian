#include <msp430x16x.h>
#include "system.h"


void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5为UART的收发端口
  U0CTL|=CHAR;     //8_bit的数据
  U0TCTL|=SSEL1;   //时钟源选择：smClk
  UBR00=0x41;   //波特率是指为9600
  UBR10=0x03;                             
  UMCTL0=0x00;
  UCTL0&= ~SWRST;  // 初始化USART0的状态
  ME1 |= URXE0+UTXE0;   //打开收发开关
  IE1 |= URXIE0;       //打开接受使能中断
}