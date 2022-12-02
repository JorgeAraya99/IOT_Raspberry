int TCP_send_frag(int sock, char status, char protocolo){
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