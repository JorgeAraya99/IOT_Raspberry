/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_event.h"

#include "esp_sleep.h"
#include "packeting.c"

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
#define uS_TO_M_FACTOR 60000000  /* Conversion factor for micro seconds to minutes */
// #define TIME_TO_SLEEP  5

int TCP_send_frag(const char* TAG, int sock, char status, char protocolo){
    //Parte el mensaje (payload) en trozos de 1000 btyes y los manda por separado, esperando un OK con cada trozo
    printf("Sending!\n");
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo+1) -1;
    char rx_buffer[128];
    int packLen = 1000;

    for (int i = 0; i < payloadLen; i += packLen){
        // Generamos el siguiente trozo
        int size = fmin(packLen, payloadLen - i);
        char *pack = malloc(size);
        memcpy(pack, &(payload[i]), size);

        //Enviamos el trozo
        ESP_LOGI(TAG, "sending fragment %i by 4 protocol", i);
        int err = send(sock, pack, size, 0);
        free(pack);
        if (err < 0){
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        }

        // wait for confirmation
        int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        // Error occurred during receiving
        if (len < 0){
            //En caso de error abortamos
            ESP_LOGE(TAG, "recv failed: errno %d", errno);
            break;
        }else{
            rx_buffer[len] = 0;
            char OK_r = rx_buffer[0];
            if (!OK_r){
                ESP_LOGE(TAG, "Server error in fragmented send.");
                free(payload);
                return -1;
            }else{
                ESP_LOGI(TAG, "fragment %i received correctly", i);
            }
        }
    }
    //el último mensaje es solo un \0 para avisarle al server que terminamos
    int err = send(sock, "\0", 1, 0);

    free(payload);

    return err;
}

//********************CLIENTE************************
void tcp_client(const char* TAG, int status, int sleep_time){

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

        ESP_LOGI(TAG, "sending data to %s by TCP", host_ip);
        esp_sleep_enable_timer_wakeup(sleep_time * uS_TO_M_FACTOR);
        while (1) {
            if(id_protocol == 4){
                int err = TCP_send_frag(TAG, sock, tp_layer ,id_protocol);
                if (err < 0){
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                }
            }else{
                ESP_LOGI(TAG,"creating message");
                char* msg = mensaje(id_protocol, tp_layer);
                ESP_LOGI(TAG,"sending data with %i protocol", id_protocol);
                int err = send(sock, msg, strlen(msg), 0);
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                    free(msg);
                    break;
                }
                free(msg);
            }
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
                // Data received
            else {
                buf[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                ESP_LOGI(TAG, "%s", rx_buffer);
                if(status == 22) esp_deep_sleep_start();
            }
        } 

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
        
    }
}