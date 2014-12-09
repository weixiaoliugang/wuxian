/*******************************************************
     看一下接收与发送地址的具体含义,应答信号为1111111
*******************************************************/
#include <msp430x16x.h>
#include "wuxian.h"
#include "nrf905.h"
#include "system.h"
#include "USART.h"

uchar RBuff[8]={1,1,1,1,1,1,1,1};
uchar acception[8];//用来存放接收到的数据

void main()
{
  uchar j;
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//时钟初始化
  USART_Init();
  _EINT();  //全局使能中断
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化
  
  while(1)
  {
      RxPacket(); //接受数据
     /* for(j=0;j<8;j++)//送到串口显示
      {
        U0TXBUF=acception[j];
        while((IFG1&UTXIFG0)==0); //等待发送完成
      }*/
      
      P4DIR=0x47;
      P4OUT&=~BIT6;
      P4DIR=0x07;
      delay(10);
      TxPacket(RBuff); //发送应答信号
      
  }
}
