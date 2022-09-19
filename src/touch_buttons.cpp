/**
 * @file touch_buttons.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@msemicon.com)
 * @brief Create an interface for the integrate touch buttons
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <M5Core2.h>
#include "touch_buttons.h"
#include "debug_utils.h"

extern app_t app;

void buttons_init(void){
    M5.BtnA.longPressTime = TOUCH_BUTTON_LONG_PRESS_DEBOUNCE_TIME_MS;
    M5.BtnB.longPressTime = TOUCH_BUTTON_LONG_PRESS_DEBOUNCE_TIME_MS;
    M5.BtnC.longPressTime = TOUCH_BUTTON_LONG_PRESS_DEBOUNCE_TIME_MS;

    M5.BtnA.addHandler(button_touch,E_TOUCH);
    M5.BtnB.addHandler(button_touch,E_TOUCH);
    M5.BtnC.addHandler(button_touch,E_TOUCH);

    M5.BtnA.addHandler(button_tap,E_PRESSED);
    M5.BtnB.addHandler(button_tap,E_PRESSED);
    M5.BtnC.addHandler(button_tap,E_PRESSED);

    M5.BtnA.addHandler(button_double_tap,E_DBLTAP);
    M5.BtnB.addHandler(button_double_tap,E_DBLTAP);
    M5.BtnC.addHandler(button_double_tap,E_DBLTAP);

    M5.BtnA.addHandler(button_long_pressed,E_LONGPRESSING);
    M5.BtnB.addHandler(button_long_pressed,E_LONGPRESSING);
    M5.BtnC.addHandler(button_long_pressed,E_LONGPRESSING);
}

void buttons_monitor(void){
    M5.update();
}

void button_tap(Event& e){
    if(e.button->getName() == M5.BtnA.getName()){
        MPRINT("Button A - Single Tap");
        app.alarm.flags.ring = 1;
    }else if(e.button->getName() == M5.BtnB.getName()){
        MPRINT("Button B - Single Tap");
        if(!app.displayState){
            app.flags.setDisplayOn = 1;
        }
    }else if(e.button->getName() == M5.BtnC.getName()){
        MPRINT("Button C - Single Tap");
        app.alarm.flags.snooze = 0;
    }
}

void button_double_tap(Event& e){
    button_haptics(150);
    if(e.button->getName() == M5.BtnA.getName()){
        MPRINT("Button A - Double Tap");
    }else if(e.button->getName() == M5.BtnB.getName()){
        MPRINT("Button B - Double Tap");
        if(app.displayState){
            MPRINT("Request to turn off display");
            app.flags.setDisplayOff = 1;
            app.flags.setDisplayOn = 0;

        }
    }else if(e.button->getName() == M5.BtnC.getName()){
        MPRINT("Button C - Double Tap");
    }
}

void button_long_pressed(Event& e){
    button_haptics(200);
    if(e.button->getName() == M5.BtnA.getName()){
        MPRINT("Button A - Long Tap");
    }else if(e.button->getName() == M5.BtnB.getName()){
        MPRINT("Button B - Long Tap");
        app.alarm.flags.stop = 1;
    }else if(e.button->getName() == M5.BtnC.getName()){
        MPRINT("Button C - Long Tap");
    }
}

void button_touch(Event& e){
    button_haptics(100);
    if(e.button->getName() == M5.BtnA.getName()){
        MPRINT("Button A - Touch Tap");
    }else if(e.button->getName() == M5.BtnB.getName()){
        MPRINT("Button B - Touch Tap");
    }else if(e.button->getName() == M5.BtnC.getName()){
        MPRINT("Button C - Touch Tap");
    }
}

void button_haptics(uint16_t time_ms){
    if(time_ms > 250) time_ms = 250;
    M5.Axp.SetLDOEnable(3,true);
    delay(time_ms);
    M5.Axp.SetLDOEnable(3,false);
}