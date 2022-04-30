/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/
#include "blynk_hal.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "defs.h"
#include "eeprom_hal.h"

extern app_t app;

char auth[] = BLYNK_AUTH_TOKEN;
char* ssid = SSID_BLYNK;
char* pass = PSK_BLYNK;

BLYNK_WRITE(V4){
  app.alarm.flags.set = param.asInt();
  app.flags.updateDisplay = 1;
  if(app.alarm.flags.set){
      MPRINT("Alarm is on")
  }else{
      MPRINT("Alarm is off")
  }
  eeprom_store_alarm_data();
}

BLYNK_WRITE(V5){  
  TimeInputParam t(param);
  MPRINT("Alarm to start at")
  app.alarm.alarmClock.h = t.getStartHour();
  app.alarm.alarmClock.m = t.getStartMinute();
  app.flags.updateDisplay = 1;
  eeprom_store_alarm_data();
  MPRINT(String(app.alarm.alarmClock.h)+":"+String(app.alarm.alarmClock.m))
}

void blynk_init(void){
  Blynk.begin(auth, ssid, pass);

  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  eeprom_recover_alarm_data();
  Blynk.virtualWrite(V4, app.alarm.flags.set);
  Blynk.virtualWrite(V5, 0);
}

void blynk_loop(void){
  if(app.WiFiOnline == ONLINE){
    if(!Blynk.connected()){
      Blynk.connect();
    }
      Blynk.run();
    if(app.flags.updateBlynk){
      MPRINT("Sending data to Blynk...")
      app.flags.updateBlynk = 0;
      Blynk.virtualWrite(V0, app.aq.temp);
      Blynk.virtualWrite(V1, app.aq.hum);
      Blynk.virtualWrite(V2, app.aq.gas_res);
      Blynk.virtualWrite(V6, app.WiFiLocalIP);
      Blynk.virtualWrite(V3, millis() / 1000);
    }
  }
}
