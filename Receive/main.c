#include <msp430x16x.h>
#include "wuxian.h"
#include "system.h"
#include "USART.h"

uchar data_buff;
uchar acception[8];//用来存放接收到的数据

uchar RFConf[10]=
{                             
  0x4c,                             //CH_NO,配置频段在430MHZ
  0x0C,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x20,0x20,                        //接收发送有效数据长度为32字节
  0xCC,0xCC,0xCC,0xCC,              //接收地址
  0x58,                              //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};



void main()
{
  uchar j;
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//时钟初始化
  USART_Init();
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化
  _EINT();  //全局使能中断
  while(1)
  {
      RxPacket();                  //接受数据
      for(j=0;j<8;j++)
      {
        U0TXBUF=acception[j];
        while((IFG1&UTXIFG0)==0); //等待发送完成
      }
  }
}
