/*
 * dlvr.c
 *
 *  Created on: May 20, 2023
 *      Author: Danylo Ulianych
 */

#include "driver/i2c.h"

#include "dlvr.h"

#define I2C_NO_TIMEOUT          (0)

#define DLVR_STATUS_OK          (0b00)
#define DLVR_STATUS_RESERVED    (0b01)
#define DLVR_STATUS_TIMEOUT     (0b10)
#define DLVR_STATUS_ERR         (0b11)

#define DLVR_MAX_PRESSURE_H2O    (0.5f)

#define DLVR_FULL_SCALE_REF      (1 << 14)
#define DLVR_PRESSURE_ZERO_REF   (8192)

#define DLVR_H2O_TO_PA_COEF      (248.84f)
#define DLVR_RAW_TO_PA(x)        (DLVR_H2O_TO_PA_COEF * 1.25f * (((float)x - DLVR_PRESSURE_ZERO_REF) / DLVR_FULL_SCALE_REF) * 2 * DLVR_MAX_PRESSURE_H2O)

#define DLVR_CONVERT_TEMPERATURE(x)  ((float)x * (200.f / 2047.f) - 50.f)

static i2c_port_t m_port = 0;


void dlvr_init(i2c_port_t i2c_port) {
    m_port = i2c_port;
}


esp_err_t dlvr_read_pressure_sdp(int16_t* diff_pressure_sdp) {
    uint16_t dp_raw;
    esp_err_t err = dlvr_read_pressure_raw(&dp_raw);
    if (err == ESP_OK) {
        float dp_pa = DLVR_RAW_TO_PA(dp_raw);
        *diff_pressure_sdp = (int16_t) (DLVR_SDP_PRESSURE_SCALE * dp_pa);
    }
    return err;
}


esp_err_t dlvr_read_pressure_raw(uint16_t* diff_pressure_raw) {
    uint8_t data[2] = { 0 };
    esp_err_t err = i2c_master_read_from_device(m_port, DLVR_I2C_ADDR, data, sizeof(data), I2C_NO_TIMEOUT);
    if (err == ESP_OK) {
        *diff_pressure_raw = ((uint16_t)(data[0] & 0b00111111) << 8) | (uint16_t)(data[1]);
        uint8_t status = (data[0] & 0b11000000) >> 6;
        switch (status) {
        case DLVR_STATUS_OK:
            err = ESP_OK;
            break;
        case DLVR_STATUS_RESERVED:
            err = ESP_FAIL;
            break;
        case DLVR_STATUS_TIMEOUT:
            err = ESP_ERR_TIMEOUT;
            break;
        case DLVR_STATUS_ERR:
            err = ESP_FAIL;
            break;
        default:
            err = ESP_FAIL;
            break;
        }
    }

    return err;
}


esp_err_t dlvr_read_pressure_temperature(float* diff_pressure, float* temperature) {
    uint8_t data[4] = { 0 };
    esp_err_t err = i2c_master_read_from_device(m_port, DLVR_I2C_ADDR, data, sizeof(data), pdMS_TO_TICKS(1000));
    if (err == ESP_OK) {
        uint16_t dp_raw = ((uint16_t)(data[0] & 0b00111111) << 8) | (uint16_t)(data[1]);
        uint16_t temp_raw = ((uint16_t)(data[2]) << 3) | ((uint16_t)(data[3] & 0b11100000) >> 5);
        *diff_pressure = DLVR_RAW_TO_PA(dp_raw);
        *temperature = DLVR_CONVERT_TEMPERATURE(temp_raw);
    }
    return err;
}
