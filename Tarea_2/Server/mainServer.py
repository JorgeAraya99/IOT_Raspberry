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

    lista = config[0]

    #Prender server correspondiente
    status = lista[1]
    protocol = lista[2]
    acc_sam = lista[3]
    acc_sens = lista[4]
    gyro_sens = lista[5]
    BME688 = lista[6]
    discont_time = lista[7]
    tcp_port = lista[8]
    udp_port = lista[9]
    host = lista[10]
    conf_p = str(acc_sam) + str(acc_sens) +str(gyro_sens) + str(BME688)
    
    if status == 21 or status ==22:
        TCPServer.TCPServerFunc(host, tcp_port, protocol, conf_p)
    elif status ==23:
        UDPServer.UDPServerFunc(host, udp_port, protocol, conf_p)
    elif status == 30 or status == 31:
        BLEServer.BLEServerFunc(host, tcp_port, protocol, conf_p)
    else:
        print("Invaid status input")