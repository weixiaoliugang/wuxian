#include <msp430x16x.h>
#include "wuxian.h"
#include "system.h"
#include "USART.h"

uchar data_buff;
uchar acception[8];//������Ž��յ�������

uchar RFConf[10]=
{                             
  0x4c,                             //CH_NO,����Ƶ����430MHZ
  0x0C,                             //�������Ϊ10db,���ط����ڵ�Ϊ����ģʽ
  0x44,                             //��ַ������ã�Ϊ4�ֽ�
  0x20,0x20,                        //���շ�����Ч���ݳ���Ϊ32�ֽ�
  0xCC,0xCC,0xCC,0xCC,              //���յ�ַ
  0x58,                              //CRC����8λCRCУ�飬�ⲿʱ���źŲ�ʹ�ܣ�16M����
};



void main()
{
  uchar j;
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  USART_Init();
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��
  _EINT();  //ȫ��ʹ���ж�
  while(1)
  {
      RxPacket();                  //��������
      for(j=0;j<8;j++)
      {
        U0TXBUF=acception[j];
        while((IFG1&UTXIFG0)==0); //�ȴ��������
      }
  }
}
