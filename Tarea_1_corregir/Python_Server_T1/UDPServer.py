import socket
from DatabaseWork import *
from Desempaquetamiento import *


def UDPServerFunc():
    HOST = "192.168.4.1"
    PORT = 5500
    database = "Data_Base/tarea1.db"

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    s.bind((HOST, PORT))
    #s.listen(5)
    print(f"Listening on {HOST}:{PORT}")

    #variable que representa cuantos mensajes se mandaron en el protocolo actual
    #se tiene un max de 15 mensajes por protocolo
    smol_cycle = 1

    #variable que representa el protocolo actual
    big_cycle = 0

    #al ready ser verdadero, se termina la conexión tcp
    ready = False

    while True:
        if ready: break

        print("Waiting for a connection")


        #conn, addr = s.accept()
        #print(f'Conectado por alguien ({addr[0]}) desde el puerto {addr[1]}')
        
        while True:
            try:
                data, addr = s.recvfrom(1024)
                if data == b'':
                    break
            except ConnectionResetError:
                break
            
            #se envia protocolo a usar al cliente
            protocolo = big_cycle
            respuesta_inicial = response(1, protocolo)
            print("Entregando configuración al cliente. Protocolo " + str(protocolo))
            #s.sendto(respuesta_inicial, addr)

            #para el protocolo 4
            if big_cycle == 4:
                #funcion de desfragmentacion
                data = UDP_frag_recv(s)

                if data == b'':
                    break

                
                #parse del mensaje
                hdict, ddict = parseData(data)

                #Guardado en la base de datos
                conectar = create_connection(database)
                dataSave(big_cycle, hdict, ddict, conectar)
                logSave(hdict, ddict, conectar)
                

                if smol_cycle == 15:
                    #terminar conexión
                    print("Se termina la conexión")
                    respuesta_final = response(2, 0)
                    #s.sendto(respuesta_final, addr)
                    ready = True
                    break
                else:
                    #se mantiene el protocolo

                    smol_cycle += 1
                    respuesta = response(1, big_cycle)
                    #s.sendto(respuesta, addr)
                    break

            else:
                try:
                    data, addr = s.recvfrom(1024)
                    if data == b'':
                        break
                except ConnectionResetError:
                    break
                print(f"Recibido {data}")

                
                #Parse del mensaje
                hdict, ddict = parseData(data)

                #Guardado en la base de datos
                conectar = create_connection(database)
                dataSave(big_cycle, hdict, ddict, conectar)
                logSave(hdict, ddict, conectar)
                
                
                if smol_cycle == 15:
                    #cambio de protocolo

                    print("Cambiando de protocolo")
                    big_cycle +=1
                    smol_cycle = 1
                    respuesta = response(1, big_cycle)
                    #s.sendto(respuesta, addr)
                else:
                    #se mantiene el protocolo
                    
                    smol_cycle += 1
                    respuesta = response(1, big_cycle)
                    #s.sendto(respuesta, addr)

        s.close()
        print('Desconectado')
