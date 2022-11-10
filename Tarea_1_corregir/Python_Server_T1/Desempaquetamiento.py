from struct import unpack, pack
import traceback
from DatabaseWork import *

#codigo de respuesta al cliente, en formato little endian
def response(layer:int, protocol:int):
    OK = 1
    return  pack("<BBB", OK, layer, protocol)


#funcion que recibe la data del cliente y la interpreta. actualmente no funciona.
#el error esta en headerDict y dataDict
def parseData(packet):
    header = packet[:12]
    data = packet[12:]
    headerD = headerDict(header)
    dataD = dataDict(data)
    
    if dataD is not None:
        #guardar datos en tabla
        print("saved")

    #return None if dataD is None else {**headerD, **dataD}
    return None if dataD is None else headerD, dataD

#funcion que le asigna el unpack correcto a cada protocolo. se encuentra incompleta por causa de los valores necesarios para unpack.
def protUnpack(protocol, data):
    protocol_unpack = ["<2ci", "<2cicfcf", "<2cicfc2f", "<2cicfc8f", "<2cicicf2000f2000f2000f"] #revisar fs despues
    return unpack(protocol_unpack[protocol], data)

#funcion que interpreta y separa valores del header. se reciben 8B (2 de ID y 6 de MAC), cc ( chars layer y protocol) y H (u short length)
#aunque se deberían recibir 12 bytes, no se acepta el unpack como tal
def headerDict(data):
    print(data)
    #se separan los valores
    ID_Device, M1, M2, M3, M4, M5, M6, layer,  protocol, leng_msg = unpack("<h6B2cH", data) #ver si funcionan o estan mal puestos los char
    
    #se junta mac
    MAC = ".".join([hex(x)[2:] for x in [M1, M2, M3, M4, M5, M6]])

    return {"ID_Device":ID_Device, "MAC":MAC, "layer":layer, "protocol":protocol, "length":leng_msg}

#funcion que interpreta y separa los valores de data. Debería funcionar, pero no se tienen los formatos correctos en la funcion protUnpack
def dataDict(protocol, data):
    if protocol not in [0, 1 , 2, 3, 4]:
        print("Error: protocol does not exist")
        return None
    def protFunc(protocol, keys):
        def p(data):
            unp = protUnpack(protocol, data)
            return {key:val for (key, val) in zip(keys, unp)}

    #valores de cada protocolo
    p0 = ["Val", "Batt_level", "Timestamp"]
    p1 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co"]
    p2 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "RMS"]
    p3 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "RMS", "Amp_x", "Frec_x", "Amp_y", "Frec_y", "Amp_z", "Frec_z"]
    p4 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "Acc_x", "Acc_y", "Acc_z"]
    p = [p0, p1, p2, p3, p4]

    try:
        return protFunc(protocol, p[protocol])(data)
    except Exception:
        print("Data unpacking Error:", traceback.format_exc())
        return None

#funcion desfragmentar
#se eliminó el timeout porque la esp no conseguía mantener la conexión y daba error
def TCP_frag_recv(conn):
    doc = b""
    i = 0
    while True:
        try:
            #conn.settimeout(5)
            data = conn.recv(1024)
            print("recibido paquete " + str(i))
            i += 1
            if data == b'\0':
                break
            else:
                doc += data
        #except TimeoutError:
        #    conn.send(b'\0')
        #    raise
        except Exception:
            conn.send(b'\0')
            raise
        conn.send(b'\1')
    return doc

def UDP_frag_recv(s):
    doc = b""
    addr = None
    while True:
        try:
            data, addr = s.recvfrom(1024)
            if data == b'\0':
                break
            else:
                doc += data
        #except TimeoutError:
        #    raise
        except Exception:
            raise
        s.sendto(b'\1', addr)
    return (doc, addr)