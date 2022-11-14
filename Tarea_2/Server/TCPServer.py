import socket
from DatabaseWork import *
#from Desempaquetamiento import *

def TCPServerFunc():
    HOST = ""
    PORT = 1

    database = "Data_Base/tarea2.db"

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST,PORT))
    s.listen(5)
    print(f"Listening on {HOST}:{PORT}")

    while True:
        pass