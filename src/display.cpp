/**
 * @file display.cpp
 * @author Giovanni Abbruzzo
 * @brief 
 * @version 0.1
 * @date 2022-02-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "display.h"
#include "debug_utils.h"
#include "alarm_hal.h"

extern date_time_t currDateTime;

TFT_eSprite disp(&M5.Lcd);

/**
 * @brief Init display
 * 
 */
void display_init(void){
    disp.createSprite(320, 240);
    disp.fillSprite(BLACK);
    disp.setTextColor(TXT_COLOR);
    disp.setTextSize(TXT_FONT_SIZE);
    disp.setCursor(0, 0);
    disp.pushSprite(0, 0);

    MPRINT("Display width: "+String(disp.width())+", display height: "+String(disp.height()))
}

/**
 * @brief Clear the display
 * 
 */
void display_clear(void){
    disp.fillRect(0,0,disp.width(),disp.height(),BG_COLOR1);
    disp.setCursor(0,0);
    disp.pushSprite(0, 0);
}

void display_println(const char *s){
    // Clear
    disp.fillRect(0,0,disp.width(),disp.height(),BG_COLOR1);
    disp.setCursor(0,0);
    // Update
    disp.println(s);
    disp.pushSprite(0, 0);
}

/**
 * @brief Process the display data if flag set
 * 
 */
void display_process(void){
    if(app.flags.updateDisplay && app.displayState){
        MPRINT("Processing display...")
        app.flags.updateDisplay = 0;
        // Clear
        disp.fillRect(0,0,disp.width(),disp.height(),BG_COLOR1);
        // Design the UI
        // Sensor data
        disp.fillRect(10,10,80,70,BG_CLOCK);
        disp.fillRect(120,10,80,70,BG_CLOCK);
        disp.fillRect(230,10,80,70,BG_CLOCK);
        // Clock data
        disp.fillRect(10,90,(disp.width()-20),(disp.height()-100),BG_CLOCK);

        // Update data
        disp.setTextSize(TXT_FONT_SIZE);
        disp.setCursor(15,15);
        disp.print("Temp: ");
        disp.setCursor(15,35);
        disp.print(app.aq.temp);
        disp.setCursor(15,55);
        disp.println("*C");

        disp.setCursor(125,15);
        disp.print("Hum: ");
        disp.setCursor(125,35);
        disp.print(app.aq.hum);
        disp.setCursor(125,55);
        disp.println("%");

        disp.setCursor(235,15);
        disp.print("Gas: ");
        disp.setCursor(235,35);
        disp.print(app.aq.gas_res);
        disp.setCursor(235,55);
        disp.println("KOhms");

        // Update clock
        getStringDate();
        getStringTime();
        disp.setCursor(20,100);
        disp.print(currDateTime.dateString);
        disp.setCursor(20,125);
        if(appAlarm.flags.set){
            String alarmTime;
            if(appAlarm.alarmClook.h < 10){
                alarmTime = "0"+String(appAlarm.alarmClook.h)+":";
            }else{
                alarmTime = String(appAlarm.alarmClook.h)+":";
            }
            if(appAlarm.alarmClook.m < 10){
                alarmTime += "0"+String(appAlarm.alarmClook.m);
            }else{
                alarmTime += String(appAlarm.alarmClook.m);
            }
            disp.printf("Alarm set @ %s",alarmTime);
        }else{
            disp.print("Alarm not set!");
        }
        disp.setTextSize(7);
        disp.setCursor(60,165);
        disp.print(currDateTime.timeString);

        disp.pushSprite(0, 0);
    }
}

/**
 * @brief Monitor the display to see if it needs to be turned on or off
 * 
 */
void display_monitor(void){
    if(app.flags.setDisplayOn){
        app.flags.setDisplayOn = 0;
        M5.Lcd.wakeup();
        M5.Lcd.setBrightness(20);
        app.displayState = 1;
        app.timers.displayOffTimer = DISPLAY_OFF_TIMEOUT_TIME_MS;
    }
    if(app.flags.setDisplayOff){
        app.flags.setDisplayOff = 0;
        M5.Lcd.sleep();
        M5.Lcd.setBrightness(0);
        app.displayState = 0;
    }
}