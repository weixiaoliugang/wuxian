/*******************************************************
     ��һ�½����뷢�͵�ַ�ľ��庬��,Ӧ���ź�Ϊ1111111
*******************************************************/
#include <msp430x16x.h>
#include "wuxian.h"
#include "nrf905.h"
#include "system.h"
#include "USART.h"

uchar RBuff[8]={1,1,1,1,1,1,1,1};
uchar acception[8];//������Ž��յ�������

void main()
{
  uchar j;
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  USART_Init();
  _EINT();  //ȫ��ʹ���ж�
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��
  
  while(1)
  {
      RxPacket(); //��������
     /* for(j=0;j<8;j++)//�͵�������ʾ
      {
        U0TXBUF=acception[j];
        while((IFG1&UTXIFG0)==0); //�ȴ��������
      }*/
      
      P4DIR=0x47;
      P4OUT&=~BIT6;
      P4DIR=0x07;
      delay(10);
      TxPacket(RBuff); //����Ӧ���ź�
      
  }
}
