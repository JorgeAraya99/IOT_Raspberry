import sqlite3 as sql
from sqlite3 import Error


#Funciones de guardado de data para cada protocolo
def DataSave_1(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Datos_1 () VALUES () ''', (header[1], dict[1]))

    conn.execute(''' INSERT INTO Datos_2 () VALUES () ''', (header[1], dict[1]))

    conn.close()

def DataSave_2(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Datos_1 () VALUES () ''', (header[1], dict[1]))

    conn.execute(''' INSERT INTO Datos_2 () VALUES () ''', (header[1], dict[1]))

    conn.close()

def DataSave_3(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Datos_1 () VALUES () ''', (header[1], dict[1]))

    conn.execute(''' INSERT INTO Datos_2 () VALUES () ''', (header[1], dict[1]))

    conn.close()

def DataSave_4(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Datos_1 () VALUES () ''', (header[1], dict[1]))

    conn.execute(''' INSERT INTO Datos_2 () VALUES () ''', (header[1], dict[1]))

    conn.close()

def DataSave_5(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Datos_1 () VALUES () ''', (header[1], dict[1]))

    conn.execute(''' INSERT INTO Datos_2 () VALUES () ''', (header[1], dict[1]))

    conn.close()


#funcion que guarda en data segun protocolo
def DataSave(header, dict, protocol, db_file):
    match protocol:
        case 1:
            DataSave_1(header, dict, db_file)
        case 2:
            DataSave_2(header, dict, db_file)
        case 3:
            DataSave_3(header, dict, db_file)
        case 4:
            DataSave_4(header, dict, db_file)
        case 5:
            DataSave_5(header, dict, db_file)
        case _:
            print("protocolo invalido")

#funcion que guarda en Log
def LogSave(header, dict, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    conn.execute(''' INSERT INTO Log () VALUES () ''', (header[1], dict[1]))

    conn.close()

#funcion que haga query de configuracion
def getConfig():
    pass