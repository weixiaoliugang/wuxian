#ifndef _nrf905
#define _nrf905


void nRF905_IO_set(void);
void TxPacket(unsigned char *sended_data );//发送函数
void RxPacket();//接收函数
void Config905(); //初始化RF寄存器

#endif