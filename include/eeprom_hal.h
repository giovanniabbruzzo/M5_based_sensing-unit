#ifndef EEPROM_HAL_H
#define EEPROM_HAL_H

#define EEPROM_SIZE 3
#define EEPROM_ALARM_SET_ADDR 0 // Contains whether the alarm is set or no
#define EEPROM_ALARM_H_ADDR 1 // Contains the hour when the alarm has to be triggered
#define EEPROM_ALARM_M_ADDR 2 // Contains the minutes when the alarm has to be triggered

void eeprom_init(void);
void eeprom_store_alarm_data(void);
void eeprom_recover_alarm_data(void);


#endif // EEPROM_HAL_H