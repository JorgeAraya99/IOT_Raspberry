

void tcp_client(int sock, char* buf, char* host_ip){
    
    ESP_LOGI(TAG, "sending data to %s by TCP", host_ip);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    while (1) {
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