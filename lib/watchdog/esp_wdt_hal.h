/**
 * @file esp_wdt_hal.h
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef ESP_WDT_HAL_H
#define ESP_WDT_HAL_H

#include <Arduino.h>

#define WDT_SHORT_TIMEOUT 5 // s
#define WDT_LONG_TIMEOUT 60 // s
#define WDT_VERY_LONG_TIMEOUT 5*60 // s

void wdt_config_hal(uint32_t timeout);
void wdt_reset_hal(void);


#endif // ESP_WDT_HAL_H