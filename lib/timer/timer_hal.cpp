/**
 * @file timer_hal.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief Provides a 1ms timer
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "timer_hal.h"
#include "defs.h"

hw_timer_t * timer = NULL;
volatile uint8_t timerFiredFlag;
extern app_t app;

/**
 * @brief Initialize a 1ms timer only if the CPU is 240 Mhz
 * 
 */
void timer_init(void){
  // Configure the timer
  timer = timerBegin(0, 80, true); // APB is 80 if CPU is 240
  // interruptCounter = 0;
  timerFiredFlag = 0;
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true); // Set timer every 1 ms
  timerAlarmEnable(timer);
  // Timers variable
#ifdef OTA
  app.timers.OTAUpdateTimer = OTA_UPDATE_POLL_PERIOD_MS;
#endif
  app.timers.generalTestTimer = 0;
  // BME
  app.timers.bmeReadingTimer = BME_POLLING_PERIOD_MS;
  // Display off
  app.timers.displayOffTimer = DISPLAY_OFF_TIMEOUT_TIME_MS;
  app.timers.mqttSendTimer = MQTT_SEND_PERIOD_MS;
}

/**
 * @brief ALl the timers to monitor need to be placed here
 * 
 */
void timer_monitor(void){
  if (timerFiredFlag) {
    portENTER_CRITICAL(&timerMux);
    timerFiredFlag = 0;
#ifdef OTA
    if(app.timers.OTAUpdateTimer){
      app.timers.OTAUpdateTimer--;
    }
#endif
    if(app.timers.generalTestTimer){
      app.timers.generalTestTimer--;
    }
    if(app.timers.bmeReadingTimer){
      app.timers.bmeReadingTimer--;
    }
    if(app.timers.displayOffTimer){
      app.timers.displayOffTimer--;
    }
    if(app.timers.mqttSendTimer){
      app.timers.mqttSendTimer--;
    }

    portEXIT_CRITICAL(&timerMux);
  }
}