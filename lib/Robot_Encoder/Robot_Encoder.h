#pragma once
#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/pcnt.h>
#define MAX_ESP32_ENCODERS PCNT_UNIT_MAX

class Robot_Encoder
{
private:
  void attach(int aPintNumber);
  boolean attached = false;

  static pcnt_isr_handle_t user_isr_handle; // user's ISR service handle
  bool direction;
  bool working;

public:
  Robot_Encoder();
  ~Robot_Encoder();
  void attachCounter(int aPintNumber);
  int32_t getCount();
  int32_t getCountRaw();
  int32_t clearCount();
  int32_t pauseCount();
  int32_t resumeCount();

  boolean isAttached()
  {
    return attached;
  }
  void setCount(int32_t value);
  static Robot_Encoder *encoders[MAX_ESP32_ENCODERS];
  static bool attachedInterrupt;
  gpio_num_t aPinNumber;
  gpio_num_t bPinNumber;
  pcnt_unit_t unit;
  bool fullQuad = false;
  int countsMode = 2;
  volatile int32_t count = 0;
  pcnt_config_t r_enc_config;
};
