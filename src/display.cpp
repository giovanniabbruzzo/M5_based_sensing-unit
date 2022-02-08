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
    if(app.flags.updateDisplay){
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

