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

//static const char *payload = "Message from ESP32 ";

//******************SENSORES********************
float floatrand(float min, float max){     
    return min + (float)rand()/(float)(RAND_MAX/(max-min)); 
    }

float* acc_sensor_acc_x(){     
    float* arr = malloc(2000* sizeof(float));     
    for (int i =0; i <2000; i++){         
        arr[i] = (float)2 * sin(2.0 * 3.14 * 0.001  * floatrand(-8000, 8000)); 
        //printf("accx = %f\n", arr[i]);
    }     
    return arr; 
}
float* acc_sensor_acc_y(){     
    float* arr = malloc(2000* sizeof(float));     
    for (int i =0; i <2000; i++){         
        arr[i] = (float)3 * cos(2.0 * 3.14 * 0.001  * floatrand(-8000, 8000)); 
        //printf("accy = %f\n", arr[i]);
    }     
    return arr; 
}

float* acc_sensor_acc_z(){     
    float* arr = malloc(2000* sizeof(float));     
    for (int i =0; i <2000; i++){         
        arr[i] = (float)(10 * sin(2.0 * 3.14 * 0.001 * floatrand(-8000, 8000))); 
        //printf("accz = %i\n", arr[i]);
    }     
    return arr; 
}

float* thpc(){
    char pres= floatrand(1000,1200);
    float CO2=floatrand(30,200);
    //printf("CO2 =%f\n", CO2);
    //printf("pres = %i\n", pres);
    float* arr = malloc(2* sizeof(float));
    arr[0]=pres;
    arr[3]=CO2;
    return arr;
}

float THPC_sensor_pres(){
    float pres= floatrand(1000,1200);
    //printf("pres = %i\n", pres);
    return pres;
}

float THPC_sensor_co(){
    float CO2= floatrand(30,200);
    //printf("pres = %i\n", CO2);
    return CO2;
}

char THPC_sensor_temp(){
    char temp=(char) 5+(rand()%25);
    //printf("temp = %c\n", temp +'0');
    //int itemp = (int)temp; -> unused
    //printf("itemp1 = %d\n", itemp);
    return temp;
}

char THPC_sensor_hum(){
    char hum=(char) 30+(rand()%50);
    //printf("hum1 = %c\n", hum+ '0');
    //int ihum = (int)hum; -> unused
    //printf("ihum1 = %d\n", ihum);
    
    return hum;
    
}

char batt_sensor(){
    char bat = (char) 1+(rand()%99);
    //printf("battery = %d\n", bat);
    //int ibat = (int)bat; -> unused
    //printf("ibat = %d\n", ibat);
    return bat;
}

float* Acc_kpi(){
    float ampx= floatrand(59,1200)/10000;
    float frecx= (floatrand(290,310))/10;
    float ampy= (floatrand(41,1100))/10000;
    float frecy= (floatrand(590,610))/10;
    float ampz= floatrand(8,150)/1000;
    float frecz= floatrand(890,910)/10;
    float RMS= sqrt(pow(ampx,2) + pow(ampx,2)+ pow(ampx,2));
    /*
    printf("ampx = %f\n", ampx);
    printf("ampy = %f\n", ampy);
    printf("ampz = %f\n", ampz);
    printf("frecx = %f\n", frecx);
    printf("frecy = %f\n", frecy);
    printf("frecz = %f\n", frecz);
    printf("RMS = %f\n", RMS);
    */
    float* arr = malloc(7* sizeof(float));
    arr[0]=ampx;
    arr[1]=frecx;
    arr[2]=ampy;
    arr[3]=frecy;
    arr[4]=ampz;
    arr[5]=frecz;
    arr[6]=RMS;
    for(int i = 0; i < 7; i++){
        printf("ampx = %f\n", arr[i]);
    }
    return arr;

}

float Acc_kpi_ampx(){
    float ampx= floatrand(59,1200)/10000;
    return ampx;
    
}
float Acc_kpi_ampy(){
    float ampy= (floatrand(41,1100))/10000;
    return ampy;
    
}
float Acc_kpi_ampz(){
    float ampz= floatrand(8,150)/1000;
    return ampz;
    
}
float Acc_kpi_frecx(){
    float frecx= (floatrand(290,310))/10;
    return frecx;
    
}
float Acc_kpi_frecy(){
    float frecy= (floatrand(590,610))/10;
    return frecy;
    
}
float Acc_kpi_frecz(){
    float frecz= floatrand(890,910)/10;
    return frecz;
}

// *****************PACKETING*******************

unsigned short lengmsg[6] = {2, 6, 16, 20, 44, 24016};
unsigned short dataLength(char protocol){
    return lengmsg[ (unsigned int) protocol];
}

unsigned short messageLength(char protocol){
    return 12+dataLength(protocol);
}

