
#ifndef __DHT11_H__
#define __DHT11_H__

#include "pindef.h"

#define DHT_DATA_IN()  {DHT_PORT->CRH&=0xFFFFFFF0;DHT_PORT->CRH|=((unsigned int)8<<0);}//PB3
#define DHT_DATA_OUT() {DHT_PORT->CRH&=0xFFFFFFF0;DHT_PORT->CRH|=(unsigned int)(3<<0);}

#endif


