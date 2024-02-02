#include "main.h"


void app_main(void)
{
    #if DEBUG == 1
        debug();
    #endif /* DEBUG == 1 */
}

void display_test(){
    SSD1306_t dev; 
    ssd1306_clear_screen(&dev, false); //Bool is for 'invert' parameter. No idea what that does.
	ssd1306_contrast(&dev, 0xff); //What does this do? I don't know.
	ssd1306_display_text_x3(&dev, 0, "Hello", 5, false); //Page number, text, text length, invert parameter
	vTaskDelay(3000 / portTICK_PERIOD_MS); //Add a delay for some reason
}