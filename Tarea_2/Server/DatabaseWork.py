import sqlite3 as sql
from sqlite3 import Error


#Funciones de guardado de data para cada protocolo
def DataSave_1(header, dict, db_file, conf):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    cur = conn.cursor()

    cur.execute(''' INSERT INTO Log (Id_device, Status_report, Protocol_report, Battery_level, Conf_peripheral, Time_client, configuration_Id_device) VALUES (?, ?, ?, ?, ?) ''', (header["ID_device"], header["Status"], header["Protocol"], dict["Batt_level"], conf,dict["Time"], header["ID_device"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_1 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.commit()

    conn.close()

def DataSave_2(header, dict, db_file, conf):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    cur = conn.cursor()

    cur.execute(''' INSERT INTO Log (Id_device, Status_report, Protocol_report, Battery_level, Conf_peripheral, Time_client, configuration_Id_device) VALUES (?, ?, ?, ?, ?) ''', (header["ID_device"], header["Status"], header["Protocol"], dict["Batt_level"], conf,dict["Time"], header["ID_device"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO) VALUES (?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.commit()

    conn.close()

def DataSave_3(header, dict, db_file, conf):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    cur = conn.cursor()

    cur.execute(''' INSERT INTO Log (Id_device, Status_report, Protocol_report, Battery_level, Conf_peripheral, Time_client, configuration_Id_device) VALUES (?, ?, ?, ?, ?) ''', (header["ID_device"], header["Status"], header["Protocol"], dict["Batt_level"], conf,dict["Time"], header["ID_device"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO, RMS) VALUES (?, ?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"], dict["RMS"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.commit()

    conn.close()

def DataSave_4(header, dict, db_file, conf):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    cur = conn.cursor()

    cur.execute(''' INSERT INTO Log (Id_device, Status_report, Protocol_report, Battery_level, Conf_peripheral, Time_client, configuration_Id_device) VALUES (?, ?, ?, ?, ?) ''', (header["ID_device"], header["Status"], header["Protocol"], dict["Batt_level"], conf,dict["Time"], header["ID_device"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO, RMS, Amp_x, Freq_x, Amp_y, Freq_y, Amp_z, Freq_z) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"], dict["RMS"], dict["Amp_x"] , dict["Frec_x"] , dict["Amp_y"] , dict["Frec_y"] , dict["Amp_z"] , dict["Frec_z"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.commit()

    conn.close()

def DataSave_5(header, dict, db_file, conf):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    cur = conn.cursor()

    cur.execute(''' INSERT INTO Log (Id_device, Status_report, Protocol_report, Battery_level, Conf_peripheral, Time_client, configuration_Id_device) VALUES (?, ?, ?, ?, ?) ''', (header["ID_device"], header["Status"], header["Protocol"], dict["Batt_level"], conf,dict["Time"], header["ID_device"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO) VALUES (?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"]))

    conn.commit()

    cur.execute(''' INSERT INTO Datos_2 (Id_device, Time_client, Racc_x, Racc_y, Racc_z, Rgyr_x, Rgyr_y, Rgyr_z) VALUES (?, ?, ?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["RAcc_x"], dict["RAcc_y"], dict["RAcc_z"], dict["Rgyr_x"], dict["Rgyr_y"], dict["Rgyr_z"]))

    conn.commit()

    conn.close()


#funcion que guarda en data y log segun protocolo
def DataSave(header, dict, protocol, db_file, conf): # que es conf
    if protocol == 1:
        DataSave_1(header, dict, db_file, conf)
    elif protocol == 2:
        DataSave_2(header, dict, db_file, conf)
    elif protocol == 3:
        DataSave_3(header, dict, db_file, conf)
    elif protocol == 4:
        DataSave_4(header, dict, db_file, conf)
    elif protocol == 5:
        DataSave_5(header, dict, db_file, conf)
    else:
        print("protocolo invalido")


#funcion que haga query de configuracion
def getConfig(ID, db_file):
    conn = None
    
    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)

    cur = conn.cursor()

    order = "SELECT * FROM Configuration WHERE Id_device = " + str(ID) + ";"

    cur.execute(order)
    

    config = cur.fetchall()

    conn.close()

    return config

def getFinished(db_file):
    conn = None
    
    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    order = "SELECT STAT FROM Finished WHERE "
    
    conn.cursor().excecute(order)
    
    config = cur.fetchall()
    
    conn.close()
    
    Val = config[0][0]
    
    return Val
