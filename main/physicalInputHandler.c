#include "main.h"


int remote_on = 1;
int speed_level = 1;
int forward_direction = 1;

void setup_inputs(void)
{
    gpio_reset_pin(BUTTON_IN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BUTTON_IN, GPIO_MODE_INPUT);

    button_setup();
}

static void button_single_click_cb(void *arg,void *usr_data)
{
    speed_level = (speed_level % 3) + 1;
    ESP_LOGI("Mode:", "Speed Level: %d", speed_level);
}

static void button_double_click_cb(void *arg,void *usr_data)
{
    if (forward_direction){
        forward_direction = 0;
        ESP_LOGI("Direction:", "Backwards");
    } else {
        forward_direction = 1;
        ESP_LOGI("Direction:", "Forward");
    }
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
