import socket

def TCPServerInit(package):
	HOST = "192.168.4.1"
	PORT = 5000
	
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	s.bind((HOST,PORT))
	s.listen(5)
	print(f'Listening on{HOST}:{PORT} ')
	
	
	ready = False
	while True:
		if ready:break
		
		print("Waiting for a connection")
		conn, addr = s.accept()
		print(f'Connected to ({addr[0]}) on port ({addr[1]})')
		
		try:
			data = conn.recv(1024)
			if data == b'':
				break
		except ConnectionError:
			print("Connection Error")
			break
		
		conn.send(package)
		print("Config sent succesfully!")
		ready = not ready
		
