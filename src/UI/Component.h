#pragma once

class TFT_eSPI;
class Component
{
public:
  int x, y, width, height;
  bool visible = false;
  Component(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
  {
  }
  virtual void _draw(TFT_eSPI &display) = 0;
  void draw(TFT_eSPI &display)
  {
    if (visible)
    {
      _draw(display);
    }
  }
};