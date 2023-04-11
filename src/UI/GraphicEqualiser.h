#pragma once

#include "Component.h"
#include "Palette.h"


class GraphicEqualiser : public Component
{
private:
  const Palette &m_palette;
  int m_num_bins;
  float *bar_chart;
  float *bar_chart_peaks;

public:
  GraphicEqualiser(const Palette &palette, int x, int y, int width, int height, int num_bins);
  void update(const float *magnitudes, int size);
  void _draw(TFT_eSPI &display);
};
