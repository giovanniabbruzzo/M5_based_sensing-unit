/**
 * @file includes.h
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief This file will include all the necessary library to make
 *        a standard M5 Core 2 module work. It has to be included
 *        in the main. 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INCLUDES_H
#define INCLUDES_H

// Includes
#include "defs.h"
#include "debug_utils.h"
#include "timer_hal.h" // Timer
#include "esp_wdt_hal.h" // WDT
#include "app.h"
#include "wifi_manger_hal.h" // WiFi Manager
#ifdef OTA
#include <ArduinoOTA.h>
#endif
#ifdef ARDUINO_ORA
#include <ArduinoOTA.h>
#endif
#ifdef MATH
#include "common_math.h"
#endif
// BME680 sensor
#include "bme680_hal.h"
// M5 Core 2
#include <M5Core2.h>
#include "display.h"
#include "touch_buttons.h"
// Async Server
#include "webserver.h"
// Blynk
#include "blynk_hal.h"
// Alarm
#include "alarm_hal.h"
// EEPROM
#include "eeprom_hal.h"

#endif // INCLUDES_H