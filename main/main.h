#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include "physicalInputHandler.h"

// I2C (screen) — GPIO0 (scl) and GPIO1 (sda)
// analog input (hall sensor) — GPIO4
// digital input (button) — GPIO5
// digital output (motor) — GPIO10
#define DISP_SCL 0
#define DISP_SDA 1
#define HALL_SENSOR_IN 4
#define BUTTON_IN 5
#define MOTOR_OUT 10

