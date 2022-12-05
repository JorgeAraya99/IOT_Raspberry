import socket
import Desempaquetamiento
import DatabaseWork

def TCPServerFunc(host, port, protocol, conf_p):
    HOST = host
    PORT = port

    PROTOCOL = protocol
    CONF_P = conf_p

    database = "Data_Base/tarea2.db"

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST,PORT))
    s.listen(5)
    print(f"Listening on {HOST}:{PORT}")

    ready = False

    while True:
        if ready: break

        print("Waiting for a connection")

        conn, addr = s.accept() 

        print(f"Connected to ({addr[0]}) on port ({addr[1]})")

        while True:
            Val = DatabaseWork.getFinished(database)
            if Val == 1:
                ready = not ready
                
            if ready: break
            
            if PROTOCOL == 5:
                data = Desempaquetamiento.TCP_frag_recv(s)

                if data == b'':
                    break

                hdict, ddict = Desempaquetamiento.parseData(data)

                DatabaseWork.DataSave(hdict, ddict, PROTOCOL, database, CONF_P)

                respuesta = Desempaquetamiento.response()
                conn.send(respuesta)
            else:
                try:
                    data = conn.recv(1024)
                    if data == b'':
                        break
                except ConnectionError:
                    print("Connection Error")
                    break
                
                hdict, ddict = Desempaquetamiento.parseData(data)

                DatabaseWork.DataSave(hdict, ddict, PROTOCOL, database, CONF_P)

                respuesta = Desempaquetamiento.response()
                conn.send(respuesta)
