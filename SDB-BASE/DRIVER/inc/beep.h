

#ifndef __BEEP_H__
#define __BEEP_H__


typedef struct
{
	unsigned short int conter; //��������ۼ���
	unsigned char num:7; //��������
	unsigned char en:1;//����״̬
	unsigned int rt;//����ʱ��
}Beep_st;

extern Beep_st Beep;
void Beep_Thread(Beep_st *Beep_info);//��������

#endif


