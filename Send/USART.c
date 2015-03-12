#include <msp430x16x.h>
#include "system.h"

extern uchar RBuff[8];
extern uchar Receive_Flag; 
void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5ΪUART���շ��˿�
  U0CTL|=CHAR;     //8_bit������
  U0TCTL|=SSEL1;   //ʱ��Դѡ��smclk
  UBR00=0x41;   //��������ָΪ9600
  UBR10=0x03;                             
  UMCTL0=0x00;
  UCTL0&= ~SWRST;  // ��ʼ��USART0��״̬
  ME1 |= URXE0;   //���շ�����
  IE1 |= URXIE0;       //�򿪽���ʹ���ж�
}

#pragma vector=UART0RX_VECTOR //���ڽ����ж�
__interrupt void USART0_RXIRQ (void) 
{ 
  static uchar count =0;
  uchar temp;
  temp = RXBUF0;
  if(count<7)
  {
    RBuff[count] = temp;
    count++;
  }
  
  else
  {
    RBuff[count] = temp;
    count=0;
    Receive_Flag=1;//��λ���ձ�־λ
  }
  
}