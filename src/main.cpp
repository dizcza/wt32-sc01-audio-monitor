#include <Arduino.h>
#include <M5StX.h>
#include <M5ez.h>
#include "Application.h"

Application *application;


void setup()
{
  ez.begin();

  Wire1.begin(21, 22);
  Wire1.setClock(400000);

  application = new Application(M5.lcd);
  application->begin();
}

void loop()
{
  // service the application
  application->loop();
}