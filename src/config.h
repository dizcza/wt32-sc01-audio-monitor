#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

// sample rate for the system
#define SAMPLE_RATE 16000

// approx 30ms of audio @ 16KHz
#define WINDOW_SIZE 512

// are you using an I2S microphone - comment this out if you want to use an analog mic and ADC input
#define USE_I2S_MIC_INPUT

// I2S Microphone Settings
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
// Generally they will default to LEFT - but you may need to attach the L/R pin to GND
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// #define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_5
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_26

// Analog Microphone Settings - ADC1_CHANNEL_7 is GPIO35
#define ADC_MIC_CHANNEL ADC1_CHANNEL_7

// i2s config for reading from of I2S
extern i2s_config_t i2s_mic_Config;
// i2s microphone pins
extern i2s_pin_config_t i2s_mic_pins;
