/*
 * dlvr.h
 *
 *  Created on: May 20, 2023
 *      Author: Danylo Ulianych
 */

#ifndef COMPONENTS_DLVR_DLVR_H_
#define COMPONENTS_DLVR_DLVR_H_

#include <Wire.h>


#define DLVR_I2C_ADDR            (0x28)

/*
* Same as in SDP8x-125
*/
#define DLVR_SDP_PRESSURE_SCALE  (240)


class DLVR {
    private:
        TwoWire &wire;
        uint8_t addr = DLVR_I2C_ADDR;

    public:
        DLVR(TwoWire &wirePort = Wire);
        bool readPressure(int16_t *pressure);
        uint8_t getPressureScale();
};


#endif /* COMPONENTS_DLVR_DLVR_H_ */
