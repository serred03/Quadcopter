import socket 
UDP_IP = "192.168.42.1"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

sock.bind((UDP_IP,UDP_PORT))

while True:
    data,addr = sock.recvfrom(1024)
    print "recieved message:", data

