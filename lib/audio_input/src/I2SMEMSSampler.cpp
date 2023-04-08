#include "I2SMEMSSampler.h"
#include "soc/i2s_reg.h"

I2SMEMSSampler::I2SMEMSSampler(
    i2s_pin_config_t &i2s_pins,
    i2s_config_t i2s_config) : I2SSampler(i2s_config)
{
    m_i2sPins = i2s_pins;
}

void I2SMEMSSampler::configureI2S()
{
    i2s_set_pin(I2S_NUM_0, &m_i2sPins);
}

int I2SMEMSSampler::read(int16_t *samples, int count)
{
    int32_t * raw_samples = (int32_t *)malloc(sizeof(int32_t) * count);
    assert(raw_samples != NULL);
    size_t bytes_read = 0;
    ESP_ERROR_CHECK(i2s_read(I2S_NUM_0, (void*) raw_samples, sizeof(int32_t) * count, &bytes_read, portMAX_DELAY));
    int samples_read = bytes_read / sizeof(int32_t);
    for (int i = 0; i < samples_read; i++)
    {
        samples[i] = (raw_samples[i] & 0xFFFFFFF0) >> 11;
    }
    free(raw_samples);
    return samples_read;
}
