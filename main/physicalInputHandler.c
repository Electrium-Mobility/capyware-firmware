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



// ADC STUFF
static TaskHandle_t s_task_handle;
// set adc channel
static adc_channel_t channel = ADC_CHANNEL_3;


// check if converter is done
static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    return (mustYield == pdTRUE);
}

// initialize continuous adc
void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = ADC_ATTEN;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = ADC_UNIT;
        adc_pattern[i].bit_width = ADC_BIT_WIDTH;

        ESP_LOGI("adc_pattern[%d].atten is :%"PRIx8, i, adc_pattern[i].atten);
        ESP_LOGI("adc_pattern[%d].channel is :%"PRIx8, i, adc_pattern[i].channel);
        ESP_LOGI("adc_pattern[%d].unit is :%"PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    *out_handle = handle;
}


void handle_ADC() {
    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[READ_LEN] = {0};
    memset(result, 0xcc, READ_LEN);

    s_task_handle = xTaskGetCurrentTaskHandle();

    adc_continuous_handle_t handle = NULL;
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));

    while (1) {

        /**
         * This is to show you the way to use the ADC continuous mode driver event callback.
         * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
         * However in this example, the data processing (print) is slow, so you barely block here.
         *
         * Without using this event callback (to notify this task), you can still just call
         * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        char unit[] = ADC_UNIT_STR(ADC_UNIT);

        while (1) {
            ret = adc_continuous_read(handle, result, READ_LEN, &ret_num, 0);
            if (ret == ESP_OK) {
                ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32" bytes", ret, ret_num);
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
                    uint32_t chan_num = ADC_GET_CHANNEL(p);
                    uint32_t data = ADC_GET_DATA(p);
                    /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
                    if (chan_num < SOC_ADC_CHANNEL_NUM(ADC_UNIT)) {
                        ESP_LOGI("Unit: %s, Channel: %"PRIu32", Value: %"PRIx32, unit, chan_num, data);
                    } else {
                        ESP_LOGW("Invalid data [%s_%"PRIu32"_%"PRIx32"]", unit, chan_num, data);
                    }
                }
                /**
                 * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
                 * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
                 * usually you don't need this delay (as this task will block for a while).
                 */
                vTaskDelay(1);
            } else if (ret == ESP_ERR_TIMEOUT) {
                //We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
                break;
            }
        }
    }

    ESP_ERROR_CHECK(adc_continuous_stop(handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(handle));
}