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

extern app_t app;

#define OUTPUT_GAIN 300

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

unsigned long int timeUpdated = millis();

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

uint8_t setupTimeRTC(){
    struct tm timeinfo;
    if(getLocalTime(&timeinfo)){ 
        app.clock.h = timeinfo.tm_hour;
        app.clock.m = timeinfo.tm_min;
        app.clock.mDay = timeinfo.tm_mday;
        app.clock.wDay = timeinfo.tm_wday;
        app.clock.month = timeinfo.tm_mon + 1;
        app.clock.year = 1900+timeinfo.tm_year;
        // MPRINT("Current time:")
        // MPRINT(String(app.clock.h)+":"+String(app.clock.m))
        RTCtime.Hours = app.clock.h; //Set the time
        RTCtime.Minutes = app.clock.m;
        RTCtime.Seconds = timeinfo.tm_sec;
        M5.Rtc.SetTime(&RTCtime); //and writes the set time to the real time clock.  并将设置的时间写入实时时钟

        RTCDate.Year = app.clock.year;  
        RTCDate.Month = app.clock.month;
        RTCDate.Date = app.clock.mDay;
        RTCDate.WeekDay = app.clock.wDay;
        M5.Rtc.SetDate(&RTCDate);
        return 1;
    }
    return 0;
}

void flushTime(){
    M5.Rtc.GetTime(&RTCtime); 
    M5.Rtc.GetDate(&RTCDate);
}

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
        // Now we take the time from the RTC
        flushTime();

        app.clock.h = RTCtime.Hours;
        app.clock.m = RTCtime.Minutes;
        app.clock.mDay = RTCDate.Date;
        app.clock.wDay = RTCDate.WeekDay;
        app.clock.month = RTCDate.Month;
        app.clock.year = RTCDate.Year;

        if(RTCtime.Seconds == 0){
            app.flags.updateDisplay = 1;
        }
    }
}

void getStringDate(void){
    switch (app.clock.wDay) {
        case 1:
            app.clock.dateString = "Mon, ";
            break;
        case 2:
            app.clock.dateString = "Tue, ";
            break;
        case 3:
            app.clock.dateString = "Wed, ";
            break;
        case 4:
            app.clock.dateString = "Thu, ";
            break;
        case 5:
            app.clock.dateString = "Fri, ";
            break;
        case 6:
            app.clock.dateString = "Sat, ";
            break;
        case 7:
            app.clock.dateString = "Sun, ";
            break;        
        default:
            app.clock.dateString = "Sun, ";
            break;
    }
    if(app.clock.mDay < 10){
        app.clock.dateString += "0"+String(app.clock.mDay)+"-";
    }else{
        app.clock.dateString += String(app.clock.mDay)+"-";
    }
    String mmth;
    switch(app.clock.month){
        case 1:
            app.clock.dateString += "Jan";
            break;
        case 2:
            app.clock.dateString += "Feb";
            break;
        case 3:
            app.clock.dateString += "Mar";
            break;
        case 4:
            app.clock.dateString += "Apr";
            break;
        case 5:
            app.clock.dateString += "May";
            break;
        case 6:
            app.clock.dateString += "Jun";
            break;
        case 7:
            app.clock.dateString += "Jul";
            break;
        case 8:
            app.clock.dateString += "Aug";
            break;
        case 9:
            app.clock.dateString += "Sep";
            break;
        case 10:
            app.clock.dateString += "Oct";
            break;
        case 11:
            app.clock.dateString += "Nov";
            break;
        case 12:
            app.clock.dateString += "Dec";
            break;
        default:
            app.clock.dateString += "Jan";
            break;
    }
    app.clock.dateString += "-"+String(app.clock.year);
}

void getStringTime(void){
    if(app.clock.h < 10){
        app.clock.timeString = "0"+String(app.clock.h)+":";
    }else{
        app.clock.timeString = String(app.clock.h)+":";
    }
    if(app.clock.m < 10){
        app.clock.timeString += "0"+String(app.clock.m);
    }else{
        app.clock.timeString += String(app.clock.m);
    }
}

void alarm_init(void){
    app.alarm.flags.val = 0;
    app.alarm.trackName = "/track1.mp3";
    app.alarm.pos = 0;
    app.alarm.triggereTime = 0;
    app.alarm.alarmClock.h = 0;
    app.alarm.alarmClock.m = 0;
   
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
    setupTimeRTC();
}

void alarm_start(void){
    mp3->stop();
    delete file;
    delete out;
    delete mp3;
    mp3 = NULL;
    file = NULL;
    out = NULL;
    file = new AudioFileSourceSD(strToChar(app.alarm.trackName));
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
    if(((app.clock.h == app.alarm.alarmClock.h) && (app.clock.m == app.alarm.alarmClock.m) && !app.alarm.flags.triggered)){
        app.alarm.flags.triggered = 1;
        app.alarm.triggereTime = millis();
        MPRINT("Trigger alarm")
        return 1;
    }else if(app.alarm.flags.triggered){
        if((millis() - app.alarm.triggereTime) > 61000){
            app.alarm.flags.triggered = 0;
            app.alarm.triggereTime = millis();
        }
    }
    return 0;
}

void alarm_process(void){
    if(app.alarm.flags.set){
        if(alarm_do_ring()){
            app.alarm.flags.ring = 1;
        }
    }
    if(app.alarm.flags.ring){
        app.alarm.flags.ring = 0;
        app.alarm.flags.ringing = 1;
        alarm_start();
    }else if(app.alarm.flags.stop){
        app.alarm.flags.stop = 0;
        app.alarm.flags.ringing = 0;
        alarm_stop();
    }
    if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
    }else if (app.alarm.flags.ringing){
        alarm_start();
    }
    update_time();
}