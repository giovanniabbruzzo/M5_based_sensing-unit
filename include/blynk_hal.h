#ifndef BLYNK_HAL_H
#define BLYNK_HAL_H

#include "secrets.h"
#include "debug_utils.h"

// // Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// // See the Device Info tab, or Template settings
// #define BLYNK_TEMPLATE_ID "TMPLWNpxpR_A"
// #define BLYNK_DEVICE_NAME "Home Environment Monitor"
// #define BLYNK_AUTH_TOKEN "iCu7d_pmIkMd1RYQLFCH-SJgRnh5nNc2"

#ifdef SERIAL_DEBUGS
#define BLYNK_PRINT Serial
#endif

void blynk_init(void);
void blynk_loop(void);

#endif // BLYNK_HAL_H