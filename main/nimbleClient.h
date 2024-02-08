#ifndef BLE_CLIENT_SCAN_H
#define BLE_CLIENT_SCAN_H

#include <stdio.h>
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Define the UUID of the service to discover
#define MOTOR_CONTROLLER_DATA_UUID 0x180D  // Example 16-bit UUID for the Heart Rate Service

// Struct to store received data
struct motor_controller_data {
    float velocity;
    int motor_battery;
};

// Motor controller read callback
static void motor_controller_chr_read_cb(struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg);

// Function to initiate reading the characteristic value
void receive_motor_controller_data(void);

// Handles devices related to event discovery
static int ble_gap_event(struct ble_gap_event *event, void *arg);

// Scan for devices
void ble_app_scan(void);

// Executed once host syncs with controller
void ble_app_on_sync(void);

// Task run by RTOS
void host_task(void *param)

// Callback for connection events
static void on_connect(int status, struct ble_gap_conn_desc *desc, void *arg);

// Function to initiate connection to a peripheral device
void initiate_connection(void);

// Function to perform service discovery
void perform_service_discovery(uint16_t conn_handle);

// Callback function for service discovery
static void service_discovery_cb(int status, uint16_t svc_handle, const struct ble_gatt_svc *svc, void *arg);

// Function to set up the BLE client
void set_up_client(void);

#endif /* BLE_CLIENT_SCAN_H */
