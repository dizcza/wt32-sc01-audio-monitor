#include <Arduino.h>
#include <M5Display.h>
#include "Waveform.h"

Waveform::Waveform(TFT_eSPI &display, int x, int y, int width, int height, int window_size, float scale_factor, uint16_t color) :
  Component(x, y, width, height),
  m_window_size(window_size),
  scale_factor(scale_factor),
  color(color)
{
  m_samples = static_cast<float *>(calloc(window_size, sizeof(float)));
}

void Waveform::update(const float *samples, int size)
{
  memmove(m_samples, &m_samples[size], (m_window_size - size) * sizeof(float));
  memmove(&m_samples[m_window_size - size], samples, size * sizeof(float));
}

void Waveform::_draw(TFT_eSPI &display)
{
  float xp = 0;
  float x_step = (float)width * point_step / (float)m_window_size;
  display.fillRect(0, y, width, height, 0);
  const int32_t yc = y + height / 2;
  const int32_t y_bound = y + height;
  for (int i = point_step; i < m_window_size; i += point_step)
  {
    int32_t y0 = yc + m_samples[i - point_step] * scale_factor;
    int32_t y1 = yc + m_samples[i] * scale_factor;
    if (y1 > y_bound) y1 = y_bound;
    if (y0 > y_bound) y0 = y_bound;
    display.drawLine(xp, y0, xp + x_step, y1, color);
    xp += x_step;
  }
}