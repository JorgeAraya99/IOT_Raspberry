from socket import timeout
import numpy
from findAddresses import findAddresses
import pygatt
from struct import pack
from ex import Ui_Dialog
from PyQt5 import QtCore, QtGui,  QtWidgets #QtQuickWidgets,
import pyqtgraph as pg
import time
import logging
import sqlite3 as sql
from sqlite3 import Error
import tcp_BD

database = "Data_Base/tarea2.db"

class GUIController:
    def __init__(self, parent):
        self.ui = Ui_Dialog()
        self.parent = parent
        self.macs = []
        self.ID = [1,2]
        self.actual =0
        self.UUIDs = []
        self.servers = []
        self.plot = None
        self.grph = None
        self.macindx = None

        self.adapter = pygatt.GATTToolBackend() ##pygatt
        print()
        
    def actualizarMacs(self):
        # actualiza la lista de dispositivos con bluetooth disponibles: boton search_esp32
        adrs = findAddresses()
        self.macs = adrs[1]
        self.UUIDs = adrs[2]
        self.ui.esp32_select.clear()
        self.ui.esp32_select.addItems(adrs[0])
        
        #print()

    def setSignals(self):
        self.ui.search_esp32.clicked.connect(self.actualizarMacs)
        self.ui.config_btn.clicked.connect(self.configSetup)

    def conectarMac(self):
        # se conecta mediante BLE a un dispostivo disponible
        indx = self.ui.esp32_select.currentIndex()
        self.macindx = indx
        ##pygatt
        logging.basicConfig()
        logging.getLogger('pygatt').setLevel(logging.DEBUG)
        qty = 0
        while(qty<100):
            try:
                self.adapter.start()
                device = self.adapter.connect(self.macs[indx],timeout=2.0)
                print('Se conecto!')
                # La siguiente linea es para ver todas las caracteristicas disponibles
                characteristics = device.discover_characteristics()
                for i in characteristics.keys():
                    print('Caracteristicas: '+str(i))#list(characteristics.keys())))
                time.sleep(1)
                qty = 100
            except pygatt.exceptions.NotConnectedError:
                qty += 1
                print("Se han fallado: {qty} intentos" )
                print("Not connected")
                time.sleep(1)
            finally:
                self.adapter.stop()
        print("Termino de test de conexión")

    def getConfigParams(self):
        params = []
        confMode= self.ui.config_mode_select.currentIndex()
        statusSelect= self.ui.status_select.currentIndex()
        #conectselect
        print(str(self.ui.config_mode_select.currentIndex() =="Configuracion por Bluetooth"))
        print(str(self.ui.config_mode_select.currentIndex()))
        #Reisar como usar conf y set status de forma separada
        if confMode == 0:
            params.append(0)
        if confMode == 1:
            params.append(20)

        if statusSelect == 0:
            params.append(21)


        if statusSelect == 1:
            params.append(22)
            
        if statusSelect == 2:
            params.append(23)
            
        if statusSelect == 3:
            params.append(30)
            
        if statusSelect == 4:
            params.append(31)
        
        #agregar id protocol

        params.append(self.ui.id_protocol_select.currentIndex())

        TCP = int(self.ui.port_tcp_field.toPlainText())
        UDP = int(self.ui.port_udp_field.toPlainText())
        IP = self.ui.host_ip_addr_field.toPlainText()
        SSID = self.ui.ssid_field.toPlainText()
        PASS = self.ui.pass_field.toPlainText()
        params.append(int(self.ui.acc_samp_field.toPlainText()))
        params.append(int(self.ui.acc_sens_field.toPlainText()))
        params.append(int(self.ui.gyro_sens_field.toPlainText()))
        params.append(int(self.ui.bme_field.toPlainText()))
        params.append(int(self.ui.disc_time_field.toPlainText()))
        params.append(TCP)
        params.append(UDP)
        IP = bytes(IP, "ascii")
        params.append(IP)
        SSID = bytes(SSID, "ascii")
        params.append(SSID) #JorgePiWireless
        PASS = bytes(PASS, "ascii")
        params.append(PASS) #AardvarkBadgerHedgehog
        #len(SSID) == len(JorgePiWireless)
        print(type(TCP)) 
        print(UDP) 
        print(IP) 
        print(SSID) 
        print(PASS)
        print(len(params))
        print(params)

        return params   
        
    def configSetup(self):
        Param = self.getConfigParams()
        self.ConfigSave(database, Param, self.ID[self.actual])
        if Param[0] == 0:
                self.configSetupBT(Param)
        else:
                self.configSetupTCP(Param)
                

    def configSetupBT(self, params):
        # envía una configuración indicada por BLE al dispositivo conectado
        self.conectarMac()
        ESPconf = params
        paquete = pack("<iiiiiiiiiii15s22s",ESPconf[0], ESPconf[1], ESPconf[2], ESPconf[3], ESPconf[4] ,ESPconf[5],ESPconf[6],ESPconf[7],ESPconf[8],ESPconf[9], ESPconf[10], ESPconf[11], ESPconf[12])
        print("El largo del paquete es:" + str(len(paquete)))
        qty=0
        while qty<100:
            try:
                self.adapter.start()
                device = self.adapter.connect(self.macs[self.macindx], timeout=2.0)
                device.exchange_mtu(80)
                print('Se conecto!')
                characteristics = device.discover_characteristics().keys()
                # La siguiente linea es para escribir en la caracteristica de UUID list(characteristics)[4], puede hardcodear si
                # sabe la UUID de la caracteristica a escribir, este misma funcion para leer es tan solo char_read
                # Recomiendo leer acerca del sistema de Subscribe para recibir notificaciones del cambio u otros
                device.char_write(list(characteristics)[4], paquete)
                print("Se escribio el paquete")
                qty = 100
            except pygatt.exceptions.NotConnectedError:
                qty += 1
                print("Se han fallado: {qty} intentos" )
                print("Not connected")
                time.sleep(1)
            finally:
                self.adapter.stop()
                
    def configSetupTCP(self, params):
        # envía una configuración indicada por BLE al dispositivo conectado
        #123456789012345 6789012
        #192.168.4.1
        ESPconf = params
        paquete = pack("<iiiiiiiiii11s15s22s",ESPconf[0], ESPconf[1], ESPconf[2], ESPconf[3], ESPconf[4] ,ESPconf[5],ESPconf[6],ESPconf[7],ESPconf[8],ESPconf[9], ESPconf[10], ESPconf[11], ESPconf[12])
        print("El largo del paquete es:" + str(len(paquete)))
        tcp_BD.TCPServerInit(paquete)
        

    def ConfigSave(self, db_file, params, ID):
        conn = None

        try:
            conn = sql.connect(db_file)
        except Error as e:
            print(e)

        cur = conn.cursor()

        cur.execute(''' INSERT INTO Configuration (Id_device, Status_conf, Protocol_conf, Acc_sampling, Acc_sensibility, Gyro_sensibility, BME688_sampling, Discontinuos_time, TCP_PORT, UDP_PORT, Host_ip_addr, Ssid, Pass) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) ''', (ID, params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10].decode(), params[11].decode(), params[12].decode()))
        
        conn.commit()

        conn.close()

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    cont = GUIController(Dialog)
    ui = cont.ui
    ui.setupUi(Dialog)
    Dialog.show()
    cont.setSignals()
    sys.exit(app.exec_())
