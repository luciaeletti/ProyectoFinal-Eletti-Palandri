/** @file Connection.c
 * @version		1.0
 * @date		15/04/2024
 * @author		Eletti Lucía Valentina
 * @note
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2024.
 */
/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_event.h"
#include "esp_wpa2.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "gpio_mcu.h"
#include "at24c.h"
#include "mqtt_client.h"
#include "connection.h"
#include "conditions.h"
#include "definitions.h"
/*==================[macros]=================================================*/
static const char *TAG = "WIFI";

uint8_t value = DISCONNECTION;

EventGroupHandle_t s_wifi_event_group_sc;

wifi_config_t wifi_config_sc;

nvs_handle_t my_handle;

const int CONNECTED_BIT = BIT0;
const int ESPTOUCH_DONE_BIT = BIT1;

INFO_CONNECTION_T my_connection;
DATA_CONNECTION_T my_wifi = {0};


void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://mqtt.eclipseprojects.io",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void smartconfig_example_task(void *parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
    smartconfig_start_config_t cfg_sc = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_smartconfig_start(&cfg_sc));
    while (1)
    {
        uxBits = xEventGroupWaitBits(s_wifi_event_group_sc, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
        if (uxBits & CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        if (uxBits & ESPTOUCH_DONE_BIT)
        {
            ESP_LOGI(TAG, "smartconfig over");
            my_connection.flag = true;
            SetInfoConnection(&my_connection);
            GPIOInit(WIFI_OK, GPIO_OUTPUT);
            GPIOOn(WIFI_OK);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            GPIOOff(WIFI_OK);
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}

void event_handler_smartconfig(void* arg_sc, esp_event_base_t event_base_sc, int32_t event_id_sc, void* event_data_sc)
{
    if (event_base_sc == WIFI_EVENT && event_id_sc == WIFI_EVENT_STA_START)
    {
        xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
    }
    else if (event_base_sc == WIFI_EVENT && event_id_sc == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group_sc, CONNECTED_BIT);
    }
    else if (event_base_sc == IP_EVENT && event_id_sc == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(s_wifi_event_group_sc, CONNECTED_BIT);
    }
    else if (event_base_sc == SC_EVENT && event_id_sc == SC_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "Scan done");
    }
    else if (event_base_sc == SC_EVENT && event_id_sc == SC_EVENT_FOUND_CHANNEL)
    {
        ESP_LOGI(TAG, "Found channel");
    }
    else if (event_base_sc == SC_EVENT && event_id_sc == SC_EVENT_GOT_SSID_PSWD)
    {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data_sc;
        uint8_t ssid[33] = {0};
        uint8_t password[65] = {0};
        uint8_t rvd_data[33] = {0};

        bzero(&wifi_config_sc, sizeof(wifi_config_t));
        memcpy(wifi_config_sc.sta.ssid, evt->ssid, sizeof(wifi_config_sc.sta.ssid));
        memcpy(wifi_config_sc.sta.password, evt->password, sizeof(wifi_config_sc.sta.password));
        wifi_config_sc.sta.bssid_set = evt->bssid_set;
        if (wifi_config_sc.sta.bssid_set == true)
        {
            memcpy(wifi_config_sc.sta.bssid, evt->bssid, sizeof(wifi_config_sc.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        my_wifi.SSID = ssid;
        my_wifi.PASSWORD = password;
     //   memcpy(my_wifi->SSID, evt->ssid, sizeof(evt->ssid));
     //   memcpy(my_wifi->PASSWORD, evt->password, sizeof(evt->password));

      /*  InitRom(&my_memory);
        WriteRom(&my_memory,0x100,my_wifi->SSID);
        WriteRom(&my_memory,0x101,my_wifi->PASSWORD);*/

        
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);
        

        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sc));
        esp_wifi_connect();
    }
    else if (event_base_sc == SC_EVENT && event_id_sc == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(s_wifi_event_group_sc, ESPTOUCH_DONE_BIT);
    }
}

void initialize_wifi_app(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    s_wifi_event_group_sc = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif_sc = esp_netif_create_default_wifi_sta();
    assert(sta_netif_sc);

    wifi_init_config_t cfg_sc = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg_sc));
    // Eventos de smartconfig
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler_smartconfig, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler_smartconfig, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler_smartconfig, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void vConnectionApp(void *pvParameters)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    initialize_wifi_app();
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
/*
void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
*/

void initialise_wifi(void){
   

}

/*
void vConnectionWIFI(void *pvParameters){

    ESP_ERROR_CHECK(nvs_flash_init());

    while (1)
    {
    nvs_open("storage", NVS_READONLY, &my_handle);
    nvs_get_i32(my_handle, "RED", my_wifi->SSID);
    //nvs_open("storage", NVS_READONLY, &my_handle);
   // nvs_get_i32(my_handle, "CONTRASEÑA", my_wifi->PASSWORD);
    nvs_commit(my_handle);
    nvs_close(my_handle);
    printf("LA SSID ES:  %ld.\n\r", my_wifi->SSID);
 //   printf("EL PASSWORD ES:  %ld.\n\r", my_wifi->PASSWORD);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    //guardarrrr
nvs_open("storage", NVS_READWRITE, &my_handle);
        nvs_set_i32(my_handle, "RED", evt->ssid);
        nvs_open("storage", NVS_READWRITE, &my_handle);
        nvs_set_i32(my_handle, "CONTRASEÑA", evt->password);
        nvs_commit(my_handle);
        nvs_close(my_handle);
}*/




void ConnectionMQTT()
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_WS", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    mqtt_app_start();
}

void vConnectionTask(void *pvParameters)
{

    while (1)
    {
        switch (value)
        {
        case CONNECTION_WIFI_SMARTCONFIG:
          //  initialise_wifi_app();

            break;
        case CONNECTION_WIFI_STA:

            break;
        case COMUNICATION_MQTT:

            break;
        case DISCONNECTION:
            GetInfoConnection(&my_connection);
            if (my_connection.mode == CONNECTION_WIFI_SMARTCONFIG)
            {
                value = CONNECTION_WIFI_SMARTCONFIG;
            }
            if (my_connection.mode == CONNECTION_WIFI_STA)
            {
                value = CONNECTION_WIFI_STA;
            }
            // value = DISCONNECTION;
            break;
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}