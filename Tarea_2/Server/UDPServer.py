import socket
import Desempaquetamiento
import DatabaseWork

def UDPServerFunc(host, port, protocol, conf_p):
    HOST = host
    PORT = port

    PROTOCOL = protocol
    CONF_P = conf_p

    database = ""

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    s.bind(HOST, PORT)

    print(f"Listening on {HOST}:{PORT}")

    ready = False

    while True:
        if ready: break
        print("Waiting for a connection")

        while True:
            
            if PROTOCOL == 5:
                data = Desempaquetamiento.UDP_frag_recv(s)
                if data == b'':
                    break

                hdict, ddict = Desempaquetamiento.parseData(data)

                DatabaseWork.DataSave(hdict, ddict, PROTOCOL, database, CONF_P) #revisar que mandar al guardado

            else:
                try:
                    data, addr = s.recvfrom(1024)
                    if data == b'':
                        break
                except ConnectionError:
                    print("Connection error")
                    break
                
                hdict, ddict = Desempaquetamiento.parseData(data)

                DatabaseWork.DataSave(hdict, ddict, PROTOCOL, database, CONF_P) #revisar que mandar al guardado

        