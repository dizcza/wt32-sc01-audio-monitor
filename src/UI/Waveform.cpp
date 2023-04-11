#include <Arduino.h>
#include <M5Display.h>
#include "Waveform.h"

Waveform::Waveform(TFT_eSPI &display, int x, int y, int width, int height, int window_size) : Component(x, y, width, height), m_window_size(window_size)
{
  m_samples = static_cast<float *>(malloc(sizeof(float) * window_size));
}

void Waveform::update(const float *samples)
{
  memcpy(m_samples, samples, sizeof(float) * m_window_size);
}

void Waveform::_draw(TFT_eSPI &display)
{
  float x = 0;
  float x_step = (float)width / (float)m_window_size;
  display.fillRect(x, y, width, height, 0);
  for (int i = 4; i < m_window_size; i += 4)
  {
    display.drawLine(x, height / 2 + m_samples[i - 4] * 3, x + x_step * 4, height / 2 + m_samples[i] * 3, 0xfff);
    x += x_step * 4;
  }
}