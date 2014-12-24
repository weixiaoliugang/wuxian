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
uint count1=0;
void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//时钟初始化
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化
  SetRxMode();//设置接受模式
  //delay(1);
  _EINT();  //全局使能中断
  while(1)
  {
    
  }
}


#pragma vector=PORT1_VECTOR     //接收中断
__interrupt void KEY_PORT1(void)
{
  RxPacket(); //接受数据      
  delay(1);
  TxPacket(RBuff,1); //发送应答信
  count++;
  SetRxMode();//设置接受模式 
  P1IFG=0x00;

  
}

