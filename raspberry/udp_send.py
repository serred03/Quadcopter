import socket 

UDP_IP = "192.168.42.1"
UDP_PORT = 5005
commandE = ""


sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
print "Enter command to send, and 'exit' to terminate program"

while 1:
    command = raw_input(">>")
    sock.sendto(command,(UDP_IP,UDP_PORT))
    reply = sock.recvfrom(1024)
    print reply[0]
    if command == "exit":
        exit()
    
