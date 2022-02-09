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
        disp.setCursor(0,0);
        // Update
        disp.printf(
            "Temp: %1.2f *C\nPres: %u hPa\nHum: %1.2f %\nGas res: %u KOhms\nAltitude: %1.2f m\n",
            app.aq.temp,
            app.aq.press,
            app.aq.hum,
            app.aq.gas_res,
            app.aq.alt
            );
        disp.print("IP addr: ");
        disp.println(app.WiFiLocalIP);
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