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
#include "touch_buttons.h"
#include "debug_utils.h"
#include "includes.h"

touch_buttons_t bttns;
extern alarm_t appAlarm;

void buttons_init(void){
    bttns.state = TB_RELEASED;
    bttns.process = 0;
    bttns.pressedTime = 0;
}

void buttons_monitor(void){
    if(M5.BtnB.wasPressed()) {
        switch (bttns.state){
            case TB_RELEASED:
                bttns.state = TB_ONE_TAP;
                bttns.pressedTime = millis();
                if(!app.displayState){
                    app.flags.setDisplayOn = 1;
                }
                // Enable haptic
                app.timers.hapticTriggered = millis();
                app.flags.haptFlag = 1;
                DEBUG_PRINT("One tap")
                break;
            case TB_ONE_TAP:
                if((millis() - bttns.pressedTime) <= TOUCH_BUTOON_DOUBLE_TAP_DEBOUNCE_TIME_MS){
                    bttns.state = TB_DOUBLE_TAP;
                    DEBUG_PRINT("Double tap detected")
                    // Enable haptic
                    app.timers.hapticTriggered = millis();
                    app.flags.haptFlag = 1;
                }
                else{
                    bttns.state = TB_RELEASED;
                    bttns.pressedTime = 0;
                    DEBUG_PRINT("Double tap ignored")
                }
                break;
            case TB_DOUBLE_TAP:
                DEBUG_PRINT("Triple tap ignored")
                bttns.state = TB_RELEASED;
                bttns.pressedTime = 0;
                break;
            case TB_LONG_PRESSED:
                bttns.state = TB_RELEASED;
                bttns.pressedTime = 0;
                break;
            default:
                DEBUG_PRINT("Error")
                break;
        }
    }
    else if(M5.BtnB.isReleased()){
        if(bttns.state != TB_RELEASED){
            if((millis() - bttns.pressedTime) > TOUCH_BUTOON_DOUBLE_TAP_DEBOUNCE_TIME_MS){
                bttns.state = TB_RELEASED;
                bttns.pressedTime = 0;
            }   
        }
    }
    else{
        switch (bttns.state){
            case TB_RELEASED:
                break;
            case TB_ONE_TAP:
                if((millis() - bttns.pressedTime) > TOUCH_BUTOON_RELEASE_DEBOUNCE_TIME_MS){
                    bttns.state = TB_LONG_PRESSED;
                }
                break;
            case TB_DOUBLE_TAP:
                break;
            case TB_LONG_PRESSED:
                if(M5.BtnB.pressedFor(TOUCH_BUTOON_LONG_PRESS_DEBOUNCE_TIME_MS)){
                    DEBUG_PRINT("Long tap detected")
                    bttns.process = 1;
                    bttns.state = TB_RELEASED;
                    bttns.pressedTime = 0;
                    if(app.displayState){
                        app.flags.setDisplayOff = 1;
                    }
                    // Enable haptic
                    app.timers.hapticTriggered = millis();
                    app.flags.haptFlag = 1;
                }
                break;
            default:
                DEBUG_PRINT("Error")
                break;
        }
    }
    if (M5.BtnA.wasPressed()){
        MPRINT("Trigger alarm")
        appAlarm.flags.ring = 1;
    }
    if (M5.BtnC.wasPressed()){
        MPRINT("Snooze alarm")
        appAlarm.flags.snooze = 0;
    }        
}

void buttons_process(void){
    if(bttns.process){
        bttns.process = 0;
        DEBUG_PRINT("Long press button recorded")
        appAlarm.flags.stop = 1;
    }
}