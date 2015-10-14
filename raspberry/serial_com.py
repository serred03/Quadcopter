#!/usr/bin/python
#Program to comunicate between raspberry pi and arduino 
#using serial connection over the pins 0(RX) and 1(TX)
#on arduino and pins 14(TX) and 15(RX) on raspberry pi. 
#There needs to be a voltage divider on pin TX from 
#arduino due to the diference in logic levels of 5V 
#and 3.3V on raspberry.
#
#Created by serred 10/14/15

import serial, sys

args = sys.argv

port = serial.Serial("/dev/ttyACM0",baudrate=9600,timeout=3.0)

pwm = 0
if len(args) == 3:
    pwm = args[2]

if len(args) < 2 or len(args) > 3:
    print "Invalid number of arguments"
elif args[1] == 'TH':
    com = "TH_" + str(pwm) + "\r"
    port.write(com)
elif args[1] == 'AX':
    com = "AX_" + str(pwm) + "\r"
    port.write(com)
elif args[1] == 'AI':
    com = "AI_" + str(pwm) + "\r"
    port.write(com)
elif args[1] == 'EL':
    com = "EL_" + str(pwm) + "\r"
    port.write(com)
elif args[1] == 'RD':
    com = "RD_" + str(pwm) + "\r"
    port.write(com)
elif args[1] == 'ARM':
    com = "ARM\r"
    port.write(com)
elif args[1] == 'DRM':
    com = "DRM\r"
    port.write(com)
else:
    print "Invalid command"
    sys.exit() 
inLine = port.readline()

print inLine

    
