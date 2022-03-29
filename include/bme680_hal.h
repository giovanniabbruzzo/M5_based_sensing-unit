/**
 * @file bme680_hal.h
 * @author Giovanni Abbruzzo
 * @brief 
 * @version 0.1
 * @date 2022-02-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_I2C_SDA 21
#define BME_I2C_SCL 22
#define BME_I2C_FREQ 400000L

#define SEALEVELPRESSURE_HPA (1013.25)

#define TEMP_OFFSET -3.5f
#define HUM_OFFSET 6.5f

void bme_init(void);
void bme_init_reading(void);
void bme_read_data(void);