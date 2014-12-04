#ifndef __wuxian
#define __wuxian


#define  TXEN_0     P4OUT &=~BIT0          //输出0, P4.0,1发送模式
#define  TXEN_1     P4OUT |= BIT0          //输出1，P4.0,0接收模式
//==============================================================================
#define  TRX_CE_0   P4OUT &=~BIT1    //输出0, P4.1, 0 发射或接收禁止
#define  TRX_CE_1   P4OUT |= BIT1    //输出1, P4.1, 1 发射或接收使能
//==============================================================================
#define  PWR_0      P4OUT &=~BIT2    //输出0,  P4.2,  0 未上电
#define  PWR_1      P4OUT |= BIT2    //输出1,  P4.2,  1上电
//===================================主入从出===================================
#define  MISO_0     P5OUT &=~BIT2     // 输出0,  P5.2,  0   RF905给单片机 
#define  MISO_1     P5OUT |= BIT2      // 输出1,  P5.2, 1   RF905给单片机  
//==================================主出从入====================================
#define  MOSI_0     P5OUT &=~BIT1         // 输出0,  P5.1,  0   单片机给RF905
#define  MOSI_1     P5OUT |= BIT1         // 输出1,  P5.1,  1   单片机给RF905
//===================================SPI时钟端口================================
#define  SCK_0      P5OUT &=~BIT3         //输出0,  P5.3,  0 单片机给RF905时钟低电平
#define  SCK_1      P5OUT |= BIT3         //输出1,  P5.3,  1 单片机给RF905时钟高电平
//===================================SPI使能端口=================================
#define  CSN_0      P5OUT &=~BIT0   //SPI 低电平  P5.0 使能SPI
#define  CSN_1      P5OUT |= BIT0   //SPI 高电平 P5.0 禁止SPI
//==================================以下为状态端口==============================
//===========================AM  地址匹配=======================================
#define  AM_0       P4OUT &=~BIT5     //  地址匹配 P4.5 
#define  AM_1       P4OUT |= BIT5     //  地址匹配 P4.5 
//=================================DR 数据接收状态==============================
#define  DR_0       P4OUT &=~BIT6
#define  DR_1       P4OUT |= BIT6
//=====================================CD 载波侦听状态==========================
#define  CD_0       P4OUT &=~BIT4
#define  CD_1       P4OUT |= BIT4
//===========================NRF905:SPI指令=====================================
#define WC		0x00
#define RC		0x10
#define WTP		0x20
#define RTP		0x21
#define WTA		0x22
#define RTA		0x23
#define RRP		0x24

void nRF905_IO_set(void);
void SetRxMode(void);//设置接收模式
void Spiwrite_byte(unsigned char byte);//用spi向无线模块寄存器中写数据
unsigned char Spiread_byte();//用spi主机从无线模块寄存器中读数据
unsigned char CheckDR(void);//检查是否有新数据传入 Data Ready
void RxPacket();
void Config905(); //初始化RF寄存器

#endif
