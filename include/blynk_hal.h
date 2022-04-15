#ifndef BLYNK_HAL_H
#define BLYNK_HAL_H

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLWNpxpR_A"
#define BLYNK_DEVICE_NAME "Home Environment Monitor"
#define BLYNK_AUTH_TOKEN "iCu7d_pmIkMd1RYQLFCH-SJgRnh5nNc2"

#define BLYNK_PRINT Serial

void blynk_init(char* ssid, char* pass);
void blynk_loop(void);

#endif // BLYNK_HAL_H