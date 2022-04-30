/**
 * @file display.h
 * @author Giovanni Abbruzzo
 * @brief 
 * @version 0.1
 * @date 2022-02-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <M5Core2.h>
#include "defs.h"

#define TXT_COLOR LIGHTGREY
// Used for the visualization of the gas data
#define BG_COLOR1 DARKGREEN
// Used for the visualization of the accelerometer data
#define BG_COLOR2 BLUE
// Used for the visualization of the buttons
#define BG_COLOR3 WHITE

// Color of the clock display
#define BG_CLOCK BLACK

#define TXT_FONT_SIZE 2

void display_init(void);
void display_clear(void);
void display_println(const char *s);
void display_process(void);
void display_monitor(void);