
#include <msp430x16x.h>
#include "system.h"
void Init_Clk(void)
{
   unsigned int i;   
   BCSCTL1 &= ~XT2OFF; //ʹTX2��Ч,TX2�ϵ�ʱĬ��Ϊ�رյ�.
  do 
  { 
   IFG1 &= ~OFIFG; //������ʧЧ��־ 
   for(i= 0xff; i>0; i--); //��ʱ,���ȶ�. 
   } 
  while ((IFG1 & OFIFG)!=0);  //������ʧЧ��־��Ч,һֱ�ȴ� 
   BCSCTL2 |=SELM_2;   //ʹMCLKѡ��XT2��MCLK=XT2=8MHZ ��ʹSMCLKѡ��XT2��SMCLK=XT2=8MHZ
   BCSCTL2 |=SELS;  		
}

void delay(uint n)//��ʱ1ms
{
    uint j;
    uchar i;
    for(j = n;j > 0;j--)
	for(i = 150;i > 0;i--)  
          _NOP();
}
