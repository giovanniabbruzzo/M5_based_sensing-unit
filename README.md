# M5_based_sensing-unit and alarm clock
Sensing unit based on the M5 Stack core2 monitoring air quality with the BME68

Connect the BME68 sensor to the I2C to start reading the values straight away.

You can use the WiFi manager to connect to the WiFi and then use the addresss to updload new code via OTA.

Creates a local web server where it's possible to see the data.

Connects to blynk cloud to show the data. From Blynk is now possible to set an alarm.

Changed GUI on the display to visualize the data and current time.
![GUI img](/images/gui.jpg "GUI")

Possible to be used as an alarm clock if alarm set on the cloud.

RTC now configured and synced with the cloud at midnight