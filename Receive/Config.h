#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

/*��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��*/
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ

#define LED8PORT        P2OUT                   //P2��8��LED��
#define LED8SEL         P2SEL                   //P2��8��LED��
#define LED8DIR         P2DIR                   //P2��8��LED��

#define DATAPORT	P4OUT                   //���ݿ����ڶ˿�P4
#define DATASEL         P4SEL                   //���ݿڹ��ܼĴ��������ƹ���ģʽ
#define DATADIR         P4DIR                   //���ݿڷ���Ĵ���

#define CTRPORT	        P6OUT                   //���������ڵĶ˿�P6
#define CTRSEL          P6SEL                   //���ƿڹ��ܼĴ��������ƹ���ģʽ
#define CTRDIR          P6DIR                   //���ƿڷ���Ĵ���

#define DCTR0	        P6OUT &= ~BIT4          //����ܶο���λ�ź��õ�
#define DCTR1           P6OUT |=  BIT4          //����ܶο���λ�ź��ø�
#define WCTR0	        P6OUT &= ~BIT3          //�����λ����λ�ź��õ�
#define WCTR1           P6OUT |=  BIT3          //�����λ����λ�ź��ø�

#define KEYPORT	        P1OUT                   //�������ڵĶ˿�P1
#define KEYSEL          P1SEL                   //���ƿڹ��ܼĴ��������ƹ���ģʽ
#define KEYDIR          P1DIR                   //���ƿڷ���Ĵ���
#define KEYIN           P1IN                    //����ɨ���ж���Ҫ��ȡIO��״ֵ̬

#define DACCS0          P5OUT &= ~BIT4          //DACƬѡ�ź��õ�
#define DACCS1          P5OUT |=  BIT4          //DACƬѡ�ź��ø�
#define DIN0            P3OUT &= ~BIT1          //DAC DIN�ܽ�λ����Ϊ0
#define DIN1            P3OUT |=  BIT1          //DAC DIN�ܽ�λ����Ϊ1
#define DOUT0           P3OUT &= ~BIT2          //DAC DOUT�ܽ�λ����Ϊ0
#define DOUT1           P3OUT |=  BIT2          //DAC DOUT�ܽ�λ����Ϊ0
#define SCLK0           P3OUT &= ~BIT3          //DAC SCLK�ܽ�λ����Ϊ0
#define SCLK1           P3OUT |=  BIT3          //DAC SCLK�ܽ�λ����Ϊ0

#define SOUNDON         P6OUT &= ~BIT2          //������������
#define SOUNDOFF        P6OUT |=  BIT2          //������������

#define RS_CLR	        P5OUT &= ~BIT5           //RS�õ�
#define RS_SET	        P5OUT |=  BIT5           //RS�ø�

#define RW_CLR	        P5OUT &= ~BIT6           //RW�õ�
#define RW_SET	        P5OUT |=  BIT6           //RW�ø�

#define EN_CLR	        P5OUT &= ~BIT7           //E�õ�
#define EN_SET	        P5OUT |=  BIT7           //E�ø�

#define PSB_CLR	        P5OUT &= ~BIT0            //PSB�õͣ����ڷ�ʽ
#define PSB_SET	        P5OUT |=  BIT0            //PSB�øߣ����ڷ�ʽ

#define RST_CLR	        P5OUT &= ~BIT1            //RST�õ�
#define RST_SET	        P5OUT |= BIT1             //RST�ø�

#define DataPort        P4OUT                     //P4��Ϊ���ݿ�

/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN	0x01		          //����ָ�������ACֵΪ00H
#define AC_INIT		0x02		          //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD	0x06		          //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE	0x30		          //����ģʽ��8λ����ָ�
#define DISPLAY_ON	0x0c		          //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF	0x08		          //��ʾ��
#define CURSE_DIR	0x14		          //�α������ƶ�:AC=AC+1
#define SET_CG_AC	0x40		          //����AC����ΧΪ��00H~3FH
#define SET_DD_AC	0x80                      //����DDRAM AC

uchar   key=0xFF;                                 //��ֵ����

//***********************************************************************
//			�����������ʾ�Ķ����
//***********************************************************************

uchar table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
               0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ��
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}
//***********************************************************************
//               MSP430�ڲ����Ź���ʼ��
//***********************************************************************
void WDT_Init()
{
   WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
}

//***********************************************************************
//		�ر������
//***********************************************************************
void Close_LED()
{
  DATASEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  DATADIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  DATAPORT = 0xFF;                      //P4�ڳ�ʼ����ΪFF
  
  CTRSEL  =  0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  CTRDIR |=  BIT3 + BIT4;               //����IO�ڷ���Ϊ���,���ƿ���P63,P64
  CTRPORT =  0xFF;                      //P6�ڳ�ʼ����ΪFF  
  
  DCTR1;                                //�ص�����ܣ�������ʾ����                   
  WCTR1;                     
  DATAPORT=0xFF;                        //�������            
  WCTR0; 
}