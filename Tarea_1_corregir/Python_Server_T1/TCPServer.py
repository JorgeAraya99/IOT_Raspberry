import socket
from DatabaseWork import *
from Desempaquetamiento import *


def TCPServerFunc():
    HOST = "192.168.4.1"
    PORT = 5000

    database = "Data_Base/tarea1.db"

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    s.listen(5)
    print(f"Listening on {HOST}:{PORT}")

    #la variable first sirve para diferenciar entre configuracion inicial y el resto de mensajes
    first = True

    #contador para saber en que protocolo nos encontramos
    cycle = 0

    #al ready ser verdadero, se termina la conexión tcp
    ready = False

    while True:
        if ready: break

        print("Waiting for a connection")

        conn, addr = s.accept()

        print(f'Conectado por alguien ({addr[0]}) desde el puerto {addr[1]}')
        while True:

            try:
                data = conn.recv(1024)
                if data == b'':
                    break
            except ConnectionResetError:
                break
                        
            #se saca el primer elemento de la tabla Configuracion
            if first:    
                conectar = create_connection(database)
                datos = select_first_value(conectar)
                protocolo, layer = datos[0][0], datos[0][1]

                #se crea una respuesta
                respuesta = response(layer, protocolo)
                print("Entregando configuración al cliente. Protocolo " + str(protocolo))
                conn.send(respuesta)
                first = False

            if not first:
                protocolo = cycle
                respuesta = response(0, protocolo)
                print("Entregando configuración al cliente. Protocolo " + str(protocolo))
                conn.send(respuesta)
                
            #para el protocolo 4
            if cycle == 4:
                #funcion de desfragmentacion
                data = TCP_frag_recv(conn)

                if data == b'':
                    break
                
                
                #Parse del mensaje
                hdict, ddict = parseData(data)

                #Guardado en la base de datos
                conectar = create_connection(database)
                dataSave(cycle, hdict, ddict, conectar)
                logSave(hdict, ddict, conectar)
                

                #cambiar a udp    
                print("Mandando orden de cambio de layer")
                respuesta = response(1, 0)
                conn.send(respuesta)
                print("Se termina la conexión TCP")
                ready = True
                break

            #para el resto de los protocolos
            else:
                try:
                    data = conn.recv(1024)
                    if data == b'':
                        break

                except ConnectionResetError:
                    break
                print(f"Recibido {data}")

                
                #Parse del mensaje
                hdict, ddict = parseData(data)

                #Guardado en la base de datos
                conectar = create_connection(database)
                dataSave(cycle, hdict, ddict, conectar)
                logSave(hdict, ddict, conectar)  
                

                #Avanzar al siguiente protocolo
                cycle += 1
                respuesta = response(0, cycle)
                print("Cambiando de protocolo")
                conn.send(respuesta)
                    

        conn.close()
        print('Desconectado')





