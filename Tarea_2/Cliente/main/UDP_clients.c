int UDP_send_frag(int sock, char status, char protocolo, struct sockaddr_in dest_addr, char* buf){
    //Parte el mensaje (payload) en trozos de 1000 btyes y los manda por separado, esperando un OK con cada trozo
    printf("Sending!\n");
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo+1) - 1;
    char rx_buffer[128];
    int packLen = 1000;

    for (int i = 0; i < payloadLen; i += packLen){
        // Generamos el siguiente trozo
        int size = fmin(packLen, payloadLen - i);
        char *pack = malloc(size);
        memcpy(pack, &(payload[i]), size);

        //Enviamos el trozo

        int err = sendto(sock, pack, size, 0,(struct sockaddr*)&dest_addr, sizeof(dest_addr));
        free(pack);
        if (err < 0){
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        }

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);
        int len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
        // Error occurred during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
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
    int err = sendto(sock, "\0", 1, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

    free(payload);

    return err;
}

//void udp_client(int sock, char* buf, char* host_ip, struct sockaddr_in dest_addr, int id_protocol, int tp_layer){
void udp_client(int addr_fam, int ip_proto, char* buf, char* host_ip, struct sockaddr_in dest_addr, int id_protocol, int tp_layer){    
    int sock = socket(addr_fam, SOCK_DGRAM, ip_proto);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return;
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
            int err = UDP_send_frag(sock, tp_layer ,id_protocol, dest_addr, buf);
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
            buf[len] = 0; // Null-terminate whatever we received and treat like a string
            ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
            ESP_LOGI(TAG, "%s", buf);
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