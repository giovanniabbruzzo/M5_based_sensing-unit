#ifndef TOUCH_BUTTON_H
#define TOUCH_BUTTON_H

#include "defs.h"

void buttons_init(void);
void buttons_monitor(void);
void button_tap(Event& e);
void button_double_tap(Event& e);
void button_long_pressed(Event& e);
void button_touch(Event& e);
void button_haptics(uint16_t time_ms);
#endif