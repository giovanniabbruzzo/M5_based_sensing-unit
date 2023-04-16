#include "eeprom_hal.h"
#include <EEPROM.h>
#include "defs.h"

extern app_t app;

/**
 * @brief Initialize the eeprom  memory
 */
void eeprom_init(void){
    EEPROM.begin(EEPROM_SIZE);
}

void eeprom_store_alarm_data(void){
    PRINT("Storing alarm data")
    EEPROM.write(EEPROM_ALARM_SET_ADDR, app.alarm.flags.set);
    EEPROM.write(EEPROM_ALARM_H_ADDR, app.alarm.alarmClock.h);
    EEPROM.write(EEPROM_ALARM_M_ADDR, app.alarm.alarmClock.m);
    EEPROM.commit();
    PRINT("app.alarm.flags.set: "+String(app.alarm.flags.set))
    PRINT("app.alarm.alarmClock.h: "+String(app.alarm.alarmClock.h))
    PRINT("app.alarm.alarmClock.m: "+String(app.alarm.alarmClock.m))
    delay(1);
}

void eeprom_recover_alarm_data(void){
    PRINT("Recovering alarm data")
    app.alarm.flags.set = EEPROM.read(EEPROM_ALARM_SET_ADDR);
    app.alarm.alarmClock.h = EEPROM.read(EEPROM_ALARM_H_ADDR);
    app.alarm.alarmClock.m = EEPROM.read(EEPROM_ALARM_M_ADDR);
    PRINT("app.alarm.flags.set: "+String(app.alarm.flags.set))
    PRINT("app.alarm.alarmClock.h: "+String(app.alarm.alarmClock.h))
    PRINT("app.alarm.alarmClock.m: "+String(app.alarm.alarmClock.m))
}