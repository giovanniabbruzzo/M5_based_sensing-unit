/**
 * @file timer_hal.h
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

// Timer variables
extern volatile uint8_t timerFiredFlag;
extern portMUX_TYPE timerMux;

void onTimer(void);
void timer_init(void);
void timer_monitor(void);
/**
 * @brief This function needs to be placed in the main
 * 
 */
void timer_process(void);

#endif // TIMER_H