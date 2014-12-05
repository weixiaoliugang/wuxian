
#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"
#include "nrf905.h"
#include "USART.h"

uchar RBuff[8];
uchar Receive_Flag=0;
void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//时钟初始化
  USART_Init();//串口初始化
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化  
  _EINT();  //全局使能中断
  while(1)
  {
    if(Receive_Flag==1)
    {
      TxPacket(RBuff);
      Receive_Flag=0;//接收标志位清0
    }
    
    
  }
}

 
  
