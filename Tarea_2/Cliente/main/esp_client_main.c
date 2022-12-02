/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_event.h"

#include "esp_sleep.h"

#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "addr_from_stdin.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "esp_system.h"
#include "esp_mac.h"
#include "sdkconfig.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>            // struct addrinfo
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_log.h"
#if defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
#include "addr_from_stdin.h"
#endif

#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
#define HOST_IP_ADDR ""
#endif

#define PORT CONFIG_EXAMPLE_PORT
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5

static const char *TAG = "example";

RTC_DATA_ATTR int8_t status;
RTC_DATA_ATTR int8 id_Protocol;
RTC_DATA_ATTR int32_t bme270_Sampling;
RTC_DATA_ATTR int32_t bme270_Acc_Sensibility;
RTC_DATA_ATTR int32_t bme270_Gyro_Sensibility;
RTC_DATA_ATTR int32_t bme688_Sampling;
RTC_DATA_ATTR int32_t discontinuos_Time;
RTC_DATA_ATTR int32_t port_TCP;
RTC_DATA_ATTR int32_t port_UDP;
RTC_DATA_ATTR int32_t host_IP_Addr;
RTC_DATA_ATTR char ssid_x;
RTC_DATA_ATTR char pass;


void ini_client(void){

    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    int id_protocol = 0;
    int tp_layer = 0;

    while (1) {
#if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
        struct sockaddr_storage dest_addr = { 0 };
        ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_STREAM, &ip_protocol, &addr_family, &dest_addr));
#endif

        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create tcp_socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "TCP socket created, connecting to %s:%d", host_ip, PORT);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Successfully connected");

        //preguntamos por protocolo
        char* conf_msg = dataprotocol00();

        int tries = 0;
        while(1){
            tries++;
            if(tries > 4){
                break;
            }
            ESP_LOGI(TAG, "asking configuration");
            int conf_request = send(sock, conf_msg, strlen(conf_msg), 0);
            if (conf_request < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno); 
                free(conf_msg);
                continue;
            }
            free(conf_msg);
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                ESP_LOGI(TAG, "Received layer %i and protocol %i", rx_buffer[1], rx_buffer[2]);
                id_protocol = rx_buffer[2];
                tp_layer = rx_buffer[1];
                break;
            }
        }

        if(!tp_layer){
            tcp_client(sock, rx_buffer, host_ip, id_protocol, tp_layer);
        }else{
            //xTaskCreate(udp_client, "udp_client", 4096, NULL, 5, NULL);
            udp_client(addr_family, ip_protocol, rx_buffer, host_ip, dest_addr, id_protocol, tp_layer);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    ini_client();
}
