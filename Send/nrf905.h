#ifndef _nrf905
#define _nrf905

void nRF905_IO_set(void);
void TxPacket(unsigned char *sended_data,unsigned char length);//���ͺ���
void Config905(); //��ʼ��RF�Ĵ���
void response();   //Ӧ����
void RxPacket();      //������ģ���������
#endif