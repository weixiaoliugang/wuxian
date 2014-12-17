/*******************************************************
     看一下接收与发送地址的具体含义,应答信号为1111111
*******************************************************/
#include <msp430x16x.h>
#include "wuxian.h"
#include "nrf905.h"
#include "system.h"
#include "USART.h"

uchar RBuff[1]={1};
uchar acception[8];//用来存放接收到的数据
uint count=0;//计数值
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
      delay(10);
      for(j=2;j>0;j--)
      {
        TxPacket(RBuff,1); //发送应答信号
        P4DIR=0x47;
        P4OUT&=~BIT6;
        P4DIR=0x07;
      }
      count++;
  }
}
