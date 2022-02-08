/**
 * @file debug_utils.h
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief Define SERIAL_DEBUG in includes.h. 
 *        Use:
 *          - DEBUG_PRINT, when you need debugging information such as
 *            line, file name, numer
 *          - MPRINT when you need to print standard println()
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include "defs.h"

#ifdef SERIAL_DEBUGS
#define SERIAL_INIT Serial.begin(115200);
#define DEBUG_PRINT(str)    \
    Serial.print(millis());     \
    Serial.print(": ");    \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' ');      \
    Serial.print(__FILE__);     \
    Serial.print(':');      \
    Serial.print(__LINE__);     \
    Serial.print(' ');      \
    Serial.print("\"");      \
    Serial.print(str);    \
    Serial.println("\"");
#define MPRINT(x) Serial.println(x);
#else
#define SERIAL_INIT 
#define DEBUG_PRINT(str) 
#define MPRINT(x)
#endif

#endif