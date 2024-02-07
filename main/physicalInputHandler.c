#include "main.h"

bool remote_on = true;

void setup_inputs(void)
{
    gpio_reset_pin(BUTTON_IN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BUTTON_IN, GPIO_MODE_INPUT);

    button_setup();
}

static void button_single_click_cb(void *arg,void *usr_data)
{
    ESP_LOGI("Mode Changed to :", "BUTTON_SINGLE_CLICK");
}

static void button_double_click_cb(void *arg,void *usr_data)
{
    ESP_LOGI("Mode Changed to :", "BUTTON_DOUBLE_CLICK");
}

static void button_long_press_cb(void *arg,void *usr_data)
{
    remote_powered_on = false;
    ESP_LOGI("Mode Changed to :", "BUTTON_LONG_PRESS");
}
void button_setup(void)
{
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_ticks = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_ticks = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
        .gpio_button_config = {
            .gpio_num = 0,
            .active_level = 0,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if(NULL == gpio_btn) {
        ESP_LOGE("Button setup: ", "Button create failed");
    }

    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_single_click_cb,NULL);
    iot_button_register_cb(gpio_btn, BUTTON_DOUBLE_CLICK, button_double_click_cb,NULL);
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_UP, button_long_press_cb,NULL);
}



// int get_digital_input(int pin_number)
// {
//     return gpio_get_level(pin_number);
// }
