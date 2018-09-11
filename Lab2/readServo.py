import serial

arduinoComPort = "/dev/ttyACM0(Arduino/Genuino Uno)"

# set baud rate
baudRate = 9600

# open serial port
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

while True:
    lineOfData = serialPort.readline().decode()

    if len(lineOfData) > 0:

        sensor, outputval = (int(x) for x in lineOfData.split)
