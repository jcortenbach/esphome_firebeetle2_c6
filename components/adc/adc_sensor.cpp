#include "adc_sensor.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace adc {

static const char *const TAG = "adc";

#ifndef SOC_ADC_RTC_MAX_BITWIDTH
#if USE_ESP32_VARIANT_ESP32S2
static const int32_t SOC_ADC_RTC_MAX_BITWIDTH = 13;
#else
static const int32_t SOC_ADC_RTC_MAX_BITWIDTH = 12;
#endif
#endif

static const int ADC_MAX =
    (1 << SOC_ADC_RTC_MAX_BITWIDTH) - 1;  // 4095 (12 bit) or 8191 (13 bit)
static const int ADC_HALF =
    (1 << SOC_ADC_RTC_MAX_BITWIDTH) >> 1;  // 2048 (12 bit) or 4096 (13 bit)

void ADCSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ADC '%s'...", this->get_name().c_str());
  pin_->setup();

  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = ADC_UNIT_1,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &handle_));
  adc_oneshot_chan_cfg_t config = {
      .atten = ADC_ATTEN_DB_12,  // 12 dB = 4x voltage.
      .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(
      handle_, static_cast<adc_channel_t>(channel1_), &config));
  adc_cali_curve_fitting_config_t cali_config = {
      .unit_id = ADC_UNIT_1,
      .atten = ADC_ATTEN_DB_12,
      .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  ESP_ERROR_CHECK(
      adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle_));
  ESP_LOGCONFIG(TAG, "ADC '%s' setup finished!", this->get_name().c_str());
}

void ADCSensor::dump_config() {
  LOG_SENSOR("", "ADC Sensor", this);
  LOG_UPDATE_INTERVAL(this);
}

float ADCSensor::get_setup_priority() const { return setup_priority::DATA; }

void ADCSensor::update() {
  float value_v = this->sample();
  this->publish_state(value_v);
}

float ADCSensor::sample() {
  int raw;
  ESP_ERROR_CHECK(
      adc_oneshot_read(handle_, static_cast<adc_channel_t>(channel1_), &raw));
  int mv;
  ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle_, raw, &mv));
  return mv * 1e-3f;
}

}  // namespace adc
}  // namespace esphome
