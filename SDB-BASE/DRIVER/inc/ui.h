
#ifndef __UI_H__
#define __UI_H__

#include "lcd.h"
#include "delay.h"
#include "bsp.h"

extern Alarm_obj Alarm;
extern Battery_obj Battery;
extern Bluetooth_obj Bluetooth;
extern Internet_obj  Internet;
extern Environ_sensor Temperature;
extern ProgressBar_obj ProgressBar;
extern Calendar_obj Calendar;

void Home_Init(void);
void Calendar_UI(Calendar_obj *Calendar_info);
void Temperature_UI(Environ_sensor *Temperature_info);
void Battery_UI(Battery_obj *Battery_info);
void Bluetooth_UI(Bluetooth_obj *Bluetooth_info);
void ProgressBar_UI(ProgressBar_obj *ProgressBar_info);
void Alarm_UI(Alarm_obj *Alarm_info);
void Internet_UI(Internet_obj *Internet_info);


#endif
