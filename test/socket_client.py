#simple client program
#www.tutorialspoint.com/python/python_networking.htm

import sys
import socket

if len(sys.argv) == 1:
	s = socket.socket()
	host = socket.gethostname()
	port = 12345
	s.connect((host,port))
	print s.recv(1024)
	s.close()
else:
	s = socket.socket()
	host = str(sys.argv[1])
	port = 12345
	s.connect((host,port))
	print s.recv(1024)
	s.close()
