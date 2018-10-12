import serial, time, pickle

import numpy as np

class LineRobot():
    def __init__(self):
        self.dataPointNum = 0
        self.lineOfData = ''
        self.sensor_left = []
        self.sensor_right = []
        self.leftmotor = []
        self.rightmotor = []
        self.time = []


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



    def read_data(self, dataNum=100):
    # """ Read data from UART"""

        while self.dataPointNum < dataNum:
            try:
                self.lineOfData = self.serialPort.readline().decode()
                print(self.lineOfData)
            except:
                print("bad serial format")


            if len(self.lineOfData) > 0:
                 val = [int(s) for s in self.lineOfData.split() if s.isdigit()]
                 print(val)
                 if len(val)==5:
                     self.leftmotor.append(val[0])
                     self.rightmotor.append(val[1])
                     self.sensor_left.append(val[2])
                     self.sensor_right.append(val[3])
                     self.time.append(val[4])


            self.dataPointNum += 1


    def exit(self):
        """ Store and plot the point cloud, reset Arduino
        """

        file_name = time.strftime("%Y%m%d-%H%M%S")
        fileObject = open(file_name,'wb')
        pickle.dump([self.leftmotor, self.rightmotor, self.sensor_left, self.sensor_right, self.time], fileObject)
        fileObject.close()

        print(self.rightmotor)




def main():

    lineRobot = LineRobot()

    lineRobot.config_serial()

    # lineRobot.config_arduino()

    lineRobot.read_data(200)


    lineRobot.exit()

if __name__ == "__main__":
    main()
