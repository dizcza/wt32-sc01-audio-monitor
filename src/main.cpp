#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Application.h"
#include "FT62XXTouchScreen.h"

Application *application;

FT62XXTouchScreen touchScreen = FT62XXTouchScreen(TFT_WIDTH, PIN_SDA, PIN_SCL);
TFT_eSPI display = TFT_eSPI();

void setup()
{
  Serial.begin(115200);
  display.begin();
  display.setRotation(1);

  // Backlight hack...
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, 128);

  touchScreen.begin();

  application = new Application(display, touchScreen);
  application->begin();
}

void loop()
{
  // service the application
  application->loop();
}