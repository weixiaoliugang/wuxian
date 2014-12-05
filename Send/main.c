
#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"
#include "nrf905.h"
#include "USART.h"

uchar RBuff[8];
uchar Receive_Flag=0;
void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  USART_Init();//���ڳ�ʼ��
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��  
  _EINT();  //ȫ��ʹ���ж�
  while(1)
  {
    if(Receive_Flag==1)
    {
      TxPacket(RBuff);
      Receive_Flag=0;//���ձ�־λ��0
    }
    
    
  }
}

 
  
