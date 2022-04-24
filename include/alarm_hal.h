#ifndef ALARM_HAL_H
#define ALARM_HAL_H

#include "defs.h"

void alarm_init(void);
void alarm_start(void);
void alarm_stop(void);
uint8_t alarm_do_ring(void);
void alarm_process(void);

#endif