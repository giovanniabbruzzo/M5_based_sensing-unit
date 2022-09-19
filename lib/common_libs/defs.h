/**
 * @file defs.h
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief It will include all the definitions and inclusions
 *        related to the scecific app
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>

// Defines
#define OTA
// #define MATH

#define FIRMWARE_VERSION "1.0.0.0"
#define OTA_PRODUCT_KEY ""

// Uncomment if you don't want the print functionalities
#define SERIAL_DEBUGS

// Difines the periods for every timer, all in ms
#ifdef OTA
#define OTA_UPDATE_POLL_PERIOD_MS 125
#endif
#define BME_POLLING_PERIOD_MS 60*1000

#define TOUCH_BUTTON_LONG_PRESS_DEBOUNCE_TIME_MS 3000

#define DISPLAY_OFF_TIMEOUT_TIME_MS 5*60*1000

// Define all the enums structures
typedef enum{
	OFFLINE,
	ONLINE
}online_state_t;

// Add here all the timers and then init then in timer_init() func
struct timers_t {
#ifdef OTA
	volatile uint64_t OTAUpdateTimer;
#endif
	volatile uint64_t generalTestTimer;
	volatile uint64_t bmeReadingTimer;
	volatile uint64_t displayOffTimer;
};

typedef union {
    uint8_t val;
    struct {
        uint8_t updateDisplay : 1;
		uint8_t updateBlynk :1;
		uint8_t launchOTA : 1;
		uint8_t setDisplayOff : 1;
		uint8_t setDisplayOn : 1;
		uint8_t readBME : 1;
		uint8_t haptFlag : 1;
		uint8_t displayAutoTurnOffFlag : 1;
    };
} app_ctx_flags_t;

struct air_quality_t{
	float temp;
	uint32_t press;
	float hum;
	uint32_t gas_res;
	float alt;
};

typedef union {
    uint8_t val;
    struct {
        uint8_t ring : 2;
		uint8_t ringing :1;
		uint8_t snooze : 1;
		uint8_t stop : 1;
		uint8_t set : 1;
		uint8_t triggered : 1;
    };
} alarm_flags_t;

struct date_time_t{
	int h;
	int m;
	int mDay;
	int year;
	int month;
	int wDay;
	String dateString;
	String timeString;
};

struct alarm_t{
	alarm_flags_t flags;
	String trackName;
	int pos;
	date_time_t alarmClock;
	unsigned long triggereTime;
};

struct app_t {
	// WiFi information
	uint8_t WiFiOnline;
	String WiFiMacAddress;
	String WiFiLocalIP;
	// Timers
	timers_t timers;
	// Air quality sensor
	air_quality_t aq;
	// Display
	volatile uint8_t displayState;
	// Flags
	volatile app_ctx_flags_t flags;
	alarm_t alarm;
	date_time_t clock;
};

// All the extern variables go here
extern app_t app;

#endif // DEFS_H