#pragma once

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/voltage_sampler/voltage_sampler.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace adc {

class ADCSensor : public sensor::Sensor,
                  public PollingComponent,
                  public voltage_sampler::VoltageSampler {
 public:
  /// Set the attenuation for this pin. Only available on the ESP32.
  void set_attenuation(adc_atten_t attenuation) { attenuation_ = attenuation; }
  void set_channel1(int channel) { channel1_ = channel; }
  void set_channel2(int channel) {}
  void set_autorange(bool autorange) { autorange_ = autorange; }

  /// Update ADC values
  void update() override;
  /// Setup ADC
  void setup() override;
  void dump_config() override;
  /// `HARDWARE_LATE` setup priority
  float get_setup_priority() const override;
  void set_pin(InternalGPIOPin *pin) { this->pin_ = pin; }
  void set_output_raw(bool output_raw) { output_raw_ = output_raw; }
  float sample() override;

 protected:
  InternalGPIOPin *pin_;
  bool output_raw_{false};

  adc_atten_t attenuation_{ADC_ATTEN_DB_0};
  adc_oneshot_unit_handle_t handle_;
  adc_cali_handle_t cali_handle_;
  int channel1_;
  bool autorange_{false};
};

}  // namespace adc
}  // namespace esphome
