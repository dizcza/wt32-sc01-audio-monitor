
#include "I2SSampler.h"
#include "driver/i2s.h"

I2SSampler::I2SSampler(const i2s_config_t &i2s_config) : m_i2s_config(i2s_config)
{
}

void I2SSampler::start()
{
    //install and start i2s driver
    i2s_driver_install(I2S_NUM_0, &m_i2s_config, 0, NULL);
    // set up the I2S configuration from the subclass
    configureI2S();
}

void I2SSampler::stop()
{
    // stop the i2S driver
    i2s_driver_uninstall(I2S_NUM_0);
}
