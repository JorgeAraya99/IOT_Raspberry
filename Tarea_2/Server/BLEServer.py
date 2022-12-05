import pygatt
from findAddresses import findAddresses
import logging
import Desempaquetamiento
import DatabaseWork


adapter = pygatt.GATTToolBackend() ##pygatt
macs = []

def BLEServerFunc(host, port, protocol, conf_p):
    f = open("macaddresbt.txt","r")
    macindx  = int(f.read())
    f.close()
    logging.basicConfig()
    logging.getLogger('pygatt').setLevel(logging.DEBUG)
    qty = 0
    while(qty<100):
        try:
            adapter.start()
            device = adapter.connect(macs[macindx],timeout=2.0)
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
            #self.adapter.stop()
            ready = False
            while True:
                Val = DatabaseWork.getFinished(database)
                if Val == 1:
                    ready = not ready
                if ready: break
                
                read_data = device.char_read(list(characteristics)[4])
    print("Termino de test de conexión")
    
    
    ready = False
    while True:
        Val = DatabaseWork.getFinished(database)
        if Val == 1:
            ready = not ready
            
        if ready: break        
        
        #recibir info
        read_data = 
        data = None
        hdict, ddict = Desempaquetamiento.parseData(data)
        
        #Guardar info
        
        DatabaseWork.DataSave(hdict, ddict, PROTOCOL, database, CONF_P)

        
