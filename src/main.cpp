#include <Arduino.h>
#include <M5StX.h>
#include "Application.h"

Application *application;

M5Display display = M5Display();


void setup()
{
  Serial.begin(115200);
  display.begin();

  Wire1.begin(21, 22);
  Wire1.setClock(400000);

  application = new Application(display);
  application->begin();
}

void loop()
{
  // service the application
  application->loop();
}