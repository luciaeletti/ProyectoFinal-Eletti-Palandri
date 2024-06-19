/*
 * Connection.h
 *
 *  Created on: 15/04/2024
 *      Author: Prototipado
 */
#ifndef CONNECTION_H
#define CONNECTION_H
/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "esp_event.h"
#include "conditions.h"
#include "at24c.h"
#include "store.h"
/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
/*==================[internal functions declaration]=========================*/
void event_handler_smartconfig(void* arg_sc, esp_event_base_t event_base_sc, int32_t event_id_sc, void* event_data_sc);
//void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);                              
void mqtt_app_start(void);
void initialize_wifi_app(void);
void initialise_wifi(void);

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void log_error_if_nonzero(const char *message, int error_code);
void smartconfig_example_task(void * parm);
/*==================[external functions declaration]=========================*/
void vConnectionTask(void *pvParameters);
void vConnectionApp(void *pvParameters);
void ConnectionMQTT();
void vConnectionWIFI(void *pvParameters);



#endif   #ifndef CONNECTION_H