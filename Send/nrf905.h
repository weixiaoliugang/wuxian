#ifndef _nrf905
#define _nrf905



void nRF905_IO_set(void);
void SetTxMode(void);//设置发送模式
void Spiwrite_byte(unsigned char byte);//用spi向无线模块寄存器中写数据
void TxPacket(void);
void Config905(); //初始化RF寄存器


#endif