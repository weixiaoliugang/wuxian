#include <msp430x16x.h>
#include "system.h"
#include "wuxian.h"

extern uchar acception[8];//������Ž��յ�������
extern uchar RFConf[10];
uchar  address[4]={0xAA,0xAA,0xAA,0xAA};//���ܵĵ�ַ
uchar  RFConf[10]=
{                             
  0x4c,                             //CH_NO,����Ƶ����430MHZ
  0x0C,                             //�������Ϊ10db,���ط����ڵ�Ϊ����ģʽ
  0x44,                             //��ַ������ã�Ϊ4�ֽ�
  0x08,0x01,                        //������Ч���ݳ���Ϊ8�ֽڣ��������ݵ���Ч����Ϊ1��������
  0xBB,0xBB,0xBB,0xBB,              //�����ĵ�ַ
  0x58,                              //CRC����8λCRCУ�飬�ⲿʱ���źŲ�ʹ�ܣ�16M����
};

void nRF905_IO_set(void)       //�˿�����
{  
        P1DIR=0xfe;//����λ���������λ����
        P1IES=0x00;//�����ش���
        P1IE=0x01;//��������λ���ⲿ�ж�ʹ��
        P4DIR |= 0x07;
        P4DIR &= 0x8F;    
        P5DIR = 0x0B;
        P4SEL&=0x88;
        P5SEL&=0xF4;         //p4.0,p4.1,p4.2��������ֶ˿ڣ�p4.4,p4.5,p4.6���룬���ֶ˿�  
        CSN_1;				            // Spi 	disable
	SCK_0;				          // Spi clock line init low
	PWR_1;					 // nRF905 power on
	TRX_CE_0;				// Set nRF905 in standby mode
	TXEN_0;					// set radio in Rx mode
}

void Spiwrite_byte(uchar byte)//��spi������ģ��Ĵ�����д����
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
    SCK_1;                 //�½���д
    data_buff=data_buff<<1;//Ϊ��һ�ε�д��׼��
    //delay(1);
    SCK_0;   
  }
}

unsigned char Spiread_byte()//��spi����������ģ��Ĵ����ж�����
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

void SetRxMode(void)//���ý���ģʽ
{
        PWR_1;
        TXEN_0;
	TRX_CE_1;
	delay(1); 					// Delay for mode change(>=650us)
}

void SetTxMode(void)//���÷���ģʽ
{
        PWR_1;
        TXEN_1;// Delay for mode change(>=650us)
	TRX_CE_0;
	delay(1); 					
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

void RxPacket()      //������ģ���������
{
    uchar i;
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
    delay(10);//�ȴ�DR��AM���õ�     
}


void TxPacket(uchar *sended_data,uchar length)     //������ģ�鷢������
{
  uchar i,j;
  SetTxMode();//���÷���ģʽ
  //delay(5);
  CSN_0;//ʹ������ģ���spi
  Spiwrite_byte(WTP);//д���ݷ��ͼĴ�����Ԥ����ָ��
  delay(1);
  for(i=0;i<length;i++)
  {
    Spiwrite_byte(sended_data[i]);//д��Ҫ���͵�����
  }
  CSN_1;//�ر�����ģ���spi
  delay(1);
  CSN_0;//ʹ������ģ���spi
  Spiwrite_byte(WTA);//д���ݷ��ͼĴ�����Ԥ����ָ��
  delay(1);
  for(j=0;j<4;j++)
  {
    Spiwrite_byte(address[j]);  
  }
  CSN_1;//�ر�����ģ���spi 
  TRX_CE_1;					// �򿪷���ʹ��
  delay(10);					// ����Ҫ��һ��Ҫ��֤�㹻��ʱ��
  TRX_CE_0;	                                // �رշ���ʹ��
}




