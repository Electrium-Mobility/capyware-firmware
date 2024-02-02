#include "main.h"

//static const char *TAG = "example";
void setup_inputs(void)
{
    gpio_reset_pin(BUTTON_IN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BUTTON_IN, GPIO_MODE_INPUT);
}

int get_digital_input(int pin_number)
{
    return gpio_get_level(pin_number);
}
