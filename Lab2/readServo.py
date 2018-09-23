import serial, time
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

class Scanner():
    def __init__(self):
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.dataPointNum = 0
        self.lineOfData = ""
        self.measure = []
        self.pan = []
        self.tilt = []

    def config_serial(self, baudrate=115520):
        """ Set up the serial port to communicate with Arduino
        """
        try: # Try either ttyACM0 or ttyACM1 because Arduino switches around between these two
            arduinoComPort = "/dev/ttyACM0"
            # set baud rate
            baudRate = baudrate
            # open serial port
            self.serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)
            print('Serial Port ttyACM0')
        except:
            arduinoComPort = "/dev/ttyACM1"
            # set baud rate
            baudRate = baudrate
            # open serial port
            self.serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)
            print('Serial Port ttyACM1\n')

    def config_arduino(self):
        """ Make Arduino ready for scanning
        """
        time.sleep(3) # Sleep to allow Arduino self reset
        print("Resetting Servo positions")
        self.serialPort.write(b'e') # "Initialize" state trigger event
        time.sleep(3) # delay to allow servo to reset

        print("Scanning is starting immediately.")
        self.serialPort.write(b'i') # "Scanning" state trigger event

    def read_data(self, dataNum=100):
        """ Read data from UART
        """
        while self.dataPointNum < dataNum:
            try:
                self.lineOfData = self.serialPort.readline().decode()
            except:
                print("bad serial format")


            if len(self.lineOfData) > 0:
                print(self.lineOfData)
                val = [int(s) for s in self.lineOfData.split() if s.isdigit()]
                if(len(val)<3):
                    continue
                val[2] = interpolate(val[2])
                if val[2] == 0:
                    continue
                print(val)

                self.measure.append(val[2])
                self.pan.append(val[0])
                self.tilt.append(val[1])
                
                # plt.zlabel('Tilt')
                self.dataPointNum += 1

    def exit(self):
        """ Store and plot the point cloud, reset Arduino
        """
        self.serialPort.write(b'e')
        self.ax.set_xlabel('Distance')
        self.ax.set_ylabel('Pan(degrees)')
        self.ax.set_zlabel('Tilt(degrees)')
        self.ax.scatter(self.measure, self.pan, self.tilt, color='b')
        plt.show()


def interpolate(sensorVal):
    distance = 0
    if sensorVal > 519:
        # print("Too close")
        return 0
    # elif sensorVal <= 519 and sensorVal > 460:
    #     distance = 65.01 - 0.085*(sensorVal)
    # elif sensorVal <= 460 and sensorVal > 363:
    #     distance = 60.278 - 0.076*(sensorVal)
    # elif sensorVal <= 363 and sensorVal > 289:
    #     distance = 78.47 - 0.122*(sensorVal)
    # elif sensorVal <= 289 and sensorVal > 245:
    #     distance = 93.23 - 0.173*(sensorVal)
    # elif sensorVal <= 245 and sensorVal > 193:
    #     distance = 110.637 - 0.244*(sensorVal)
    # elif sensorVal <= 193 and sensorVal > 160:
    #     distance = 152.631 - 0.462*(sensorVal)
    # elif sensorVal <= 160 and sensorVal > 113:
    #     distance = 182.502-0.649*(sensorVal)
    # elif sensorVal <= 113 and sensorVal > 97:
    #     distance = 234.79 - 1.111*(sensorVal)
    # elif sensorVal <= 97 and sensorVal > 80:
    #     distance = 271.6 - 1.49*(sensorVal)
    elif sensorVal <= 80:
        # print("Too far away")
        return 0

    return sensorVal


def main():

    scanner = Scanner()

    scanner.config_serial()

    scanner.config_arduino() 

    scanner.read_data(10)

    scanner.exit()

if __name__ == "__main__":
    main()