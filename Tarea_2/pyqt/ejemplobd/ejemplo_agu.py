import sqlite3 as sql
from sqlite3 import Error

ejemplo = """
    CREATE TABLE IF NOT EXISTS EJEMPLO(
    VAL1 INT PRIMARY KEY NOT NULL,
    VAL2 INT,
    VAL3 INT
);"""

database = "ejemplo.db"

def create_table(table, db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
        conn.cursor().execute(table)
    except Error as e:
        print(e)

    conn.close()

def getConfig(ID, db_file):
    conn = None
    
    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)

    cur = conn.cursor()

    order = "SELECT * FROM EJEMPLO WHERE VAL1 = " + str(ID) +";"

    cur.execute(order)

    config = cur.fetchall()

    conn.close()

    return config

def DataSave(db_file):
    conn = None

    try:
        conn = sql.connect(db_file)
    except Error as e:
        print(e)
    
    cur = conn.cursor()

    cur.execute(''' INSERT INTO EJEMPLO (VAL1, VAL2, VAL3) VALUES (69, 420, 2077) ''')

    conn.close()

create_table(ejemplo, database)
DataSave(database)
row = getConfig(69, database)
print(row)