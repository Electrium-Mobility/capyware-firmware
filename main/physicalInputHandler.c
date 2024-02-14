#include "main.h"

int remote_on = 1;
int speed_level = 1;
int forward_direction = 1;

void setup_inputs(void)
{
    gpio_reset_pin(BUTTON_IN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BUTTON_IN, GPIO_MODE_INPUT);

    button_handle_t gpio_btn = button_setup();

    /* Register callback functions */
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_single_click_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_DOUBLE_CLICK, button_double_click_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_UP, button_long_press_cb, NULL);

    /* Init the Non Volatile Storage to store speed level */
    nvs_init();

    load_speed_level();

}

button_handle_t button_setup(void)
{
    /* Button initialization */
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
        .gpio_button_config = {
            .gpio_num = 0,
            .active_level = 0,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if(NULL == gpio_btn) {
        ESP_LOGE("Button setup: ", "Button create failed");
        return NULL;
    }
    return gpio_btn;
}

void nvs_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

/* Function to store the current speed level to NVS*/
void store_speed_level(void)
{
    nvs_handle_t handle;
    esp_err_t err;

    // Open
    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK){
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    // Write
    err = nvs_set_i32(handle, "speed_level", speed_level);
    if (err != ESP_OK){
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        return;
    }
    // Commit written value.
    err = nvs_commit(handle);
    if (err != ESP_OK){
        printf("Error (%s) commiting writing to NVS!\n", esp_err_to_name(err));
        return;
    }
    // Close
    nvs_close(handle);
}

void load_speed_level(void)
{
    nvs_handle_t handle;
    esp_err_t err;
    int32_t speed_level_temp = 1; // Default value if we cannot load a speed value

    // Open
    err = nvs_open("storage", NVS_READONLY, &handle);
    if (err != ESP_OK){
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    // Read
    err = nvs_get_i32(handle, "speed_level", &speed_level_temp);
    speed_level = speed_level_temp;
    if (err != ESP_OK){
        printf("Error (%s) trying to read from storage!\n", esp_err_to_name(err));
        return;
    }
    // Close
    nvs_close(handle);
}

/* Callback Functions */
void button_single_click_cb(void *arg, void *usr_data)
{
    speed_level = (speed_level % 3) + 1;
    ESP_LOGI("Mode:", "Speed Level: %d", speed_level);
}

void button_double_click_cb(void *arg,void *usr_data)
{
    if (forward_direction){
        forward_direction = 0;
        ESP_LOGI("Direction:", "Backwards");
    } else {
        forward_direction = 1;
        ESP_LOGI("Direction:", "Forward");
    }
}

void button_long_press_cb(void *arg,void *usr_data)
{
    remote_on = false;
    ESP_LOGI("Power off", "BUTTON_LONG_PRESS");
    store_speed_level();
}

int get_digital_input(int pin_number)
{
    return gpio_get_level(pin_number);
}
