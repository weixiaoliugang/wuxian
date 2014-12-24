
#include <msp430x16x.h>
#include "system.h"
void Init_Clk(void)
{
   unsigned int i;   
   BCSCTL1 &= ~XT2OFF; //使TX2有效,TX2上电时默认为关闭的.
  do 
  { 
   IFG1 &= ~OFIFG; //清振荡器失效标志 
   for(i= 0xff; i>0; i--); //延时,待稳定. 
   } 
  while ((IFG1 & OFIFG)!=0);  //若振荡器失效标志有效,一直等待 
   BCSCTL2 |=SELM_2;   //使MCLK选择XT2且MCLK=XT2=8MHZ ，使SMCLK选择XT2且SMCLK=XT2=8MHZ
   BCSCTL2 |=SELS;  		
}

void delay(uint n)//延时1ms
{
    uint j;
    uchar i;
    for(j = n;j > 0;j--)
	for(i = 150;i > 0;i--)  
          _NOP();
}
