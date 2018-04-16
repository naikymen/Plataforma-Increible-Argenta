#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import serial
import serial.tools.list_ports
import time

ports = list(serial.tools.list_ports.comports())
for p in ports:
    print(p)

# Toma dato del serial
time.sleep(2)
ser = serial.Serial('/dev/ttyACM0', 9600)
print(ser.readline())

# Activa ploteo interactivo
plt.ion()
plt.axis([0, 100, 0, 100])

i = 0
while True:
    print(ser.readline())
    line = str(ser.readline())
    t1 = float(line.split(":")[1].split()[0])
    t2 = float(line.split(":")[2].split()[0][0:-5])
    print(t1,t2)
    i += 1
    plt.scatter(i, t1, color="red")
    plt.scatter(i, t2, color="blue")
    plt.pause(1)

