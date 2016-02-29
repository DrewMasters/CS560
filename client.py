import socket
def client(string):
	HOST, PORT = 'localhost', 2000
	# SOCK_STREAM == a TCP socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#sock.setblocking(0)  # optional non-blocking
	sock.connect((HOST, PORT))
	sock.send(string)
	reply = sock.recv(16384)  # limit reply to 16K
	print reply
	sock.send(string)
	reply = sock.recv(16384)  # limit reply to 16K
	print reply
	sock.close()
	return reply

print client('2+2\n')