int getTime(){
    return (int)time(NULL);
}

//Genera el header de un mensaje, con la MAC, el protocolo, status, y el largo del mensaje.
char* header(char protocol, char transportLayer){
	char* head = malloc(12);

    short ID = 1;
    uint8_t* MACaddrs = malloc(6);
	esp_efuse_mac_get_default(MACaddrs);
    //memcpy((void*) &(head[0]), (void*) MACaddrs, 2);
    memcpy((void)* &(head[0]), (void*) ID, 2);
    //memcpy((void*) &(head[0]), ID, 2);
	memcpy((void*) &(head[2]), (void*) MACaddrs, 6);
    head[8]= transportLayer;
	head[9]= protocol;
	unsigned short dataLen = dataLength(protocol+1);
	memcpy((void*) &(head[10]), (void*) &dataLen, 2);
	free(MACaddrs);
	return head;
}

// Arma un paquete para el protocolo de inicio, que busca solo respuesta
char* dataprotocol00(){
    char* msg = malloc(dataLength(0));
    msg[0] = 1;
    return msg;
}

// Arma un paquete para el protocolo 0, con la bateria
char* dataprotocol0(){
    char* msg = malloc(dataLength(1));
    //char batt = batt_sensor();
    char k = (char) 1;
    msg[0] = k;
    char batt = batt_sensor();
    msg[1] = batt;
    int t = getTime();
    memcpy((void*) &(msg[2]), (void*) &t, 4);
    return msg;
}

char* dataprotocol1(){   
    char* msg = malloc(dataLength(2));
    char k = (char) 1;
    msg[0] = k;
    char batt = batt_sensor();
    msg[1] = batt;
    int t = getTime();
    memcpy((void*) &(msg[2]), (void*) &t, 4);
    char temp = THPC_sensor_temp();
    msg[6] = temp;
    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4);
    char hum = THPC_sensor_hum();
    msg[11] = hum;
    float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4);
    return msg;
}

char* dataprotocol2(){
    char* msg = malloc(dataLength(3));
    char batt = batt_sensor();
    msg[1] = batt;
    int t = getTime();
    memcpy((void*) &(msg[2]), (void*) &t, 4);
    char temp = THPC_sensor_temp();
    msg[6] = temp;
    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4);
    char hum = THPC_sensor_hum();
    msg[11] = hum;
    float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4);
    float* kpi = Acc_kpi();
    float rms =kpi[6];
    memcpy((void*) &(msg[16]), (void*) &rms, 4);
    return msg;
}

char* dataprotocol3(){
    
    char* msg = malloc(dataLength(4));
    char batt = batt_sensor();
    char k = (char) 1;
    msg[0] = k;
    msg[1] = batt;
    int t = getTime();
    memcpy((void*) &(msg[2]), (void*) &t, 4);
    char temp = THPC_sensor_temp();
    msg[6] = temp;
    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4);
    char hum = THPC_sensor_hum();
    msg[11] = hum;
    float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4);
    float* kpi = Acc_kpi();
    float rms =kpi[6];
    memcpy((void*) &(msg[16]), (void*) &rms, 4);
    float ampx =kpi[0];
    memcpy((void*) &(msg[20]), (void*) &ampx, 4);
    float frecx =kpi[1];
    memcpy((void*) &(msg[24]), (void*) &frecx, 4);
    float ampy =kpi[2];
    memcpy((void*) &(msg[28]), (void*) &ampy, 4);
    float frecy =kpi[3];
    memcpy((void*) &(msg[32]), (void*) &frecy, 4);
    float ampz =kpi[4];
    memcpy((void*) &(msg[36]), (void*) &ampz, 4);
    float frecz =kpi[5];
    memcpy((void*) &(msg[40]), (void*) &frecz, 4);
    
    return msg;
}

char* dataprotocol4(){
    char* msg = malloc(dataLength(5));
    char k = (char) 1;
    msg[0] = k;
    char batt = batt_sensor();
    msg[1] = batt;
    int t = getTime();
    memcpy((void*) &(msg[2]), (void*) &t, 4);
    char temp = THPC_sensor_temp();
    msg[6] = temp;
    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4);
    char hum = THPC_sensor_hum();
    msg[11] = hum;
    float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4);
    float* accx = acc_sensor_acc_x();
    memcpy((void*) &(msg[16]), (void*) &accx, 4);
    float* accy = acc_sensor_acc_y();
    memcpy((void*) &(msg[8016]), (void*) &accy, 4);
    float* accz = acc_sensor_acc_z();
    memcpy((void*) &(msg[16016]), (void*) &accz, 4);
    return msg;
}

