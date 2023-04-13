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
  static constexpr float alpha_fast = 0.50;
  static constexpr float alpha_slow = 0.25;
  float ewma(float data, float ave, float alpha);
  int bar_height(const float *chart);

public:
  GraphicEqualiser(const Palette &palette, int x, int y, int width, int height, int num_bins);
  void update(const float *magnitudes);
  void _draw(TFT_eSPI &display);
};
