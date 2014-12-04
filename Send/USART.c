#include <msp430x16x.h>
#include "system.h"

extern uchar RBuff[8];
uchar Flag_Receive = 0;

void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5ΪUART���շ��˿�
  U0CTL|=CHAR;     //8_bit������
  U0TCTL|=SSEL0;   //ʱ��Դѡ��AClk
  UBR00=0x03;   //��������ָΪ9600
  UBR10=0x00;                             
  UMCTL0=0x4A;
  UCTL0&= ~SWRST;  // ��ʼ��USART0��״̬
  ME1 |= URXE0;   //���շ�����
  IE1 |= URXIE0;       //�򿪽���ʹ���ж�
}

#pragma vector=UART0RX_VECTOR //���ڽ����ж�
__interrupt void USART0_RXIRQ (void) 
{ 
//  �ݴ��������
  uchar temp = RXBUF0;
  // 8 �ֽڽ��ܻ������
  RBuff[0] = RBuff[1]; 
  RBuff[1] = RBuff[2]; 
  RBuff[2] = RBuff[3]; 
  RBuff[3] = RBuff[4]; 
  RBuff[4] = RBuff[5]; 
  RBuff[5] = RBuff[6]; 
  RBuff[6] = RBuff[7]; 
  RBuff[7] = temp;
  //  �ж�������Ч(��Ч�����ñ�־λ���ȴ�����) 
  if (RBuff[0] == 0xd0)
  {
    Flag_Receive = 1;
    return; 
  }
  else
    Flag_Receive = 0; 
}