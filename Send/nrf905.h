#ifndef _nrf905
#define _nrf905

void nRF905_IO_set(void);
void TxPacket(unsigned char *sended_data,unsigned char length);//发送函数
void Config905(); //初始化RF寄存器
void response();   //应答函数
void RxPacket();      //用无线模块接收数据
#endif