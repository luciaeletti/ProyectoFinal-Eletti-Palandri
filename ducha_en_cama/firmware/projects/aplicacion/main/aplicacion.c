/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
/*==================[inclusions]=============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/gpio.h"
#include "LCD_I2C.h"
#include "driver/i2c.h"
#include "KeyPad.h"
#include "FDC1004.h"
#include "ds18b20.h"
#include "interface.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/


/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/

/*==================[external functions declaration]=========================*/

void medirnivel(){
   
  /*  
    LCDI2C_clear();
    char nivel_agua[10]; 
    snprintf(nivel_agua, 10, "%.2f"); 
    LCDI2C_setCursor(0,1);
    LCDI2C_print("El nivel de agua es: ");
    LCDI2C_setCursor(0,2);
    LCDI2C_print(nivel_agua);
    delayMs(3000);
*/
}

  


void app_main(void){
    
  /*  esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
	LCDI2C_init(LCD_ADDR,20,4,I2C_MASTER_SDA_IO,I2C_MASTER_SCL_IO);
	LCDI2C_backlight();

 	gpio_set_direction(UP_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(DOWN_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(SELECT_BUTTON_PIN, GPIO_MODE_INPUT);

  	gpio_set_pull_mode(UP_BUTTON_PIN, GPIO_PULLUP_ONLY);
  	gpio_set_pull_mode(DOWN_BUTTON_PIN, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(SELECT_BUTTON_PIN, GPIO_PULLUP_ONLY);

 	button_queue = xQueueCreate(10, sizeof(button_event_t));
  	button_timer = xTimerCreate("button_timer", pdMS_TO_TICKS(100), pdTRUE, (void *)0, button_timer_callback);
    xTimerStart(button_timer, 0);

	LCDI2C_setCursor(0,0);
	LCDI2C_print("********************");
	LCDI2C_setCursor(0,1);
	LCDI2C_print("      SISTEMA ");
	LCDI2C_setCursor(0,2);
	LCDI2C_print("   DUCHA EN CAMA ");
	LCDI2C_setCursor(0,3);
	LCDI2C_print("********************");
	delayMs(5000);
	LCDI2C_clear();
	LCDI2C_setCursor(0,0);
	LCDI2C_print(" Elija una opcion: ");
 	print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);

   while (1) {
        if (xQueueReceive(button_queue, &event, portMAX_DELAY)) {
            switch (event) {
                case BUTTON_UP:
                    selected = (selected - 1 + sizeof(menus) / sizeof(menus[0])) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_DOWN:
                    selected = (selected + 1) % (sizeof(menus) / sizeof(menus[0]));
					print_menu(menus, sizeof(menus) / sizeof(menus[0]), selected);
                    break;
                case BUTTON_SELECT:
                    sub_menu(selected);
                    break;
            }
            
        }

}
*/


}



