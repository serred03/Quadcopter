#!/usr/bin/python
#Program to comunicate between raspberry pi and arduino 
#using serial connection over the pins 0(RX) and 1(TX)
#on arduino and pins 14(TX) and 15(RX) on raspberry pi. 
#There needs to be a voltage divider on pin TX from 
#arduino due to the diference in logic levels of 5V 
#and 3.3V on raspberry.
#
#Created by serred 10/14/15

import serial, sys, socket

args = sys.argv

UDP_PORT = 5005
UDP_IP = "192.168.1.20"

port = serial.Serial("/dev/ttyAMA0",baudrate=9600,timeout=3.0)


if len(args) > 1 and len(args) < 3:
    mode = args[1]
    if mode == "wifi":
        UDP_IP = "192.168.1.20"
    elif mode == "adhoc":
        UDP_IP = "10.0.0.200"
    else:
        print mode, " is an invalid mode."
        exit()
else:
    UDP_IP = "192.168.1.20"

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

sock.bind((UDP_IP,UDP_PORT))

commands = 1

while 1:
    commands,addr = sock.recvfrom(1024)
    comm_list = commands.split()

    if commands == 'exit':
        port.close()
        exit()
    else:
        for com in comm_list:
	    print com + '\r'
            port.write(com + '\r')
            line = port.readline()
	    sock.sendto(line,addr)
