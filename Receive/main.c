#include <msp430x14x.h>
#include "wuxian.h"
#include "Config.h"
#define uchar unsigned char
#define uint  unsigned int
uchar data_buff;
uchar acception[8];//������Ž��յ�������
uchar j;

// test

unsigned char  RFConf[10]=
{                             
  0x4c,                             //CH_NO,����Ƶ����430MHZ
  0x0C,                             //�������Ϊ10db,���ط����ڵ�Ϊ����ģʽ
  0x44,                             //��ַ������ã�Ϊ4�ֽ�
  0x20,0x20,                        //���շ�����Ч���ݳ���Ϊ32�ֽ�
  0xCC,0xCC,0xCC,0xCC,              //���յ�ַ
  0x58,                              //CRC����8λCRCУ�飬�ⲿʱ���źŲ�ʹ�ܣ�16M����
};

void Init_Clk(void)
{
   unsigned int i;   
   BCSCTL1 &= ~XT2OFF; //ʹTX2��Ч,TX2�ϵ�ʱĬ��Ϊ�رյ�.
  do 
  { 
   IFG1 &= ~OFIFG; //������ʧЧ��־ 
   for(i= 0xff; i>0; i--); //��ʱ,���ȶ�. 
   } 
  while ((IFG1 & OFIFG)!=0);  //������ʧЧ��־��Ч,һֱ�ȴ� 
   BCSCTL2 =SELM1+SELS;    //ʹMCLKѡ��XT2��MCLK=XT2=8MHZ ��ʹSMCLKѡ��XT2��SMCLK=XT2=8MHZ
   			
}

void delay(uint n)//��ʱ1ms
{
    uint j;
    uchar i;
    for(j = n;j > 0;j--)
	for(i = 150;i > 0;i--)  
          _NOP();
}

void nRF905_IO_set(void)
{
        P4DIR |= 0x07; P4DIR &= 0x8F;   P4SEL&=0x88;  //p4.0,p4.1,p4.2��������ֶ˿ڣ�p4.4,p4.5,p4.6���룬���ֶ˿�
        P5DIR = 0x0B;  P5SEL&=0xF4;  
        CSN_1;				            // Spi 	disable
	SCK_0;				          // Spi clock line init low
	PWR_1;					 // nRF905 power on
	TRX_CE_0;				// Set nRF905 in standby mode
	TXEN_0;					// set radio in Rx mode
}

void SetRxMode(void)//���ý���ģʽ
{
        TXEN_0;
	TRX_CE_1;
	delay(1); 					// Delay for mode change(>=650us)
}

void Spiwrite_byte(uchar byte)//��spi������ģ��Ĵ�����д����
{
  uchar i;
  data_buff=byte;
  for(i=0;i<8;i++)
  {
    if((data_buff&0x80)!=0)
      MOSI_1;
    else      
      MOSI_0;
    SCK_1;                 //�½���д
    data_buff=data_buff<<1;//Ϊ��һ�ε�д��׼��
    SCK_0;   
  }
}

uchar Spiread_byte()//��spi����������ģ��Ĵ����ж�����
{
  uchar data_buff=0x00;//������Ž��ܵ�������
  uchar i;
  for(i=0;i<8;i++)
  {
    data_buff=data_buff<<1;//Ϊ��һ����׼��
    SCK_1;//�½��ض�
    if((P5IN&0x04)!=0)
      data_buff |=0x01;
    else
      data_buff &=~(0x01);
    SCK_0; 
  }
  return data_buff;
}

unsigned char CheckDR(void)		//����Ƿ��������ݴ��� Data Ready
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
    SetRxMode();//���ý���ģʽ
    delay(1);
    while(CheckDR()==0);//�ж��Ƿ������ݴ���
    TRX_CE_0;
    CSN_0;//ʹ������ģ���spi
    delay(1);
    Spiwrite_byte(RRP);//�����ݽ��ܼĴ�����Ԥ����ָ��
    delay(1);
    for(i=0;i<8;i++)
    {
      acception[i]=Spiread_byte();
    }
    CSN_1;
    delay(5);//�ȴ�DR��AM���õ�     
}

void Config905() //��ʼ��RF�Ĵ���
{
  uchar i;
  CSN_0;//ʹ������ģ���spi
  Spiwrite_byte(WC);//дRF�Ĵ�����Ԥ����ָ��
  for(i=0;i<10;i++)
  {
    Spiwrite_byte(RFConf[i]);//дRF�еĸ���������Ϣ
  }
  CSN_1;//��ֹspiʹ��  
}

void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5ΪUART���շ��˿�
  U0CTL|=CHAR;     //8_bit������
  U0TCTL|=SSEL1;   //ʱ��Դѡ��smClk
  UBR00=0x41;   //��������ָΪ9600
  UBR10=0x03;                             
  UMCTL0=0x00;
  UCTL0&= ~SWRST;  // ��ʼ��USART0��״̬
  ME1 |= URXE0+UTXE0;   //���շ�����
  IE1 |= URXIE0;       //�򿪽���ʹ���ж�
}


void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  Close_LED();
  USART_Init();
  _EINT();  //ȫ��ʹ���ж�
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��
  while(1)
  {
      RxPacket();
      for(j=0;j<8;j++)
      {
        U0TXBUF=acception[j];
        while((IFG1&UTXIFG0)==0); //�ȴ��������
      }
    
  }
}
