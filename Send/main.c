
#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"
#include "nrf905.h"
#include "USART.h"

uchar RBuff[8];

void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//时钟初始化
  USART_Init();//串口初始化
  _EINT();  //全局使能中断
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化  
  while(1)
  {
    TxPacket();
  }
}

 
  
