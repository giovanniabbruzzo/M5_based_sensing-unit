/**
 * @file bme680_hal.cpp
 * @author Giovanni Abbruzzo
 * @brief Abstraction Layer for the BME680 Air Quality sensor
 * @version 0.1
 * @date 2022-02-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "bme680_hal.h"
#include "debug_utils.h"
#include <M5Core2.h>

TwoWire I2CBME = TwoWire(1);
Adafruit_BME680 bme;

/**
 * @brief Initialization of the BME680 sensor
 * 
 */
void bme_init(void){
    I2CBME.begin(BME_I2C_SDA, BME_I2C_SCL, BME_I2C_FREQ);
    if (!bme.begin(0x76,&I2CBME)){
        DEBUG_PRINT("Could not find a valid BME680 sensor, check wiring!")
    }
    
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

    bme_init_reading();

    
}

/**
 * @brief Initiate the reading from the gas sensor, it takes
 *        some time to get to the proper heat temperature.
 *        It MUST be called before bme_read_data().
 */
void bme_init_reading(void){
    // Tell BME680 to begin measurement.
    unsigned long endTime = bme.beginReading();
    if (endTime == 0) {
        Serial.println(F("Failed to begin reading :("));
        return;
    }

    MPRINT("Reading started at "+String(millis())+" and will finish at "+String(endTime))
}

/**
 * @brief Main process for reading the BME sensor
 * 
 */
void bme_read_data(void){
    if (!bme.endReading()) {
        MPRINT("Failed to complete reading :(")
        return;
    }

    app.aq.temp = bme.temperature + TEMP_OFFSET;
    app.aq.press = bme.pressure / 100.0;
    app.aq.hum = bme.humidity + HUM_OFFSET;
    app.aq.gas_res = bme.gas_resistance / 1000.0;
    app.aq.alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

    app.flags.updateDisplay = 1;

    MPRINT("Temperature = "+String(app.aq.temp)+" *C")
    MPRINT("Pressure = "+String(app.aq.press)+" hPa")
    MPRINT("Humidity = "+String(app.aq.hum)+" %")
    MPRINT("Gas = "+String(app.aq.gas_res)+" KOhms")
    MPRINT("Approx. Altitude = "+String(app.aq.alt)+" m")

}