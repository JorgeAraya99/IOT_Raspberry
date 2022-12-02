import sqlite3 as sql
from sqlite3 import Error


#Funciones de guardado de data para cada protocolo
def DataSave_1(header, dict, db_file, conf):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log (Id_device, Status_report, Battery_level, Time_client, Conf_peripheral) VALUES (?, ?, ?, ?, ?) ''', (header["ID_device"], header["Status"], dict["Batt_level"], dict["Time"]))

    conn.execute(''' INSERT INTO Datos_1 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.close()

def DataSave_2(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log (Id_device, Status_report, Battery_level, Time_client) VALUES (?, ?, ?) ''', (header["ID_device"], header["Status"], dict["Batt_level"], dict["Time"]))

    conn.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO) VALUES (?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"]))

    conn.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.close()

def DataSave_3(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log (Id_device, Status_report, Battery_level, Time_client) VALUES (?, ?, ?) ''', (header["ID_device"], header["Status"], dict["Batt_level"], dict["Time"]))

    conn.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO, RMS) VALUES (?, ?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"], dict["RMS"]))

    conn.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.close()

def DataSave_4(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log (Id_device, Status_report, Battery_level, Time_client) VALUES (?, ?, ?) ''', (header["ID_device"], header["Status"], dict["Batt_level"], dict["Time"]))

    conn.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO, RMS, Amp_x, Freq_x, Amp_y, Freq_y, Amp_z, Freq_z) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"], dict["RMS"], dict["Ampx"] , dict["Freqx"] , dict["Ampy"] , dict["Freqy"] , dict["Ampz"] , dict["Freqz"]))

    conn.execute(''' INSERT INTO Datos_2 (Id_device, Time_client) VALUES (?, ?) ''', (header["Id_device"], dict["Time"]))

    conn.close()

def DataSave_5(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log (Id_device, Status_report, Battery_level, Time_client) VALUES (?, ?, ?) ''', (header["ID_device"], header["Status"], dict["Batt_level"], dict["Time"]))

    conn.execute(''' INSERT INTO Datos_1 (Id_device, Time_client, Temperature, Press, Hum, CO) VALUES (?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Temp"], dict["Press"], dict["Hum"], dict["CO"]))

    conn.execute(''' INSERT INTO Datos_2 (Id_device, Time_client, Racc_x, Racc_y, Racc_z, Rgyr_x, Rgyr_y, Rgyr_z) VALUES (?, ?, ?, ?, ?, ?, ?, ?) ''', (header["Id_device"], dict["Time"], dict["Raccx"], dict["Raccy"], dict["Raccy"], dict["Rgyrx"], dict["Rgyry"], dict["Rgyrz"]))

    conn.close()


#funcion que guarda en data segun protocolo
def DataSave(header, dict, protocol, db_file, conf):
    match protocol:
        case 1:
            DataSave_1(header, dict, db_file, conf)
        case 2:
            DataSave_2(header, dict, db_file, conf)
        case 3:
            DataSave_3(header, dict, db_file, conf)
        case 4:
            DataSave_4(header, dict, db_file, conf)
        case 5:
            DataSave_5(header, dict, db_file, conf)
        case _:
            print("protocolo invalido")

#funcion que guarda en Log

"""
def LogSave(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log () VALUES () ''', (header[1], dict[1]))

    conn.close()
"""
#funcion que haga query de configuracion
def getConfig():
    pass