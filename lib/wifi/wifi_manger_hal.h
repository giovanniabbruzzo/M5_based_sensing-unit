/**
 * @file wifi_manger_hal.h
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <WiFiManager.h>
#include "defs.h"

#define CONN_TIMEOUT 30 // Timeout in seconds

bool wifi_hal_handle_connect(const char* ap_ssid, const char* ap_psk);
online_state_t wifi_check_connection_status(void);