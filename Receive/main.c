/*******************************************************
     ��һ�½����뷢�͵�ַ�ľ��庬��,Ӧ���ź�Ϊ1111111
*******************************************************/
#include <msp430x16x.h>
#include "wuxian.h"
#include "nrf905.h"
#include "system.h"
#include "USART.h"

uchar RBuff[1]={1};
uchar acception[8];//������Ž��յ�������
uint count=0;//����ֵ
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
      delay(10);
      for(j=2;j>0;j--)
      {
        TxPacket(RBuff,1); //����Ӧ���ź�
        P4DIR=0x47;
        P4OUT&=~BIT6;
        P4DIR=0x07;
      }
      count++;
  }
}
