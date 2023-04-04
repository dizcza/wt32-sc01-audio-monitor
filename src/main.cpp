#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Application.h"
#include "AXP192.h"
#include "FT62XXTouchScreen.h"

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

Application *application;

// AXP192 Axp;

FT62XXTouchScreen touchScreen = FT62XXTouchScreen(DISPLAY_HEIGHT, PIN_SDA, PIN_SCL);
TFT_eSPI display = TFT_eSPI();

void setup()
{
  // Axp.begin();

  Serial.begin(115200);
  display.begin();
  display.setRotation(1);

  // Backlight hack...
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, 128);

  touchScreen.begin();

  Wire1.begin(32, 33);
  Wire1.setClock(400000);

  application = new Application(display, touchScreen);
  application->begin();
}

void loop()
{
  // service the application
  application->loop();
}