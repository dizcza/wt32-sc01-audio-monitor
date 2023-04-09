#include <M5StX.h>
#include "Application.h"
#include "config.h"


Application::Application(TFT_eSPI &display) : m_ui(display, WINDOW_SIZE), m_processor(WINDOW_SIZE), m_sdp_sampler(WINDOW_SIZE)
{
}

bool Application::begin()
{
  return m_sdp_sampler.begin();
}

void Application::stop()
{
  m_sdp_sampler.stop();
}

void Application::process_samples()
{
  // grab the samples
  int16_t *samples = m_sdp_sampler.getCapturedAudioBuffer();
  if (samples) {
    m_processor.update(samples);
    free(samples);
    m_ui.update(m_processor.m_fft_input, m_processor.m_energy);
  }
}

void Application::loop()
{
  process_samples();
  M5.BtnB.read();
  if (M5.BtnB.wasPressed()) {
    m_ui.toggle_display();
  }
}