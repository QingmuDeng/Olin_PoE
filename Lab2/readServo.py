import serial
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

arduinoComPort = "/dev/ttyACM0"
# set baud rate
baudRate = 115520

# open serial port
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
i = 0
lineOfData = ""

while i < 300:
    try:
        lineOfData = serialPort.readline().decode()
    except:
        print("okay let's ignore it")


    if len(lineOfData) > 0:
        val = [int(s) for s in lineOfData.split() if s.isdigit()]
        if(len(val)<3):
            continue
        print(val)

        ax.scatter(val[2], val[0], val[1])
        # plt.xlim([0, 200])
        plt.xlabel('Pan')
        # plt.ylim([0, 200])
        plt.ylabel('Tilt')
        i += 1

plt.show()

