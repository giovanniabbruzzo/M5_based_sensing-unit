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

        disp.setCursor(20,20);
        disp.print("IP: ");
        disp.println(app.WiFiLocalIP);

        disp.setCursor(20,50);
        disp.print("Temp: ");
        disp.print(app.aq.temp);
        disp.println(" *C");

        disp.setCursor(20,80);
        disp.print("Hum: ");
        disp.print(app.aq.hum);
        disp.println(" %");

        disp.setCursor(20,110);
        disp.print("Gas: ");
        disp.print(app.aq.gas_res);
        disp.println(" KOhms");

        disp.setCursor(20,140);
        disp.print("Press: ");
        disp.print(app.aq.press);
        disp.println(" hPa");

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
        M5.Lcd.writecommand(ILI9341_DISPON);
        M5.Lcd.setBrightness(11);
        app.displayState = 1;
        app.timers.displayOffTimer = DISPLAY_OFF_TIMEOUT_TIME_MS;
    }
    if(app.flags.setDisplayOff){
        app.flags.setDisplayOff = 0;
        M5.Lcd.writecommand(ILI9341_DISPOFF);
        M5.Lcd.setBrightness(0);
        app.displayState = 0;
    }
}