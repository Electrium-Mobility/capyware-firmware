#include "esp_log.h"
#include "iot_button.h"
#include "driver/gpio.h"
#include "esp_adc/adc_continuous.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

// ADC definitions
#define _ADC_UNIT_STR(unit)         #unit
#define ADC_UNIT_STR(unit)          _ADC_UNIT_STR(unit)

#define ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_GET_CHANNEL(p_data)     ((p_data)->type2.channel)
#define ADC_GET_DATA(p_data)        ((p_data)->type2.data)

#define ADC_UNIT                    ADC_UNIT_1
#define ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define ADC_ATTEN                   ADC_ATTEN_DB_12
#define ADC_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH
#define READ_LEN                    256

void setup_inputs(void);
button_handle_t button_setup(int button_num);
void nvs_init(void);
void store_speed_level(void);
void load_speed_level(void);
int get_digital_input(int pin_number);
void button_single_click_cb(void *arg,void *usr_data);
void button_double_click_cb(void *arg,void *usr_data);
void button_long_press_cb(void *arg,void *usr_data);

// ADC Functions
bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data);
void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);
void handle_ADC();
