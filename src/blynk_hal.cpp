/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/
#include "blynk_hal.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "defs.h"
#include "debug_utils.h"

extern app_t app;

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timerBlynk;

void mytimerBlynkEvent(void);

// // This function is called every time the Virtual Pin 0 state changes
// BLYNK_WRITE(V0){
//   // Set incoming value from pin V0 to a variable
//   int value = param.asInt();
//   if (value){
//     app.flags.setDisplayOn = 1;
//   }else{
//     app.flags.setDisplayOff = 1;
//   }

//   // Update state
//   Blynk.virtualWrite(V1, value);
// }

// This function is called every time the device is connected to the Blynk.Cloud
// BLYNK_CONNECTED(){
//   // Change Web Link Button message to "Congratulations!"
//   Blynk.setProperty(V4, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
//   Blynk.setProperty(V4, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
//   Blynk.setProperty(V4, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
// }

// This function sends Arduino's uptime every second to Virtual Pin 2.
void mytimerBlynkEvent(void){
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, millis() / 1000);
}

void blynk_init(char* ssid, char* pass){
  Blynk.begin(auth, ssid, pass);

  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timerBlynk.setInterval(1000L, mytimerBlynkEvent);
}

void blynk_loop(void){
  Blynk.run();
  timerBlynk.run();
  if(app.flags.updateBlynk){
    MPRINT("Sending data to Blynk...")
    app.flags.updateBlynk = 0;
    Blynk.virtualWrite(V0, app.aq.temp);
    Blynk.virtualWrite(V1, app.aq.hum);
    Blynk.virtualWrite(V2, app.aq.gas_res);
  }
}
