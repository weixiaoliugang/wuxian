
#include <msp430x16x.h>
#include "wixian.h"
#define uchar unsigned char
#define uint  unsigned int

uchar data_buff;
uchar RBuff[8];
uchar address[4]={0xCC,0xCC,0xCC,0xCC};//���ܵĵ�ַ
uchar Flag_Receive = 0;
uchar temp;
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

void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5ΪUART���շ��˿�
  U0CTL|=CHAR;     //8_bit������
  U0TCTL|=SSEL0;   //ʱ��Դѡ��AClk
  UBR00=0x03;   //��������ָΪ9600
  UBR10=0x00;                             
  UMCTL0=0x4A;
  UCTL0&= ~SWRST;  // ��ʼ��USART0��״̬
  ME1 |= URXE0;   //���շ�����
  IE1 |= URXIE0;       //�򿪽���ʹ���ж�
  _EINT();  //ȫ��ʹ���ж�
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

void SetTxMode(void)//���÷���ģʽ
{
        TXEN_1;// Delay for mode change(>=650us)
	TRX_CE_0;
	delay(1); 					
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
    delay(1);
    SCK_0;   
  }
}


//������ģ�鷢������
void TxPacket(void)
{
  uchar i,j;
  CSN_0;//ʹ������ģ���spi
  Spiwrite_byte(WTP);//д���ݷ��ͼĴ�����Ԥ����ָ��
  delay(1);
  for(i=0;i<8;i++)
  {
    Spiwrite_byte(RBuff[i]);//д��Ҫ���͵�����
  }
  CSN_1;//�ر�����ģ���spi
  delay(5);
  
  CSN_0;//ʹ������ģ���spi
  Spiwrite_byte(WTA);//д���ݷ��ͼĴ�����Ԥ����ָ��
  delay(1);
  for(j=0;j<4;j++)
  {
    Spiwrite_byte(address[j]);  
  }
  CSN_1;//�ر�����ģ���spi
  SetTxMode();//���÷���ģʽ
  TRX_CE_1;					// �򿪷���ʹ��
  delay(1);					// while (DR!=1);�ĸ�
  TRX_CE_0;	                                // �رշ���ʹ��
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

void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  USART_Init();//���ڳ�ʼ��
  Init_Clk();//ʱ�ӳ�ʼ��
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��  
  while(1)
  {
    TxPacket();
  }
}

 
  
#pragma vector=UART0RX_VECTOR //���ڽ����ж�
__interrupt void USART0_RXIRQ (void) 
{ 
//  �ݴ��������
temp = RXBUF0; 
// 8 �ֽڽ��ܻ������
RBuff[0] = RBuff[1]; 
RBuff[1] = RBuff[2]; 
RBuff[2] = RBuff[3]; 
RBuff[3] = RBuff[4]; 
RBuff[4] = RBuff[5]; 
RBuff[5] = RBuff[6]; 
RBuff[6] = RBuff[7]; 
RBuff[7] = temp;
//  �ж�������Ч(��Ч�����ñ�־λ���ȴ�����) 
if (RBuff[0] == 0xd0){Flag_Receive = 1;return; }else{Flag_Receive = 0;} 
}