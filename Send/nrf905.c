#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"

extern uchar RBuff[8];
 uchar data_buff;
 uchar  address[4]={0xCC,0xCC,0xCC,0xCC};//接受的地址
 uchar  RFConf[10]=
{                             
  0x4c,                             //CH_NO,配置频段在430MHZ
  0x0C,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x20,0x20,                        //接收发送有效数据长度为32字节
  0xCC,0xCC,0xCC,0xCC,              //接收地址
  0x58,                              //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};

void nRF905_IO_set(void)
{
        P4DIR |= 0x07; P4DIR &= 0x8F;   P4SEL&=0x88;  //p4.0,p4.1,p4.2输出，数字端口，p4.4,p4.5,p4.6输入，数字端口 
        P5DIR = 0x0B;  P5SEL&=0xF4;  
        CSN_1;				            // Spi 	disable
	SCK_0;				          // Spi clock line init low
	PWR_1;					 // nRF905 power on
	TRX_CE_0;				// Set nRF905 in standby mode
	TXEN_0;					// set radio in Rx mode
}

void SetTxMode(void)//设置发送模式
{
        TXEN_1;// Delay for mode change(>=650us)
	TRX_CE_0;
	delay(1); 					
}


void Spiwrite_byte(uchar byte)//用spi向无线模块寄存器中写数据
{
  uchar i;
  data_buff=byte;
  for(i=0;i<8;i++)
  {
    if((data_buff&0x80)!=0)
      MOSI_1;
    else      
      MOSI_0;
    SCK_1;                 //下降沿写
    data_buff=data_buff<<1;//为下一次的写作准备
    delay(1);
    SCK_0;   
  }
}


//用无线模块发送数据
void TxPacket(uchar *sended_data)
{
  uchar i,j;
  CSN_0;//使能无线模块的spi
  Spiwrite_byte(WTP);//写数据发送寄存器的预编译指令
  delay(1);
  for(i=0;i<8;i++)
  {
    Spiwrite_byte(sended_data[i]);//写入要发送的数据
  }
  CSN_1;//关闭无线模块的spi
  delay(5);
  
  CSN_0;//使能无线模块的spi
  Spiwrite_byte(WTA);//写数据发送寄存器的预编译指令
  delay(1);
  for(j=0;j<4;j++)
  {
    Spiwrite_byte(address[j]);  
  }
  CSN_1;//关闭无线模块的spi
  SetTxMode();//设置发送模式
  TRX_CE_1;					// 打开发送使能
  delay(1);					// while (DR!=1);改改
  TRX_CE_0;	                                // 关闭发送使能
}

void Config905() //初始化RF寄存器
{
  uchar i;
  CSN_0;//使能无线模块的spi
  Spiwrite_byte(WC);//写RF寄存器的预编译指令
  for(i=0;i<10;i++)
  {
    Spiwrite_byte(RFConf[i]);//写RF中的各项配置信息
  }
  CSN_1;//禁止spi使能  
}