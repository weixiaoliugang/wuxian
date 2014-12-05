#include <msp430x16x.h>
#include "system.h"
#include "wuxian.h"


extern uchar data_buff;
extern uchar acception[8];//用来存放接收到的数据
extern uchar RFConf[10];

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

void SetRxMode(void)//设置接收模式
{
        TXEN_0;
	TRX_CE_1;
	delay(1); 					// Delay for mode change(>=650us)
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

void RxPacket()
{
    uchar i;
    SetRxMode();//设置接受模式
    delay(1);
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
    delay(5);//等待DR和AM都置低     
}


