#pragma once

class UI;
class Processor;
class M5Display;
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
  Application(M5Display &display);
  void begin();
  void loop();

  friend void processing_task(void *param);
};