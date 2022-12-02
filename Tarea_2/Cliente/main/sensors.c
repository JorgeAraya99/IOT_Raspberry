float floatrand(float min, float max){     
    return min + (float)rand()/(float)(RAND_MAX/(max-min)); 
    }

float* acc(){
    float* arr = malloc(2000* sizeof(float));     
    for (int i =0; i <2000; i++){         
        arr[i] = floatrand(-16, 16);
    }     
    return arr;
}

float* rgyr(){
    float* arr = malloc(2000* sizeof(float));     
    for (int i =0; i <2000; i++){         
        arr[i] = floatrand(-1000, 1000);
    }     
    return arr;
}

float* thpc(){
    char pres= floatrand(1000,1200);
    float CO2=floatrand(30,200);
    float* arr = malloc(2* sizeof(float));
    arr[0]=pres;
    arr[3]=CO2;
    return arr;
}

char THPC_sensor_temp(){
    int temp = (int) 5+(rand()%25);
    return temp;
}

char THPC_sensor_hum(){
    int hum = (int) 30+(rand()%50);
    return hum;
}

float THPC_sensor_pres(){
    int pres = (int) 1000 + (rand()%200);
    return pres;
}

float THPC_sensor_co(){
    float CO2= floatrand(30,200);
    return CO2;
}

char batt_sensor(){
    u_int8 bat = (u_int8) 1+(rand()%99);
    return bat;
}

float* Acc_kpi(){
    float ampx= floatrand(59,1200)/10000;
    float frecx= (floatrand(290,310))/10;
    float ampy= (floatrand(41,1100))/10000;
    float frecy= (floatrand(590,610))/10;
    float ampz= floatrand(8,150)/1000;
    float frecz= floatrand(890,910)/10;
    float RMS= sqrt(pow(ampx,2) + pow(ampy,2)+ pow(ampz,2));
    float* arr = malloc(7* sizeof(float));
    arr[0]=ampx;
    arr[1]=frecx;
    arr[2]=ampy;
    arr[3]=frecy;
    arr[4]=ampz;
    arr[5]=frecz;
    arr[6]=RMS;
    /*
    for(int i = 0; i < 7; i++){
        printf("ampx = %f\n", arr[i]);
    }
    */
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