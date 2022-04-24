#include "alarm_hal.h"
#include <M5Core2.h>
#include <driver/i2s.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "debug_utils.h"
#include "time.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

#define OUTPUT_GAIN 100

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

alarm_t appAlarm;
// Udate current time every 1 second
hour_min_t currTime;
unsigned long int timeUpdated = millis();


char* strToChar(String str) {
  int len = str.length() + 1;
  char* buf = new char[len];
  strcpy(buf, str.c_str());
  return buf;
}

void printLocalTime(){  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){ 
    MPRINT("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d \n%Y %H:%M:%S");  //Screen prints date and time.  屏幕打印日期和时间
}

void update_time(void){
    if((millis() - timeUpdated) > 1000){
        timeUpdated = millis();
        struct tm timeinfo;
        if(getLocalTime(&timeinfo)){ 
            currTime.h = timeinfo.tm_hour;
            currTime.m = timeinfo.tm_min;
            // MPRINT("Current time:")
            // MPRINT(String(currTime.h)+":"+String(currTime.m))
        }
    }
}

void alarm_init(void){
    appAlarm.flags.val = 0;
    appAlarm.trackName = "/track1.mp3";
    appAlarm.pos = 0;
    appAlarm.triggereTime = 0;
    appAlarm.alarmClook.h = 0;
    appAlarm.alarmClook.m = 0;
   
    M5.Axp.SetSpkEnable(true);   

    file = new AudioFileSourceSD("/");
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 0); // Output to builtInDAC
    out->SetPinout(12, 0, 2);
    out->SetOutputModeMono(true);
    out->SetGain((float)OUTPUT_GAIN/100.0);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out); 

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
}

void alarm_start(void){
    mp3->stop();
    delete file;
    delete out;
    delete mp3;
    mp3 = NULL;
    file = NULL;
    out = NULL;
    file = new AudioFileSourceSD(strToChar(appAlarm.trackName));
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 0); // Output to builtInDAC
    out->SetPinout(12, 0, 2);
    out->SetOutputModeMono(true);
    out->SetGain((float)OUTPUT_GAIN/100.0);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
}

void alarm_stop(void){
    mp3->stop();
}

uint8_t alarm_do_ring(void){
    if(((currTime.h == appAlarm.alarmClook.h) && (currTime.m == appAlarm.alarmClook.m) && !appAlarm.flags.triggered)){
        appAlarm.flags.triggered = 1;
        appAlarm.triggereTime = millis();
        MPRINT("Trigger alarm")
        return 1;
    }else if(appAlarm.flags.triggered){
        if((millis() - appAlarm.triggereTime) > 61000){
            appAlarm.flags.triggered = 0;
            appAlarm.triggereTime = millis();
        }
    }
    return 0;
}

void alarm_process(void){
    if(appAlarm.flags.set){
        if(alarm_do_ring()){
            appAlarm.flags.ring = 1;
        }
    }
    if(appAlarm.flags.ring){
        appAlarm.flags.ring = 0;
        appAlarm.flags.ringing = 1;
        alarm_start();
    }else if(appAlarm.flags.stop){
        appAlarm.flags.stop = 0;
        appAlarm.flags.ringing = 0;
        alarm_stop();
    }
    if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
    }else if (appAlarm.flags.ringing){
        alarm_start();
    }
    update_time();
}