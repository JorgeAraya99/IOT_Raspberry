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
char* header(char protocol, char status){
	char* head = malloc(12);

    short ID = 1;
    esp_efuse_mac_get_default(MACaddrs);
    uint8_t* MACaddrs = malloc(6);
    //memcpy((void*) &(head[0]), (void*) MACaddrs, 2);
    memcpy((void*) &(head[0]), (void*) &ID, 2);
    //memcpy((void*) &(head[0]), ID, 2);
	memcpy((void*) &(head[2]), (void*) MACaddrs, 6);
    head[8]= protocol;
	head[9]= status;
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