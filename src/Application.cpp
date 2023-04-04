#include <TFT_eSPI.h>
#include "Application.h"
#include "I2SSampler.h"
#include "I2SMEMSSampler.h"
#include "UI/UI.h"
#include "Processor.h"
#include "config.h"
#include "SDPSampler.h"

// Task to process samples
void processing_task(void *param)
{
  Application *application = (Application *)param;
  // just sit in a loop processing samples as quickly as possible
  while (true)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    application->process_samples();
  }
}

Application::Application(TFT_eSPI &display, FT62XXTouchScreen& touchScreen) : m_touchScreen(touchScreen)
{
  m_window_size = WINDOW_SIZE;
  m_sample_buffer = (int16_t *)malloc(sizeof(int16_t) * WINDOW_SIZE);
  m_ui = new UI(display, m_window_size);
  m_processor = new Processor(m_window_size);
  m_sampler = new I2SMEMSSampler(i2s_mic_pins, i2s_mic_Config);
}

void Application::begin()
{
  // set up the processing
  TaskHandle_t processing_task_handle;
  xTaskCreatePinnedToCore(processing_task, "Processing Task", 4096, this, 2, &processing_task_handle, 0);

  // start sampling from i2s device
  m_sampler->start();

  m_sdp_sampler = new SDPSampler(m_window_size, processing_task_handle);
}

// void Application::process_samples()
// {
//   // grab the samples
//   m_sampler->read(m_sample_buffer, WINDOW_SIZE);
//   m_processor->update(m_sample_buffer);
//   m_ui->update(m_processor->m_fft_input, m_processor->m_energy);
// }


void Application::process_samples()
{
  // grab the samples
  int16_t *samples = m_sdp_sampler->getCapturedAudioBuffer();
  if (samples) {
    m_processor->update(samples);
    free(samples);
    m_ui->update(m_processor->m_fft_input, m_processor->m_energy);
  }
}

void Application::loop()
{
  TouchPoint touchPos = m_touchScreen.read();
  if (touchPos.touched)
  {
    m_ui->toggle_display();
    // delay to allow for the touch to finish
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}