import DatabaseWork
import TCPServer
import UDPServer
import BLEServer
#from Desempaquetamiento import *

#server principal para conexion status 0



#generar config inicial a partir de PyQt



#Manadr config inicial a la ESP


def mainSever(ID):

    condition = str(input())
    if condition== "n":
        return
    
    #revisar CI
    config = DatabaseWork.getConfig(ID)

    #Prender server correspondiente
    status = config
    host = config
    port = config
    protocol = config
    conf_p = config
    
    if status == 21 or status ==22:
        TCPServer.TCPServerFunc(host, port, protocol, conf_p)
    elif status ==23:
        UDPServer.UDPServerFunc(host, port, protocol, conf_p)
    elif status == 30 or status == 31:
        BLEServer.BLEServerFunc(host, port, protocol, conf_p)
    else:
        print("Invaid status input")