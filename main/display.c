#include "main.h"

static ssd1306_handle_t ssd1306_dev = NULL;

ssd1306_handle_t init_display(int sda_io_num, int scl_io_num, int clk_speed, int clk_flags, int i2c_master_num, int address){
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda_io_num;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = scl_io_num;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = clk_speed;
    conf.clk_flags = clk_flags;
    i2c_param_config(i2c_master_num, &conf);
    i2c_driver_install(i2c_master_num, conf.mode, 0, 0, 0);
    return ssd1306_create(i2c_master_num, address);
}

void print_display(ssd1306_handle_t ssd1306_dev, char* str){
    ssd1306_refresh_gram(ssd1306_dev);
    ssd1306_clear_screen(ssd1306_dev, 0x00);

    char data_str[10] = {0};
    sprintf(data_str, str);
    ssd1306_draw_string(ssd1306_dev, 70, 16, (const uint8_t *)data_str, 16, 1);
    ssd1306_refresh_gram(ssd1306_dev);
}

void test_display(void)
{
    ssd1306_dev = init_display((gpio_num_t)I2C_MASTER_SDA_IO, (gpio_num_t)I2C_MASTER_SCL_IO, I2C_MASTER_FREQ_HZ, I2C_SCLK_SRC_FLAG_FOR_NOMAL, I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);
    print_display(ssd1306_dev, "HELLO WORLD!");
}

/**
 To Do:
 Understand what each line of code does
 
*/
