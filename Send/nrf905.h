#ifndef _nrf905
#define _nrf905



void nRF905_IO_set(void);
void SetTxMode(void);//���÷���ģʽ
void Spiwrite_byte(unsigned char byte);//��spi������ģ��Ĵ�����д����
void TxPacket(void);
void Config905(); //��ʼ��RF�Ĵ���


#endif