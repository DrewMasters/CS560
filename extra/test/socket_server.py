#simple test server in python
#www.tutorialspoint.com/python/python_networking.htm

#usage: python socket_server.py &

import socket

def receive_send(sock):
	t=sock.recv(1024)	
	print t
	if t=='done':
		return False
	else:
                t=subprocess.check_output(t.split())
                print t
	        sock.send(t)
		return True

s=socket.socket()
host = socket.gethostname()
port = 12345
s.bind((host,port))

s.listen(5)
while True:
	c, addr = s.accept()
	print 'Got connection from', addr
	c.send('connected')
	flag = receive_send(c)
	while flag:
		flag = receive_send(c)
