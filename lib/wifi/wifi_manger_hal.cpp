/**
 * @file wifi_manger_hal.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "wifi_manger_hal.h"

bool wifi_hal_handle_connect(const char* ap_ssid, const char* ap_psk){
    WiFiManager manager;
    manager.setConnectTimeout(CONN_TIMEOUT);
    return manager.autoConnect(ap_ssid, ap_psk);
}

online_state_t wifi_check_connection_status(void){
    //TODO 
    delay(1);
}