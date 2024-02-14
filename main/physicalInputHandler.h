#include "esp_log.h"
#include "iot_button.h"
#include "driver/gpio.h"
#include "esp_adc/adc_continuous.h"
#include "nvs_flash.h"
#include "nvs.h"

void setup_inputs(void);
button_handle_t button_setup(void);
void nvs_init(void);
void store_speed_level(void);
void load_speed_level(void);
int get_digital_input(int pin_number);
void button_single_click_cb(void *arg,void *usr_data);
void button_double_click_cb(void *arg,void *usr_data);
void button_long_press_cb(void *arg,void *usr_data);