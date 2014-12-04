
#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"
#include "nrf905.h"
#include "USART.h"

uchar RBuff[8];

void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  USART_Init();//���ڳ�ʼ��
  _EINT();  //ȫ��ʹ���ж�
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��  
  while(1)
  {
    TxPacket();
  }
}

 
  
