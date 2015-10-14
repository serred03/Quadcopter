#Program to comunicate between raspberry pi and arduino 
#using serial connection over the pins 0(RX) and 1(TX)
#on arduino and pins 14(TX) and 15(RX) on raspberry pi. 
#There needs to be a voltage divider on pin TX from 
#arduino due to the diference in logic levels of 5V 
#and 3.3V on raspberry.
#
#Created by serred 10/14/15


import serial 

port = serial.Serial("/dev/ttyACM0",baudrate=9600,timeout=3.0)
port.write("TH_100\r")
a = port.readline()
print(a)

port.write("RD_100\r")
a = port.readline()
print(a)

port.write("EL_100\r")
a = port.readline()
print(a)

port.write("AI_200\r")
a = port.readline()
print(a)

port.write("AX_200\r")
a = port.readline()
print(a)


