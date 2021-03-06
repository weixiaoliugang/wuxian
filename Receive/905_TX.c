#include <msp430x16x.h>
#include "system.h"
#include "wuxian.h"

extern uchar acception[8];//用来存放接收到的数据
extern uchar RFConf[10];
uchar  address[4]={0xAA,0xAA,0xAA,0xAA};//接受的地址
uchar  RFConf[10]=
{                             
  0x4c,                             //CH_NO,配置频段在430MHZ
  0x0C,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x08,0x01,                        //接收有效数据长度为8字节，发送数据的有效长度为1。。。。
  0xBB,0xBB,0xBB,0xBB,              //本机的地址
  0x58,                              //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};

void nRF905_IO_set(void)       //端口设置
{  
        P1DIR=0xfe;//高四位输出，低四位输入
        P1IES=0x00;//上升沿触发
        P1IE=0x01;//开启低四位的外部中断使能
        P4DIR |= 0x07;
        P4DIR &= 0x8F;    
        P5DIR = 0x0B;
        P4SEL&=0x88;
        P5SEL&=0xF4;         //p4.0,p4.1,p4.2输出，数字端口，p4.4,p4.5,p4.6输入，数字端口  
        CSN_1;				            // Spi 	disable
	SCK_0;				          // Spi clock line init low
	PWR_1;					 // nRF905 power on
	TRX_CE_0;				// Set nRF905 in standby mode
	TXEN_0;					// set radio in Rx mode
}

void Spiwrite_byte(uchar byte)//用spi向无线模块寄存器中写数据
{
  uchar i;
  uchar data_buff;
  data_buff=byte;
  for(i=0;i<8;i++)
  {
    if((data_buff&0x80)!=0)
      MOSI_1;
    else      
      MOSI_0;
    SCK_1;                 //下降沿写
    data_buff=data_buff<<1;//为下一次的写作准备
    //delay(1);
    SCK_0;   
  }
}

unsigned char Spiread_byte()//用spi主机从无线模块寄存器中读数据
{
  uchar data_buff=0x00;//用来存放接受到的数据
  uchar i;
  for(i=0;i<8;i++)
  {
    data_buff=data_buff<<1;//为下一次做准备
    SCK_1;//下降沿读
    if((P5IN&0x04)!=0)
      data_buff |=0x01;
    else
      data_buff &=~(0x01);
    SCK_0; 
  }
  return data_buff;
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

void SetRxMode(void)//设置接收模式
{
        PWR_1;
        TXEN_0;
	TRX_CE_1;
	delay(1); 					// Delay for mode change(>=650us)
}

void SetTxMode(void)//设置发送模式
{
        PWR_1;
        TXEN_1;// Delay for mode change(>=650us)
	TRX_CE_0;
	delay(1); 					
}

unsigned char CheckDR(void)		//检查是否有新数据传入 Data Ready
{
	if ((P4IN&0x40))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void RxPacket()      //用无线模块接收数据
{
    uchar i;
    while(CheckDR()==0);//判断是否有数据传入
    TRX_CE_0;
    CSN_0;//使能无线模块的spi
    delay(1);
    Spiwrite_byte(RRP);//读数据接受寄存器的预编译指令
    delay(1);
    for(i=0;i<8;i++)
    {
      acception[i]=Spiread_byte();
    }
    CSN_1;
    delay(10);//等待DR和AM都置低     
}


void TxPacket(uchar *sended_data,uchar length)     //用无线模块发送数据
{
  uchar i,j;
  SetTxMode();//设置发送模式
  //delay(5);
  CSN_0;//使能无线模块的spi
  Spiwrite_byte(WTP);//写数据发送寄存器的预编译指令
  delay(1);
  for(i=0;i<length;i++)
  {
    Spiwrite_byte(sended_data[i]);//写入要发送的数据
  }
  CSN_1;//关闭无线模块的spi
  delay(1);
  CSN_0;//使能无线模块的spi
  Spiwrite_byte(WTA);//写数据发送寄存器的预编译指令
  delay(1);
  for(j=0;j<4;j++)
  {
    Spiwrite_byte(address[j]);  
  }
  CSN_1;//关闭无线模块的spi 
  TRX_CE_1;					// 打开发送使能
  delay(10);					// 很重要，一定要保证足够的时间
  TRX_CE_0;	                                // 关闭发送使能
}




