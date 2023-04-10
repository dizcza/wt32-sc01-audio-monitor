#include "Arduino.h"
#include "Bitmap.h"

Bitmap::Bitmap()
{
  memset(pixels, 0, sizeof(pixels));
  for (int i = 0; i < height; i++)
  {
    rows[i] = pixels + width * i;
  }
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      rows[y][x] = 0;
    }
  }
}

void Bitmap::scroll_left()
{
  for (int y = 0; y < height; y++)
  {
    memmove(rows[y], rows[y] + 1, this->width * sizeof(uint16_t) - 1);
  }
}