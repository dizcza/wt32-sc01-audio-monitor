#pragma once

#include "Component.h"

#define WAVEFORM_SCALE_RAW_DATA  (15.0f)
#define WAVEFORM_SCALE_TRIGGER   (40.0f)


class Waveform : public Component
{
private:
  float *m_samples;
  int m_window_size;
  float scale_factor;
  uint16_t color;
  static constexpr int point_step = 4;

public:
  Waveform(TFT_eSPI &display, int x, int y, int width, int height, int window_size, float scale_factor, uint16_t color);
  void update(const float *samples, int size);
  void _draw(TFT_eSPI &display);
};
