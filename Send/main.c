/***********************************************
            ����Ӧ���ź�Ϊ11111111
***********************************************/
#include <msp430x16x.h>
#include "system.h"
#include "wixian.h"
#include "nrf905.h"
#include "USART.h"

uchar RBuff[8];
uchar acception[1]={3};
uchar Receive_Flag=0;
uchar aa[8]={5,5,5,5,5,5,5,5};
uchar flag=0;
uint  count=0;
void main()
{
  uchar j;
  WDTCTL = WDTPW + WDTHOLD;
  Init_Clk();//ʱ�ӳ�ʼ��
  USART_Init();//���ڳ�ʼ��
  nRF905_IO_set();
  Config905();//RF���üĴ����ĳ�ʼ��  
  _EINT();  //ȫ��ʹ���ж�
  while(1)
  {
    if(flag==0)//�˾�����������
    {
      if(Receive_Flag==1)
      {      
        TxPacket(RBuff,8);
        Receive_Flag=0;//���ձ�־λ��0
        flag=1;       //�˾�����������
        delay(10);
        P4DIR=0x47;
        P4OUT&=~BIT6;
        P4DIR=0x07;
        response();
      } 
    }
    else
    {
      for(j=50;j>0;j--)
      {
        TxPacket(aa,8);
      }
      response();//Ӧ����
      Receive_Flag=0;//���ձ�־λ��0
    }
  }
}

