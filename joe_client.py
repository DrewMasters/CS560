import socket
import itertools
import sys

def netcat(input, output, remote):
	"""trivial example for 1:1 request-response protocol"""
	for request in input:
		remote.send(request)
		response = remote.read() #recv[1024]
		output.write(response)

handshake = ['connect', 'initial', 'handshake', 'stuff']
cat = itertools.chain(handshake, sys.stdin)

server = (socket.gethostname(), 12345)
netcat(cat, sys.stdout, socket.create_connection(server))
