#include <Arduino.h>
#include <M5StX.h>
#include <M5ez.h>
#include "Application.h"

Application *application;
SDP8XX sensor = SDP8XX(Address5);


void setup()
{
  ez.begin();
  application = new Application(M5.lcd, sensor);
}


void show_spectrogram() {
  if (!application->begin()) {
    ez.msgBox("Spectrogram", "FAILED");
    return;
  }

  while (!M5.BtnA.wasPressed()) {
    application->loop();
    M5.BtnA.read();
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  application->stop();
}


void loop()
{
  ezMenu topmenu("Top Menu");
  topmenu.buttons("up # select # down");
  topmenu.addItem("Spectrogram", show_spectrogram); 
  topmenu.run();
}