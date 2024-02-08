// NimBLE Client - Scan
#include "nimbleClient.h"


// Define the UUID of the service to discover
#define MOTOR_CONTROLLER_DATA_UUID 0x180D  // Example 16-bit UUID for the Heart Rate Service


char *TAG = "BLE Client Scan";
uint8_t ble_addr_type;
uint16_t motor_controller_conn_handle;
void ble_app_scan(void);


// Define the struct to store received data
struct motor_controller_data {
    float velocity;
    int motor_battery;
};

static struct motor_controller_data received_data;

// Callback for reading the characteristic value
static void motor_controller_chr_read_cb(struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg) {
    if (error == NULL) {
        if (attr->om->om_len == sizeof(struct motor_controller_data)) {
            memcpy(&received_data, attr->om->om_data, sizeof(struct motor_controller_data));
            printf("Received velocity: %.2f\n", received_data.velocity);
            printf("Received motor battery: %d\n", received_data.motor_battery);
        } else {
            printf("Invalid data length received\n");
        }
    } else {
        printf("Error reading characteristic value: %d\n", error->status);
    }
}

// Function to initiate reading the characteristic value
void receive_motor_controller_data(void) {
    // todo: get attribute handle and connection handle
    ble_gattc_read(motor_controller_conn_handle, MOTOR_CONTROLLER_DATA_UUID, motor_controller_chr_read_cb, NULL);
}

// BLE event handling
// handles events related to device discovery
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    struct ble_hs_adv_fields fields;

    switch (event->type)
    {
    // NimBLE event discovery
    case BLE_GAP_EVENT_DISC:
        ESP_LOGI("GAP", "GAP EVENT DISCOVERY");
        ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data);
        if (fields.name_len > 0)
        {
            printf("Name: %.*s\n", fields.name_len, fields.name);
        }
        break;
    default:
        break;
    }
    return 0;
}

void ble_app_scan(void)
{
    printf("Start scanning ...\n");

    struct ble_gap_disc_params disc_params;
    disc_params.filter_duplicates = 1;
    disc_params.passive = 0;
    disc_params.itvl = 0;
    disc_params.window = 0;
    disc_params.filter_policy = 0;
    disc_params.limited = 0;

    ble_gap_disc(ble_addr_type, BLE_HS_FOREVER, &disc_params, ble_gap_event, NULL);
}

// The application
// Starts running once the stack is synchronized with the controller
void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically
    // shows all scanned devices
    ble_app_scan(); 
    // initiates a connection
    initiate_connection();                         
}

// Callback function for connection events
static void on_connect(int status, struct ble_gap_conn_desc *desc, void *arg) {
    if (status == 0) {
        printf("Connection established!\n");
        printf("Connection Handle: %d\n", desc->conn_handle);
        // Store the connection handle for future use
        motor_controller_conn_handle = desc->conn_handle;
    } else {
        printf("Connection failed! Status: %d\n", status);
    }
}

// // The infinite task
void host_task(void *param)
{
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed
}

// Function to initiate connection to a peripheral device
void initiate_connection() {
    struct ble_gap_conn_params params;
    memset(&params, 0, sizeof(params));
    // Set connection parameters as needed

    // Set callback for connection events
    ble_gap_set_connect_cb(on_connect, NULL);

    // Initiate connection to the peripheral device
    ble_gap_connect(..., &params, ...);
}



// Function to perform service discovery
void perform_service_discovery(uint16_t conn_handle) {
    // Perform service discovery for the specified service UUID
    ble_gattc_disc_all_svcs(conn_handle, MOTOR_CONTROLLER_DATA_UUID, service_discovery_cb, NULL);
}

// Callback function for service discovery
static void service_discovery_cb(int status, uint16_t svc_handle, const struct ble_gatt_svc *svc, void *arg) {
    if (status == 0 && svc != NULL) {
        printf("Service discovered!\n");

        
    } else {
        printf("Service discovery failed! Status: %d\n", status);
    }
}



void set_up_client()
{
    nvs_flash_init();                               // 1 - Initialize NVS flash using
    esp_nimble_hci_and_controller_init();           // 2 - Initialize ESP controller
    nimble_port_init();                             // 3 - Initialize the controller stack
    ble_svc_gap_device_name_set("BLE-Scan-Client"); // 4 - Set device name characteristic
    ble_svc_gap_init();                             // 4 - Initialize GAP service

    // sets up call back function on device synchronization
    ble_hs_cfg.sync_cb = ble_app_on_sync;           // 5 - Set application
    nimble_port_run();

    // setup RTOS task
    nimble_port_freertos_init(host_task);           // 6 - Set infinite task
}