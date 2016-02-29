import socket
import sys

def netcat(hostname, port, content):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((hostname, port))
	s.sendall(content)
	s.shutdown(socket.SHUT_WR)
	while 1:
		data = s.recv(1024)
		if data == "":
			break
		print "Received:", repr(data)
	print "Connection closed."
	s.close()

while 1:
	netcat(socket.gethostname(),5007,sys.stdin.readline())
