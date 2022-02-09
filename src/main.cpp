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
  M5.begin(true, false, false, false);
  display_init();
  setCpuFrequencyMhz(240);
  MPRINT("Starting initialisation process")
 
  display_println("Trying to connect to the WiFi...");
  wdt_config_hal(WDT_VERY_LONG_TIMEOUT);
  app_init();
  timer_init();
  if(wifi_hal_handle_connect("TEMP_AP","temp_01234")){
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
  MPRINT("BME init completed")
  app_ota_init();
  
  // Leave the WDT at the end
  wdt_config_hal(WDT_SHORT_TIMEOUT);
  display_println("Initialization succesful!");
  app.flags.readBME = 1; // Initiate the first sensor reading
}

void loop() {
  timer_monitor(); // This should always stay at the top
  timer_process(); 

  app_loop();
  
  display_monitor();
  display_process();

  buttons_monitor();
  buttons_proces();

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
  if(app.displayState){
    if(!app.timers.displayOffTimer){
      app.timers.displayOffTimer = DISPLAY_OFF_TIMEOUT_TIME_MS;
      app.flags.setDisplayOff = 1;
    }
  }
}