/**
 * @file main.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief Standard main
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <includes.h>

extern app_t app;

extern volatile uint8_t timerFiredFlag;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer(void) {
  portENTER_CRITICAL_ISR(&timerMux);
  timerFiredFlag = 1;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  // // Do not modify the following lines ...
  SERIAL_INIT
  M5.begin(true, true, false, false);
  display_init();
  setCpuFrequencyMhz(160);
  MPRINT("Starting initialisation process")
 
  display_println("Trying to connect to the WiFi...");
  wdt_config_hal(WDT_VERY_LONG_TIMEOUT);
  app_init();
  timer_init();
  if(wifi_hal_handle_connect("TEMP_AP","")){
    app.WiFiOnline = ONLINE;
    app.WiFiMacAddress = String(WiFi.macAddress());
    app.WiFiLocalIP = WiFi.localIP().toString();
    MPRINT("Connected")
    MPRINT("My MAC is: "+app.WiFiMacAddress+", my IP: "+app.WiFiLocalIP)
    display_println("Connected to WiFI, initializing the rest of the sensors...");
  }else{
    app.WiFiOnline = OFFLINE;
    MPRINT("Not connected")
    display_println("Couldn't connect to WiFI, initializing the rest of the sensors...");
  }
  // ... until here
  MPRINT("M5 stack init completed")
  // Init BME sensor
  bme_init();
  eeprom_init();
  buttons_init();
  MPRINT("BME init completed")
  if(app.WiFiOnline == ONLINE){
    app_ota_init();
    webserver_init();
    MPRINT("Server init completed")

    // Setup alarm
    alarm_init();
    MPRINT("Alarm setup completed")

    // // Init Blynk
    // blynk_init();
  }
  
  // Leave the WDT at the end
  wdt_config_hal(WDT_SHORT_TIMEOUT);
  display_println("Initialization succesful!");
  app.flags.readBME = 1; // Initiate the first sensor reading
  // Reset display
  display_clear();
  update_time();
  eeprom_recover_alarm_data();
}

void loop() {
  timer_monitor(); // This should always stay at the top
  timer_process(); 

  app_loop();
  // blynk_loop();
  
  display_monitor();
  display_process();

  buttons_monitor();

  alarm_process();

  wdt_reset_hal(); // This should always stay at the bottom
}

/**
 * @brief ALl the timers to process need to be placed here in the main
 * 
 */
 void timer_process(void){
  #ifdef OTA
   // Reset the timer and execute the function
   if(!app.timers.OTAUpdateTimer){
    app.timers.OTAUpdateTimer = OTA_UPDATE_POLL_PERIOD_MS;
    app.flags.launchOTA = 1;
   }
  #endif
  if(!app.timers.bmeReadingTimer){
    app.timers.bmeReadingTimer = BME_POLLING_PERIOD_MS;
    app.flags.readBME = 1;
  }
  if(app.displayState && app.flags.displayAutoTurnOffFlag){
    if(!app.timers.displayOffTimer){
      app.timers.displayOffTimer = DISPLAY_OFF_TIMEOUT_TIME_MS;
      app.flags.setDisplayOff = 1;
    }
  }
}