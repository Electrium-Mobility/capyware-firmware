#include "driver/gpio.h"
#include "esp_adc/adc_continuous.h"

void setup_inputs(void);
int get_digital_input(int pin_number);