/**
 * @file haptics.cpp
 * @author Giovanni Abbruzzo
 * @brief 
 * @version 0.1
 * @date 2022-02-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "haptics.h"
#include "defs.h"
#include "includes.h"

extern app_t app;

void haptics_process(void){
    if(app.flags.haptFlag){
        if(app.hapticState){
            if((millis() - app.timers.hapticTriggered) > HAPTIC_SHORT_TIME){
                M5.Axp.SetLDOEnable(3,false);
                app.flags.haptFlag = 0;
                app.hapticState = 0;
            }                    
        }else{
            M5.Axp.SetLDOEnable(3,true);  //Open the vibration
            app.hapticState = 1;
        }
    }
}