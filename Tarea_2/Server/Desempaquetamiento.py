from struct import pack, unpack
import traceback
from DatabaseWork import *

def response():
    OK=1
    return pack("<B", OK)

def parseData(packet):
    header = packet[:12]
    data = packet[12:]
    headerD = headerDict(header)
    dataD = dataDict(headerD["Protocol"], data)
    if dataD is not None:
        print("saved")
    
    return None if dataD is None else headerD, dataD


def protUnpack(protocol, data):
    protocol_unpack = ["<2ci", "<2cicfcf", "<2cicfc2f", "<2cicfc8f", "<2cicicf2000f2000f2000f"] #revisar fs despues
    return unpack(protocol_unpack[protocol], data)

def headerDict(header):
    ID, M1, M2, M3, M4, M5, M6, protocol, status, leng = unpack("<algo", header)

    MAC = ".".join([hex(x)[2:] for x in [M1, M2, M3, M4, M5, M6]])

    return {"ID_device" : ID, "MAC":MAC, "Status":status, "Leng":leng, "Protocol":protocol}

def dataDict(protocol, data):
    if protocol not in [1, 2, 3, 4, 5]:
        print("Error: protocol does not exist")
        return None
    
    def protFunc(protocol, keys):
        def p(data):
            unp = protUnpack(protocol, data)
            return {key:val for (key, val) in zip(keys, unp)}

    #valores de cada protocolo
    p1 = ["Val", "Batt_level"]
    p2 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co"]
    p3 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "RMS"]
    p4 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "RMS", "Amp_x", "Frec_x", "Amp_y", "Frec_y", "Amp_z", "Frec_z"]
    p5 = ["Val", "Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "RAcc_x", "RAcc_y", "RAcc_z", "Rgyr_x", "Rgyr_y", "Rgyr_z"]
    p = [p1, p2, p3, p4, p5]

    try:
        return protFunc(protocol, p[protocol])(data)
    except Exception:
        print("Data unpacking Error:", traceback.format_exc())
        return None

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
    return (doc, addr)
