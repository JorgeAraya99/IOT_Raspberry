import sqlite3 as sql
from sqlite3 import Error


#codigo para crear conexion a la data base
def create_connection(db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
        return conn

    except Error as e:
        print(e)

    return conn

#codigo para crear tablas con instrucciones sql
def create_table(conn, create_table_sql):
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
    except Error as e:
        print(e)


#funcion para extraer valores aleatorios de configuracion (no utilizada)
def select_random_value(conn):

    cur = conn.cursor()
    cur.execute("SELECT * FROM Configuracion ORDER BY RANDOM() LIMIT 1;")
    row = cur.fetchball()

    return row

#funcion para extraer el primer valor de la tabla configuracion
def select_first_value(conn):

    cur = conn.cursor()
    cur.execute("SELECT * FROM Configuracion LIMIT 1;")
    row = cur.fetchall()

    return row

# data Save para cada protocolo

def dataSave_0(header, data, conn):
    cur = conn.cursor()
    cur.execute('''insert into Datos (ID_Time, ID_Device, MAC, Transport_Layer, Protocol, Leng, Data1, Data2, Data3) values (?, ?, ?, ?, ?, ?, ?, ?, ?)''', (str(header["ID_Device"]) + str(data["Timestamp"]), header["ID_Device"], str(header["MAC"]), int(header["layer"]), int(header["protocol"]), str(header["length"]), data["Val"], data["Batt_level"], data["Timestamp"]))

def dataSave_1(header, data, conn):
    cur = conn.cursor()
    cur.execute('''insert into Datos (ID_Time, ID_Device, MAC, Transport_Layer, Protocol, Leng, Data1, Data2, Data3, Data4, Data5, Data6, Data7) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)''', (str(header["ID_Device"]) + str(data["Timestamp"]), header["ID_Device"], header["MAC"], header["layer"], header["protocol"], header["length"], data["Val"], data["Batt_level"], data["Timestamp"], data["Temp"], data["Pres"], data["Hum"], data["Co"]))

def dataSave_2(header, data, conn):
    cur = conn.cursor()
    cur.execute('''insert into Datos (ID_Time, ID_Device, MAC, Transport_Layer, Protocol, Leng, Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)''', (str(header["ID_Device"]) + str(data["Timestamp"]), header["ID_Device"], header["MAC"], header["layer"], header["protocol"], header["length"], data["Val"], data["Batt_level"], data["Timestamp"], data["Temp"], data["Pres"], data["Hum"], data["Co"], str(data["RMS"])))

def dataSave_3(header, data, conn):
    cur = conn.cursor()
    cur.execute('''insert into Datos (ID_Time, ID_Device, MAC, Transport_Layer, Protocol, Leng, Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11, Data12, Data13, Data14) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)''', (str(header["ID_Device"]) + str(data["Timestamp"]), header["ID_Device"], header["MAC"], header["layer"], header["protocol"], header["length"], data["Val"], data["Batt_level"], data["Timestamp"], data["Temp"], data["Pres"], data["Hum"], data["Co"], str(data["RMS"]), str(data["Amp_x"]), str(data["Frec_x"]), data["Amp_y"], data["Frec_y"], data["Amp_z"], data["Frec_z"]))

def dataSave_4(header, data, conn):
    cur = conn.cursor()
    cur.execute('''insert into Datos (ID_Time, ID_Device, MAC, Transport_Layer, Protocol, Leng, Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)''', (str(header["ID_Device"]) + str(data["Timestamp"]), header["ID_Device"], header["MAC"], header["layer"], header["protocol"], header["length"], data["Val"], data["Batt_level"], data["Timestamp"], data["Temp"], data["Pres"], data["Hum"], data["Co"], data["Acc_x"], data["Acc_y"], data["Acc_z"]))


#Guardar data segun protocolo

def dataSave(number, header, data, conn):
    if number == 0:
        dataSave_0(header, data, conn)
    elif number == 1:
        dataSave_1(header, data, conn)
    elif number == 2:
        dataSave_2(header, data, conn)
    elif number == 3:
        dataSave_1(header, data, conn)
    elif number ==4:
        dataSave_1(header, data, conn)
    else:
        print("wrong protocol")


#Guardar Logs

def logSave(header, data, conn):
    cur = conn.cursor()
    cur.execute('''insert into Logs (ID_Time, ID_Device, Transport_Layer, Protocol, Timestamp) values (?, ?, ?, ?, ?) ''', (str(header["ID_Device"]) + str(data["Timestamp"]), header["ID_Device"], header["layer"], header["protocol"], data["Timestamp"]))