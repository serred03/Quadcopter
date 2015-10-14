import serial 

port = serial.Serial("/dev/ttyAMA0",baudrate=9600,timeout=3.0)
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


