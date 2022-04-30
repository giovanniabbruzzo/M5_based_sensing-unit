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

#define OUTPUT_GAIN 300

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

alarm_t appAlarm;
// Udate current time every 1 second
date_time_t currDateTime;
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
            currDateTime.h = timeinfo.tm_hour;
            currDateTime.m = timeinfo.tm_min;
            currDateTime.mDay = timeinfo.tm_mday;
            currDateTime.wDay = timeinfo.tm_wday;
            currDateTime.month = timeinfo.tm_mon + 1;
            currDateTime.year = 1900+timeinfo.tm_year;
            // MPRINT("Current time:")
            // MPRINT(String(currDateTime.h)+":"+String(currDateTime.m))
        }
    }
}

void getStringDate(void){
    switch (currDateTime.wDay) {
        case 1:
            currDateTime.dateString = "Mon, ";
            break;
        case 2:
            currDateTime.dateString = "Tue, ";
            break;
        case 3:
            currDateTime.dateString = "Wed, ";
            break;
        case 4:
            currDateTime.dateString = "Thu, ";
            break;
        case 5:
            currDateTime.dateString = "Fri, ";
            break;
        case 6:
            currDateTime.dateString = "Sat, ";
            break;
        case 7:
            currDateTime.dateString = "Sun, ";
            break;        
        default:
            currDateTime.dateString = "Sun, ";
            break;
    }
    if(currDateTime.mDay < 10){
        currDateTime.dateString += "0"+String(currDateTime.mDay)+"-";
    }else{
        currDateTime.dateString += String(currDateTime.mDay)+"-";
    }
    String mmth;
    switch(currDateTime.month){
        case 1:
            currDateTime.dateString += "Jan";
            break;
        case 2:
            currDateTime.dateString += "Feb";
            break;
        case 3:
            currDateTime.dateString += "Mar";
            break;
        case 4:
            currDateTime.dateString += "Apr";
            break;
        case 5:
            currDateTime.dateString += "May";
            break;
        case 6:
            currDateTime.dateString += "Jun";
            break;
        case 7:
            currDateTime.dateString += "Jul";
            break;
        case 8:
            currDateTime.dateString += "Aug";
            break;
        case 9:
            currDateTime.dateString += "Sep";
            break;
        case 10:
            currDateTime.dateString += "Oct";
            break;
        case 11:
            currDateTime.dateString += "Nov";
            break;
        case 12:
            currDateTime.dateString += "Dec";
            break;
        default:
            currDateTime.dateString += "Jan";
            break;
    }
    currDateTime.dateString += "-"+String(currDateTime.year);
}

void getStringTime(void){
    if(currDateTime.h < 10){
        currDateTime.timeString = "0"+String(currDateTime.h)+":";
    }else{
        currDateTime.timeString = String(currDateTime.h)+":";
    }
    if(currDateTime.m < 10){
        currDateTime.timeString += "0"+String(currDateTime.m);
    }else{
        currDateTime.timeString += String(currDateTime.m);
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
    if(((currDateTime.h == appAlarm.alarmClook.h) && (currDateTime.m == appAlarm.alarmClook.m) && !appAlarm.flags.triggered)){
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