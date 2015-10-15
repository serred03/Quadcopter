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



print 'Enter your commands below. \r\nInsert "exit" to leave application'

commands = 1

while 1:
    commands = raw_input(">> ")
    comm_list = commands.split()

    if commands == 'exit':
        port.close()
        exit()
    else:
        for com in comm_list:
            port.write(com + '\r')
            print port.readline()

    
