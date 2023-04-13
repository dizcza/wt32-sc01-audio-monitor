#include <M5StX.h>
#include "Application.h"
#include "SpectrogramConfig.h"


Application::Application(TFT_eSPI &display, SDPSensor& sensor) :
  m_ui(display, SPECTROGRAM_WINDOW_SIZE),
  m_sdp_sampler(sensor),
  m_processor(SPECTROGRAM_WINDOW_SIZE, m_sdp_sampler.pressure_scale)
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
  int size = 0;
  const float *samples = m_sdp_sampler.getCapturedAudioBuffer(&size);
  if (size > 0) {
    m_processor.update(samples, size);
    m_ui.update(m_processor, samples, size);
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