from socketserver import UDPServer
from TCPServer import *
from UDPServer import *

#funcion que castea ambos servidores
def activate_conect():
    TCPServerFunc()
    UDPServerFunc()

#se castea la funcion
activate_conect()