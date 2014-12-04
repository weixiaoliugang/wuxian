
#include <msp430x16x.h>
#include "wixian.h"
#define uchar unsigned char
#define uint  unsigned int

uchar data_buff;
uchar RBuff[8];
uchar address[4]={0xCC,0xCC,0xCC,0xCC};//接受的地址
uchar Flag_Receive = 0;
uchar temp;
unsigned char  RFConf[10]=
{                             
  0x4c,                             //CH_NO,配置频段在430MHZ
  0x0C,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x20,0x20,                        //接收发送有效数据长度为32字节
  0xCC,0xCC,0xCC,0xCC,              //接收地址
  0x58,                              //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};

void Init_Clk(void)
{
   unsigned int i;   
   BCSCTL1 &= ~XT2OFF; //使TX2有效,TX2上电时默认为关闭的.
  do 
  { 
   IFG1 &= ~OFIFG; //清振荡器失效标志 
   for(i= 0xff; i>0; i--); //延时,待稳定. 
   } 
  while ((IFG1 & OFIFG)!=0);  //若振荡器失效标志有效,一直等待 
   BCSCTL2 =SELM1+SELS;    //使MCLK选择XT2且MCLK=XT2=8MHZ ，使SMCLK选择XT2且SMCLK=XT2=8MHZ
   			
}

void delay(uint n)//延时1ms
{
    uint j;
    uchar i;
    for(j = n;j > 0;j--)
	for(i = 150;i > 0;i--)  
          _NOP();
}

void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5为UART的收发端口
  U0CTL|=CHAR;     //8_bit的数据
  U0TCTL|=SSEL0;   //时钟源选择：AClk
  UBR00=0x03;   //波特率是指为9600
  UBR10=0x00;                             
  UMCTL0=0x4A;
  UCTL0&= ~SWRST;  // 初始化USART0的状态
  ME1 |= URXE0;   //打开收发开关
  IE1 |= URXIE0;       //打开接受使能中断
  _EINT();  //全局使能中断
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
void TxPacket(void)
{
  uchar i,j;
  CSN_0;//使能无线模块的spi
  Spiwrite_byte(WTP);//写数据发送寄存器的预编译指令
  delay(1);
  for(i=0;i<8;i++)
  {
    Spiwrite_byte(RBuff[i]);//写入要发送的数据
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

void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  USART_Init();//串口初始化
  Init_Clk();//时钟初始化
  nRF905_IO_set();
  Config905();//RF配置寄存器的初始化  
  while(1)
  {
    TxPacket();
  }
}

 
  
#pragma vector=UART0RX_VECTOR //串口接收中断
__interrupt void USART0_RXIRQ (void) 
{ 
//  暂存接受数据
temp = RXBUF0; 
// 8 字节接受缓冲队列
RBuff[0] = RBuff[1]; 
RBuff[1] = RBuff[2]; 
RBuff[2] = RBuff[3]; 
RBuff[3] = RBuff[4]; 
RBuff[4] = RBuff[5]; 
RBuff[5] = RBuff[6]; 
RBuff[6] = RBuff[7]; 
RBuff[7] = temp;
//  判断数据有效(有效则设置标志位，等待处理) 
if (RBuff[0] == 0xd0){Flag_Receive = 1;return; }else{Flag_Receive = 0;} 
}