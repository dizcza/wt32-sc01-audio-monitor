/*
 * dlvr.c
 *
 *  Created on: May 20, 2023
 *      Author: Danylo Ulianych
 */

#include "dlvr.h"

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


DLVR::DLVR(TwoWire &wirePort) : wire(wirePort) {};


bool DLVR::readPressure(int16_t *pressure) {
    const int size = 2;  // read two bytes as int16
    if (wire.requestFrom(addr, size) != size) {
        return false;
    }
    uint8_t data[] = {wire.read(), wire.read()};
    uint8_t status = (data[0] & 0b11000000) >> 6;
    uint16_t pressure_raw = ((uint16_t)(data[0] & 0b00111111) << 8) | (uint16_t)(data[1]);
    float pressure_pa = DLVR_RAW_TO_PA(pressure_raw);
    *pressure = (int16_t) (DLVR_SDP_PRESSURE_SCALE * pressure_pa);
    bool res = status == DLVR_STATUS_OK;
    return res;
}


uint8_t DLVR::getPressureScale() {
    return DLVR_SDP_PRESSURE_SCALE;
}
