/**
 * @file ota_updates_hal.cpp
 * @author Giovanni Abbruzzo (giovanni.abbruzzo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "ota_updates_hal.h"
#include <HTTPUpdate.h>

String ota_get_chip_id(void){
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}

void ota_do_update(void){
    String url = "http://otadrive.com/deviceapi/update?";
    url += MakeFirmwareInfo(ProductKey, Version);
    url += "&s=" + ota_get_chip_id();

    WiFiClient client;
    HTTPUpdateResult ret = httpUpdate.update(client, url, Version);

    switch (ret){
    case HTTP_UPDATE_FAILED:
        PRINT("Update failed");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        PRINT("No updates available");
        break;
    case HTTP_UPDATE_OK:
        PRINT("Update went ok");
        break;
    
    default:
        break;
    }    
}