#include <msp430x16x.h>
#include "system.h"


void USART_Init(void)
{
    
  P3SEL |= 0x30;    //P3.4,5ΪUART���շ��˿�
  U0CTL|=CHAR;     //8_bit������
  U0TCTL|=SSEL1;   //ʱ��Դѡ��smClk
  UBR00=0x41;   //��������ָΪ9600
  UBR10=0x03;                             
  UMCTL0=0x00;
  UCTL0&= ~SWRST;  // ��ʼ��USART0��״̬
  ME1 |= URXE0+UTXE0;   //���շ�����
  IE1 |= URXIE0;       //�򿪽���ʹ���ж�
}