#pragma once

#include <stdint.h>
#include <algorithm>

#undef min

class Palette
{
protected:
  uint16_t colors[256];

public:
  static constexpr uint16_t white = 0xFFFF;
  static constexpr uint16_t light_blue = 0x0FFF;

  Palette();
  uint16_t get_color(int index) const
  {
    return colors[std::max(0, std::min(255, index))];
  }
};
