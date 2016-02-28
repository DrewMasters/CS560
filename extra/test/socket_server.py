#simple test server in python
#www.tutorialspoint.com/python/python_networking.htm

#usage: python socket_server.py &

import socket
import subprocess
from subprocess import PIPE
import os

def receive_send(sock):
	#t=sock.recv(1024)	
	#print t
	'''
	if t=='done':
		print 'finished'
		return False
	'''
	#else:
		#t=subprocess.check_output(t.split())
	t=subprocess.check_output(["/home/dmaster1/CS560/CS560/bin/sh"],stdin=PIPE)
	print t
	sock.send(t)
	return True

s=socket.socket()
host = socket.gethostname()
port = 12345
s.bind((host,port))

os.dup2(s.fileno(),0)
os.dup2(s.fileno(),1)

s.listen(5)
while True:
	c, addr = s.accept()
	print 'Got connection from', addr
	c.send('connected')
	print 'have connection'
	flag = receive_send(c)
	while flag:
		flag = receive_send(c)
