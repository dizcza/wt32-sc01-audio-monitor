#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

// sample rate for the system
#define SAMPLE_RATE 16000

// approx 30ms of audio @ 16KHz
#define WINDOW_SIZE 256

// i2s config for reading from of I2S
extern i2s_config_t i2s_mic_Config;
// i2s microphone pins
extern i2s_pin_config_t i2s_mic_pins;