char* mensaje (char protocol, char transportLayer){
    char* mnsj = malloc(messageLength(protocol+1));
	mnsj[messageLength(protocol+1)-1]= '\0';
	char* hdr = header(protocol, transportLayer);
	char* data;
	switch (protocol) {
		case 0:
            ESP_LOGI(TAG, "paso0");
			data = dataprotocol0();
			break;
		case 1:
            ESP_LOGI(TAG, "paso1");
			data = dataprotocol1();
			break;
		case 2:
            ESP_LOGI(TAG, "paso2");
			data = dataprotocol2();
			break;
		case 3:
            ESP_LOGI(TAG, "paso3");
			data = dataprotocol3();
			break;
        case 4:
            ESP_LOGI(TAG, "paso4");
			data = dataprotocol4();
			break;
		default:
            data = dataprotocol0();
			break;
	}
    ESP_LOGI(TAG, "%s", data);
	memcpy((void*) mnsj, (void*) hdr, 12);
	memcpy((void*) &(mnsj[12]), (void*) data, dataLength(protocol+1));
    //ESP_LOGE(TAG, "%s", mnsj[12]);
	free(hdr);
	free(data);
	return mnsj;
}

int TCP_send_frag(int sock, char status, char protocolo){
    //Parte el mensaje (payload) en trozos de 1000 btyes y los manda por separado, esperando un OK con cada trozo
    printf("Sending!\n");
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo) - 1;
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
            }
        }
    }
    //el último mensaje es solo un \0 para avisarle al server que terminamos
    int err = send(sock, "\0", 1, 0);

    free(payload);

    return err;
}

int UDP_send_frag(int sock, char status, char protocolo){
    //Parte el mensaje (payload) en trozos de 1000 btyes y los manda por separado, esperando un OK con cada trozo
    printf("Sending!\n");
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo) - 1;
    char rx_buffer[128];
    int packLen = 1000;

    struct sockaddr_in to;
    struct sockaddr from;
    int addrlen;

    for (int i = 0; i < payloadLen; i += packLen){
        // Generamos el siguiente trozo
        int size = fmin(packLen, payloadLen - i);
        char *pack = malloc(size);
        memcpy(pack, &(payload[i]), size);

        //Enviamos el trozo
        memset(&to, 0, sizeof(to));
        to.sin_family = AF_INET;
        to.sin_addr = inet_addr("192.168.4.1");
        to.sin_port = htons(5000);

        int err = sendto(sock, pack, size, 0,(struct sockaddr*)&to, sizeof(to));
        free(pack);
        if (err < 0){
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        }

        // wait for confirmation
        addrlen = sizeof(from);
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, &from, &addrlen);
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
            }
        }
    }
    //el último mensaje es solo un \0 para avisarle al server que terminamos
    int err = sendto(sock, "\0", 1, 0, (struct sockaddr*)&to, sizeof(to));

    free(payload);

    return err;
}

void tcp_client(int sock, char* buf, char* host_ip, int id_protocol, int tp_layer){
    
    ESP_LOGI(TAG, "sending data to %s by TCP", host_ip);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    while (1) {
        if(id_protocol == 4){
            int err = TCP_send_frag(sock, tp_layer ,id_protocol);
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
        int len = recv(sock, buf, sizeof(buf) - 1, 0);
        // Error occurred during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recv failed: errno %d", errno);
            break;
        }
        // Data received
        else {
            buf[len] = 0; // Null-terminate whatever we received and treat like a string
            ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
            ESP_LOGI(TAG, "%s", buf);
            esp_deep_sleep_start();
            break;
        }
    }
}

//void udp_client(int sock, char* buf, char* host_ip, struct sockaddr_in dest_addr, int id_protocol, int tp_layer){
void udp_client(int addr_fam, int ip_proto, char* buf, char* host_ip, struct sockaddr_in dest_addr, int id_protocol, int tp_layer){    
    int sock = socket(addr_fam, SOCK_DGRAM, ip_proto);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        break;
    }

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
    ESP_LOGI(TAG, "Socket udp ");

    
    ESP_LOGI(TAG, "sending data to %s by UDP", host_ip);
    while (1) {
        if(id_protocol == 4){
            int err = UDP_send_frag(sock, tp_layer ,id_protocol);
            if (err < 0){
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            }
        }else{
            char* msg = mensaje(id_protocol, tp_layer);
            int err = sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");
        }

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);
        int len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

        // Error occurred during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
            break;
        }
        // Data received
        else {
            rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
            ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
            ESP_LOGI(TAG, "%s", bud);
            if (strncmp(buf, "OK: ", 4) == 0) {
                ESP_LOGI(TAG, "Received expected message, reconnecting");
                break;
            }
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    if (sock != -1) {
        ESP_LOGE(TAG, "Shutting down udp socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }
    //vTaskDelete(NULL);
}

//********************CLIENTE************************
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
            udp_client(addr_family, ip_protocol, rx_buffer, host_ip, id_protocol, tp_layer);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
}
