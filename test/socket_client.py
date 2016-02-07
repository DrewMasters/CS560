#simple client program
#www.tutorialspoint.com/python/python_networking.htm

#usage: python socket_client.py hostname (other host machine)
#	or
#	python socket_client.py (local machine)

import sys
import socket

def send_receive(sock):
	command=raw_input('terminal: ')
	while command.lower() != 'exit':
		sock.send(command)
		print sock.recv(1024)
		command=raw_input('terminal: ')
	sock.send('done')
	return
	
s = socket.socket()
if len(sys.argv) == 1:
	host = socket.gethostname()
else:
	host = str(sys.argv[1])
port = 12345
s.connect((host,port))
print s.recv(1024)
send_receive(s)
s.close()
