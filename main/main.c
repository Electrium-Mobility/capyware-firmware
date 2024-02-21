#include "main.h"

void app_main(void)
{
    #if DEBUG == 1
        debug();
    #endif /* DEBUG == 1 */
    // fprintf('Hello world')
    setup_inputs();
}

//Init
//