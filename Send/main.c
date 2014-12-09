/***********************************************
            设置应答信号为11111111
***********************************************/
#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"
#include "nrf905.h"
#include "USART.h"

uchar RBuff[8];
uchar acception[8]={3,3,3,3,3,3,3,3};
uchar Receive_Flag=0;
uchar aa[8]={3,3,3,3,3,3,3,3};
uchar flag=0;

void main()
{
  uchar j;
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//时钟初始化
  USART_Init();//串口初始化
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化  
  _EINT();  //全局使能中断
  while(1)
  {
    if(flag==0)//此句程序测试所用
    {
      if(Receive_Flag==1)
      {      
        TxPacket(RBuff);
        Receive_Flag=0;//接收标志位清0
        flag=1;       //此句程序测试所用
        delay(10);
        P4DIR=0x47;
        P4OUT&=~BIT6;
        P4DIR=0x07;
        response();
      } 
    }
    else
    {
     
      for(j=100;j>0;j--)
      {
        TxPacket(aa);
      }
      response();//应答函数
      Receive_Flag=0;//接收标志位清0
    }
  }
}

