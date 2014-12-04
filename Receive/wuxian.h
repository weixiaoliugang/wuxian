#ifndef __wuxian
#define __wuxian


#define  TXEN_0     P4OUT &=~BIT0          //���0, P4.0,1����ģʽ
#define  TXEN_1     P4OUT |= BIT0          //���1��P4.0,0����ģʽ
//==============================================================================
#define  TRX_CE_0   P4OUT &=~BIT1    //���0, P4.1, 0 �������ս�ֹ
#define  TRX_CE_1   P4OUT |= BIT1    //���1, P4.1, 1 ��������ʹ��
//==============================================================================
#define  PWR_0      P4OUT &=~BIT2    //���0,  P4.2,  0 δ�ϵ�
#define  PWR_1      P4OUT |= BIT2    //���1,  P4.2,  1�ϵ�
//===================================����ӳ�===================================
#define  MISO_0     P5OUT &=~BIT2     // ���0,  P5.2,  0   RF905����Ƭ�� 
#define  MISO_1     P5OUT |= BIT2      // ���1,  P5.2, 1   RF905����Ƭ��  
//==================================��������====================================
#define  MOSI_0     P5OUT &=~BIT1         // ���0,  P5.1,  0   ��Ƭ����RF905
#define  MOSI_1     P5OUT |= BIT1         // ���1,  P5.1,  1   ��Ƭ����RF905
//===================================SPIʱ�Ӷ˿�================================
#define  SCK_0      P5OUT &=~BIT3         //���0,  P5.3,  0 ��Ƭ����RF905ʱ�ӵ͵�ƽ
#define  SCK_1      P5OUT |= BIT3         //���1,  P5.3,  1 ��Ƭ����RF905ʱ�Ӹߵ�ƽ
//===================================SPIʹ�ܶ˿�=================================
#define  CSN_0      P5OUT &=~BIT0   //SPI �͵�ƽ  P5.0 ʹ��SPI
#define  CSN_1      P5OUT |= BIT0   //SPI �ߵ�ƽ P5.0 ��ֹSPI
//==================================����Ϊ״̬�˿�==============================
//===========================AM  ��ַƥ��=======================================
#define  AM_0       P4OUT &=~BIT5     //  ��ַƥ�� P4.5 
#define  AM_1       P4OUT |= BIT5     //  ��ַƥ�� P4.5 
//=================================DR ���ݽ���״̬==============================
#define  DR_0       P4OUT &=~BIT6
#define  DR_1       P4OUT |= BIT6
//=====================================CD �ز�����״̬==========================
#define  CD_0       P4OUT &=~BIT4
#define  CD_1       P4OUT |= BIT4
//===========================NRF905:SPIָ��=====================================
#define WC		0x00
#define RC		0x10
#define WTP		0x20
#define RTP		0x21
#define WTA		0x22
#define RTA		0x23
#define RRP		0x24

void nRF905_IO_set(void);
void SetRxMode(void);//���ý���ģʽ
void Spiwrite_byte(unsigned char byte);//��spi������ģ��Ĵ�����д����
unsigned char Spiread_byte();//��spi����������ģ��Ĵ����ж�����
unsigned char CheckDR(void);//����Ƿ��������ݴ��� Data Ready
void RxPacket();
void Config905(); //��ʼ��RF�Ĵ���

#endif