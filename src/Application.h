#pragma once

#include "UI/UI.h"
#include "Processor.h"
#include "SDPSampler.h"


class Application
{
private:
  UI m_ui;
  Processor m_processor;
  SDPSampler m_sdp_sampler;

  void process_samples();

public:
  Application(TFT_eSPI &display);
  bool begin();
  void stop();
  void loop();

  friend void processing_task(void *param);
};