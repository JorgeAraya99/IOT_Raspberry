import sqlite3 as sql
from sqlite3 import Error

database = "Data_Base/tarea1.db" 

table1 = '''
    CREATE TABLE IF NOT EXISTS Datos(
    ID_Time TEXT PRIMARY KEY,
    ID_Device INTEGER,
    MAC TEXT NOT NULL,
    Transport_Layer INTEGER,
    Protocol INTEGER NOT NULL,
    Leng TEXT,
    Data1 INTEGER,
    Data2 INTEGER,
    Data3 INTEGER,
    Data4 FLOAT,
    Data5 INTEGER,
    Data6 INTEGER,
    Data7 FLOAT,
    Data8 TEXT,
    Data9 TEXT,
    Data10 TEXT,
    Data11 FLOAT,
    Data12 FLOAT,
    Data13 FLOAT,
    Data14 FLOAT
);'''

table2 = '''
    CREATE TABLE IF NOT EXISTS Logs(
    ID_Time TEXT PRIMARY KEY,
    ID_Device INTEGER,
    Transport_Layer INTEGER,
    Protocol INTEGER,
    Timestamp TEXT
);'''

table3 = '''
    CREATE TABLE IF NOT EXISTS Configuracion(
    Protocol INT ,
    Transport_Layer INT,
    PRIMARY KEY (Protocol, Transport_Layer)
);'''

#funcion que crea la conexion a la database
def create_connection(db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
        return conn

    except Error as e:
        print(e)

    return conn

#funcion que crea tablas con comandos sql
def create_table(conn, create_table_sql):
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
    except Error as e:
        print(e)

#se comienza la conexion con la data base
conn = create_connection(database)

#se crean las tablas definidas al comienzo del archivo
if conn is not None:
    
    create_table(conn, table1)

    create_table(conn, table2)

    create_table(conn, table3)

    conn.close()
else:
    print("Error! No database found")

