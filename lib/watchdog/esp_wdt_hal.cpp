/**
 * @file esp_wdt_hal.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "esp_wdt_hal.h"
#include <esp_task_wdt.h> // WDT

void wdt_config_hal(uint32_t timeout){
  esp_task_wdt_deinit();
  esp_task_wdt_init(timeout, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);
}

void wdt_reset_hal(void){
  esp_task_wdt_reset();
}