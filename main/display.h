#include "ssd1306.h"

/*
Variables 
I2C_MODE_MASTER
GPIO_PULLUP_ENABLE
GPIO_PULLUP_ENABLE
I2C_SCLK_SRC_FLAG_FOR_NOMAL
SSD1306_I2C_ADDRESS
*/

#define I2C_MASTER_SCL_IO 26        /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 25        /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */

ssd1306_handle_t init_display(int sda_io_num, int scl_io_num, int clk_speed, int clk_flags, int i2c_master_num, int address);
void print_display(ssd1306_handle_t ssd1306_dev, char* str);
void test_display(void);

/**
 To Do:
 Understand what each line of code does
 
*/
