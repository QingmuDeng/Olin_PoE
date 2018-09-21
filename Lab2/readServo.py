import serial
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

arduinoComPort = "/dev/ttyACM0"
# set baud rate
baudRate = 115520

def interpolate(sensorVal):
    distance = 0
    if sensorVal > 519:
        print("Too close")
    elif sensorVal <= 519 and sensorVal > 460:
        distance = 65.01 - 0.085(sensorVal)
    elif sensorVal <= 460 and sensorVal > 363:
        distance = 60.278 - 0.076(sensorVal)
    elif sensorVal <= 363 and sensorVal > 289:
        distance = 78.47 - 0.122(sensorVal)
    elif sensorVal <= 289 and sensorVal > 245:
        distance = 93.23 - 0.173(sensorVal)
    elif sensorVal <= 245 and sensorVal > 193:
        distance = 110.637 - 0.244(sensorVal)
    elif sensorVal <= 193 and sensorVal > 160:
        distance = 152.631 - 0.462(sensorVal)
    elif sensorVal <= 160 and sensorVal > 113:
        distance = 182.502-0.649(sensorVal)
    elif sensorVal <= 113 and sensorVal > 97:
        distance = 234.79 - 1.111(sensorVal)
    elif sensorVal <= 97 and sensorVal > 80:
        distance = 271.6 - 1.49(sensorVal)
    elif sensorVal <= 80:
        print("Too far away")
    retrun distance

def main():
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
            val[2] = interpolate(val[2])
            print(val)

            ax.scatter(val[2], val[0], val[1])
            # plt.xlim([0, 200])
            plt.xlabel('Pan')
            # plt.ylim([0, 200])
            plt.ylabel('Tilt')
            i += 1

    plt.show()

if __name__ == "__main__":
    main()