/*
 * dlvr.h
 *
 *  Created on: May 20, 2023
 *      Author: Danylo Ulianych
 */

#ifndef COMPONENTS_DLVR_DLVR_H_
#define COMPONENTS_DLVR_DLVR_H_

#include "driver/i2c.h"


#ifdef __cplusplus
 extern "C" {
#endif

#define DLVR_I2C_ADDR            (0x28)

/*
* Same as in SDP8x-125
*/
#define DLVR_SDP_PRESSURE_SCALE  (240)


void dlvr_init(i2c_port_t i2c_port);

esp_err_t dlvr_read_pressure_sdp(int16_t* diff_pressure_sdp);

esp_err_t dlvr_read_pressure_raw(uint16_t* diff_pressure_raw);

esp_err_t dlvr_read_pressure_temperature(float* diff_pressure, float* temperature);

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_DLVR_DLVR_H_ */
