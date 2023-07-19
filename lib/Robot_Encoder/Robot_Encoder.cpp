
#include <Robot_Encoder.h>

// NOTE: Following not support for espressif32@2.1.0

// #include <soc/soc_caps.h>
// #if SOC_PCNT_SUPPORTED
// // Not all esp32 chips support the pcnt (notably the esp32c3 does not)
// #include <soc/pcnt_struct.h>

Robot_Encoder *Robot_Encoder::encoders[MAX_ESP32_ENCODERS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

bool Robot_Encoder::attachedInterrupt = false;
pcnt_isr_handle_t Robot_Encoder::user_isr_handle = NULL;

Robot_Encoder::Robot_Encoder()
{
  attached = false;
  aPinNumber = (gpio_num_t)0;
  // bPinNumber = (gpio_num_t) 0;
  working = false;
  direction = false;
  unit = (pcnt_unit_t)0; // -1 *
}

Robot_Encoder::~Robot_Encoder()
{ // TODO Auto-generated destructor stub
}

/* Decode what PCNT's unit originated an interrupt
   and pass this information together with the event type
   the main program using a queue.
*/
static void IRAM_ATTR pcnt_example_intr_handler(void *arg)
{
  Robot_Encoder *ptr;

  uint32_t intr_status = PCNT.int_st.val;
  int i;

  for (i = 0; i < PCNT_UNIT_MAX; i++)
  {
    if (intr_status & (BIT(i)))
    {
      ptr = Robot_Encoder::encoders[i];
      /* Save the PCNT event type that caused an interrupt
        to pass it to the main program */

      int status = 0;
      if (PCNT.status_unit[i].h_lim_lat)
      {
        status = ptr->r_enc_config.counter_h_lim;
      }
      if (PCNT.status_unit[i].l_lim_lat)
      {
        status = ptr->r_enc_config.counter_l_lim;
      }
      // pcnt_counter_clear(ptr->unit);
      PCNT.int_clr.val = BIT(i); // clear the interrupt
      ptr->count = status + ptr->count;
    }
  }
}

void Robot_Encoder::attach(int a)
{
  if (attached)
  {
    Serial.println("All ready attached, FAIL!");
    return;
  }
  int index = 0;
  for (; index < MAX_ESP32_ENCODERS; index++)
  {
    if (Robot_Encoder::encoders[index] == NULL)
    {
      encoders[index] = this;
      break;
    }
  }
  if (index == MAX_ESP32_ENCODERS)
  {
    Serial.println("Too many encoders, FAIL!");
    return;
  }

  // Set data now that pin attach checks are done
  unit = (pcnt_unit_t)index;
  this->aPinNumber = (gpio_num_t)a;

  // Set up the IO state of hte pin
  gpio_pad_select_gpio(aPinNumber);
  gpio_set_direction(aPinNumber, GPIO_MODE_INPUT);
  gpio_pulldown_en(aPinNumber);

  // Set up encoder PCNT configuration
  r_enc_config.pulse_gpio_num = aPinNumber; // Rotary Encoder Chan A
  r_enc_config.ctrl_gpio_num = -1;          // bPinNumber;    //Rotary Encoder Chan B

  r_enc_config.unit = unit;
  r_enc_config.channel = PCNT_CHANNEL_0;

  // PCNT_COUNT_DIS, PCNT_COUNT_INC, PCNT_COUNT_DEC
  r_enc_config.pos_mode = PCNT_COUNT_DIS;
  r_enc_config.neg_mode = PCNT_COUNT_INC; // Discard Falling-Edge

  r_enc_config.lctrl_mode = PCNT_MODE_KEEP;    // Rising A on HIGH B = CW Step
  r_enc_config.hctrl_mode = PCNT_MODE_DISABLE; // PCNT_MODE_REVERSE; // Rising A on LOW B = CCW Step

  r_enc_config.counter_h_lim = INT16_MAX;
  r_enc_config.counter_l_lim = INT16_MIN;

  pcnt_unit_config(&r_enc_config);

  // Filter out bounces and noise
  pcnt_set_filter_value(unit, 250); // Filter Runt Pulses
  pcnt_filter_enable(unit);

  /* Enable events on  maximum and minimum limit values */
  pcnt_event_enable(unit, PCNT_EVT_H_LIM);
  pcnt_event_enable(unit, PCNT_EVT_L_LIM);

  pcnt_counter_pause(unit); // Initial PCNT init
  pcnt_counter_clear(unit);

  /* Register ISR handler and enable interrupts for PCNT unit */
  if (Robot_Encoder::attachedInterrupt == false)
  {
    Robot_Encoder::attachedInterrupt = true;
    esp_err_t er = pcnt_isr_register(pcnt_example_intr_handler, (void *)NULL, (int)0, (pcnt_isr_handle_t *)&Robot_Encoder::user_isr_handle);
    if (er != ESP_OK)
    {
      Serial.println("Encoder wrap interupt failed");
    }
  }
  pcnt_intr_enable(unit);
  pcnt_counter_resume(unit);
}

void Robot_Encoder::attachCounter(int aPintNumber)
{
  attach(aPintNumber);
}

void Robot_Encoder::setCount(int32_t value)
{
  count = value - getCountRaw();
}
int32_t Robot_Encoder::getCountRaw()
{
  int16_t c;
  pcnt_get_counter_value(unit, &c);
  return c;
}
int32_t Robot_Encoder::getCount()
{
  return getCountRaw() + count;
}

int32_t Robot_Encoder::clearCount()
{
  count = 0;
  return pcnt_counter_clear(unit);
}

int32_t Robot_Encoder::pauseCount()
{
  return pcnt_counter_pause(unit);
}

int32_t Robot_Encoder::resumeCount()
{
  return pcnt_counter_resume(unit);
}

// #else
// #warning PCNT not supported on this SoC, this will likely lead to linker errors when using ESP32Encoder
// #endif // SOC_PCNT_SUPPORTED