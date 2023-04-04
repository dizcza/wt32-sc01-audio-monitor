#pragma once
#include "FT62XXTouchScreen.h"
#include <driver/i2s.h>

class UI;
class Processor;
class I2SSampler;
class TFT_eSPI;
class SDPSampler;

class Application
{
private:
  int m_window_size;
  int16_t *m_sample_buffer;
  UI *m_ui;
  Processor *m_processor;
  I2SSampler *m_sampler;
  SDPSampler *m_sdp_sampler;
  FT62XXTouchScreen& m_touchScreen;

  void process_samples();

public:
  Application(TFT_eSPI &display, FT62XXTouchScreen& touchScreen);
  void begin();
  void loop();

  friend void processing_task(void *param);
};