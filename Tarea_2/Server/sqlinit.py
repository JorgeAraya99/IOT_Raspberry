import sqlite3 as sql
from sqlite3 import Error

#Local de base de datos en raspberry
database = "Data_Base/tarea2.db"

#Query para crear las tablas
Log='''
    CREATE TABLE IF NOT EXISTS Log(
    Id_device INT NOT NULL,
    Status_report INT,
    Battery_Level INT,
    Conf_periphereal INT,
    Time_client DATETIME,
    Time_server DATETIME DEFAULT CURRENT_TIMESTAMP PRIMARY KEY,
    configuration_Id_device INT,
    FOREIGN KEY(Id_device) REFERENCES Configuration(Id_device)
);'''

Configuration='''
    CREATE TABLE IF NOT EXISTS Configuration(
    Id_device INT PRIMARY KEY NOT NULL,
    Status_conf INT,
    Protocol_conf INT,
    Acc_sampling INT,
    Acc_sensibility INT,
    Gyro_sensibility INT,
    BME688_sampling INT,
    Discontinuos_time INT,
    TCP_PORT INT,
    UDP_PORT INT,
    Host_ip_addr INT,
    Ssid VARCHAR(45),
    Pass VARCHAR(45)
);'''

Data_1='''
    CREATE TABLE IF NOT EXISTS Data_1(
    Id_device INT NOT NULL,
    Temperature INT,
    Press INT,
    Hum INT,
    Co FLOAT,
    RMS FLOAT,
    Amp_x FLOAT,
    Freq_x FLOAT,
    Amp_y FLOAT,
    Freq_y FLOAT,
    Amp_z FLOAT,
    Freq_z FLOAT,
    Time_client DATETIME,
    Time_server DATETIME DEFAULT CURRENT_TIMESTAMP PRIMARY KEY,
    FOREIGN KEY(Id_device) REFERENCES Log(Id_device)
);'''

Data_2='''
    CREATE TABLE IF NOT EXISTS Data_2(
    Id_device INT NOT NULL,
    Racc_x FLOAT,
    Racc_y FLOAT,
    Racc_z FLOAT,
    Rgyr_x FLOAT,
    Rgyr_y FLOAT,
    Rgyr_z FLOAT,
    Time_client DATETIME,
    Time_server DATETIME DEFAULT CURRENT_TIMESTAMP PRIMARY KEY,
    FOREIGN KEY(Id_device) REFERENCES Log(Id_device)
);'''

Finished = '''
    CREATE TABLE IF NOT EXISTS Finished(
    STAT INT NOT NULL PRIMARY KEY
);'''

#funcion que crea las tablas
def create_table(table, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
        conn.cursor().execute(table)
    except Error as e:
        print(e)

    conn.close()
    
def insert_Finished(db_file):
    conn = None
    order ="INSERT INTO Finished (STAT) VALUES (2);"
    
    try:
        conn = sql.connect(db_file)
        conn.cursor().execute(order)
        conn.commit()
    except Error as e:
        print(e)
        
    conn.close()

if __name__ == "__main__":
    create_table(Configuration, database)
    create_table(Log, database)
    create_table(Data_1, database)
    create_table(Data_2, database)
    create_table(Finished, database)
    insert_Finished(database)
