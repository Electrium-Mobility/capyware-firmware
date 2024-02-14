#include "esp_adc/adc_continuous.h"

void setup_inputs(void);
int get_digital_input(int pin_number);
void button_single_click_cb(void *arg,void *usr_data);
void button_double_click_cb(void *arg,void *usr_data);
void button_long_press_cb(void *arg,void *usr_data);