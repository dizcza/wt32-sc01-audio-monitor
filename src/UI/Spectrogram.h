#pragma once

#include "Component.h"
#include "Bitmap.h"
#include "Palette.h"


class Spectrogram : public Component
{
private:
  const Palette &m_palette;
  Bitmap bitmap;

public:
  Spectrogram(const Palette &palette, int x, int y, int width, int height);
  void update(const float *magnitudes, int size);
  void _draw(TFT_eSPI &display);
};
