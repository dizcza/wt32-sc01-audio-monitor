#include <Arduino.h>
#include <algorithm>
#include <M5Display.h>
#include "Palette.h"
#include "GraphicEqualiser.h"

#undef min

GraphicEqualiser::GraphicEqualiser(const Palette &palette, int x, int y, int width, int height, int num_bins) : Component(x, y, width, height), m_palette(palette)
{
  m_num_bins = num_bins;
  bar_chart = static_cast<float *>(malloc(sizeof(float) * num_bins));
  for (int i = 0; i < num_bins; i++)
  {
    bar_chart[i] = 0.0f;
  }
  bar_chart_peaks = static_cast<float *>(malloc(sizeof(float) * num_bins));
  for (int i = 0; i < num_bins; i++)
  {
    bar_chart_peaks[i] = 0.0f;
  }
}

float GraphicEqualiser::ewma(float data, float ave, float alpha)
{
  if (data > ave) {
    return data;
  }
  return alpha * data + (1.0f - alpha) * ave;
}


int GraphicEqualiser::bar_height(const float *chart) {
  float ave = 0;
  for (int j = 0; j < 4; j++)
  {
    ave += chart[j];
  }
  ave /= 4;
  return std::min(height, int(ave));
}


void GraphicEqualiser::update(const float *magnitudes)
{
  for (int i = 0; i < m_num_bins; i++)
  {
    bar_chart[i] = ewma(magnitudes[i], bar_chart[i], alpha_fast);
    bar_chart_peaks[i] = ewma(magnitudes[i], bar_chart_peaks[i], alpha_slow);
  }
}

void GraphicEqualiser::_draw(TFT_eSPI &display)
{
  int x = 0;
  int x_step = int(width / (m_num_bins / 16));
  for (int i = 2; i < m_num_bins / 4; i += 4)
  {
    int bar_value = bar_height(&bar_chart[i]);
    int peak_value = bar_height(&bar_chart_peaks[i]);
    display.fillRect(x, y, x_step, height - bar_value - 1, 0);
    display.drawLine(x, y + height - peak_value - 1, x + x_step - 1, y + height - peak_value - 1, m_palette.get_color(135 + peak_value));
    display.fillRect(x, y + height - bar_value - 1, x_step - 1, bar_value, m_palette.get_color(135 + bar_value));
    x += x_step;
  }
  display.fillRect(x, y, width - x, height, 0);
}