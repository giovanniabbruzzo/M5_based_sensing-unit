/**
 * @file app.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief All app related funcions go here
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "app.h"
#include "includes.h"
#include "debug_utils.h"

app_t app;

/**
 * @brief Init all the app related variables
 * 
 */
void app_init(void){
    app.WiFiOnline = OFFLINE;
    app.WiFiMacAddress = "";
    app.WiFiLocalIP = "";
    app.aq.temp = 0;
    app.aq.press = 0;
    app.aq.hum = 0;
    app.aq.gas_res = 0;
    app.aq.alt = 0;

    app.displayState = 1;

    app.hapticState = 0;
    
    app.flags.val = 0;
}

/**
 * @brief Init the OTA related variables
 * 
 */
void app_ota_init(void){
 // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      MPRINT("Start updating " + type);
    })
    .onEnd([]() {
      MPRINT("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) MPRINT("Auth Failed")
      else if (error == OTA_BEGIN_ERROR) MPRINT("Begin Failed")
      else if (error == OTA_CONNECT_ERROR) MPRINT("Connect Failed")
      else if (error == OTA_RECEIVE_ERROR) MPRINT("Receive Failed")
      else if (error == OTA_END_ERROR) MPRINT("End Failed")
    });

  ArduinoOTA.begin();
}

/**
 * @brief All the app related loops go in here
 * 
 */
void app_loop(void){
  M5.update();
  if(app.flags.launchOTA){
      app.flags.launchOTA = 0;
      wdt_config_hal(WDT_VERY_LONG_TIMEOUT);
      ArduinoOTA.handle();
      wdt_config_hal(WDT_SHORT_TIMEOUT);
  }
  if(app.flags.readBME){
    app.flags.readBME = 0;
    MPRINT("Reading BME...")
    bme_read_data();
  }
}   