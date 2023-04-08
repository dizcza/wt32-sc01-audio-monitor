#pragma once

class UI;
class Processor;
class TFT_eSPI;
class SDPSampler;

class Application
{
private:
  int m_window_size;
  int16_t *m_sample_buffer;
  UI *m_ui;
  Processor *m_processor;
  SDPSampler *m_sdp_sampler;

  void process_samples();

public:
  Application(TFT_eSPI &display);
  void begin();
  void loop();

  friend void processing_task(void *param);
};