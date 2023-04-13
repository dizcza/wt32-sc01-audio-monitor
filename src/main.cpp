#include <Arduino.h>
#include <M5StX.h>
#include <M5ez.h>
#include "Application.h"

Application *application;
SDP8XX sensor = SDP8XX(Address5);


void setup()
{
  ez.begin();
  log_d("Heap avail after ez.begin: %lu Kb", esp_get_free_heap_size() / 1024);
  application = new Application(M5.lcd, sensor);
}


void show_spectrogram() {
  if (!application->begin()) {
    return;
  }
  log_d("Heap avail: %lu Kb", esp_get_free_heap_size() / 1024);

  while (!M5.BtnA.wasPressed()) {
    application->loop();
    M5.BtnA.read();
    delay(100);
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