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
#include "conditions.h"
#include "general.h"
/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/*==================[internal data declaration]==============================*/
/*==================[internal functions declaration]=========================*/
//void vConnectionWFTask(void *pvParameters);
//void vConnectionMQTTTask(void *pvParameters);
void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
                                
void mqtt_app_start(void);
void initialise_wifi_app(void);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void log_error_if_nonzero(const char *message, int error_code);
void smartconfig_example_task(void * parm);
void connection_wifi();
void connection_mqtt();


/*==================[external functions declaration]=========================*/
void vConnectionTask(void *pvParameters);
void vConnectionWFTask(void *pvParameters);
void vConnectionMQTTTask(void *pvParameters);



#endif   #ifndef CONNECTION_H