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
uint count1=0;
void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��
  SetRxMode();//���ý���ģʽ
  //delay(1);
  _EINT();  //ȫ��ʹ���ж�
  while(1)
  {
    
  }
}


#pragma vector=PORT1_VECTOR     //�����ж�
__interrupt void KEY_PORT1(void)
{
  RxPacket(); //��������      
  delay(1);
  TxPacket(RBuff,1); //����Ӧ����
  count++;
  SetRxMode();//���ý���ģʽ 
  P1IFG=0x00;

  
}

