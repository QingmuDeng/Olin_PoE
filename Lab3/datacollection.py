import pickle
import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D
%matplotlib notebook
import numpy as np

// getting data points from pickle file
points = pickle.load(open("20181011-212616", "rb"))


// cut the data because it did a twist at the beginning
left_motor = points[0]
left_motor= left_motor[76:]

right_motor = points[1]
right_motor= right_motor[76:]

left_sensor = points[2]
left_sensor = left_sensor[76:]

right_sensor = points[3]
right_sensor = right_sensor[76:]

time = points[4]
time = time[76:]


// plotting left
plt.plot(time, left_motor, 'r-', label="Left Motor")
plt.plot(time, left_sensor, 'b-', label="Left Sensor")
plt.legend(loc='upper right')
plt.title("Left Side")
plt.xlabel("Time")
plt.ylabel("Motor Speed / IR reading")
plt.show

plt.savefig('poelab3_right.png', dpi=fig.dpi)

// plotting right
plt.plot(time, right_motor, 'r-', label="Right Motor")
plt.plot(time, right_sensor, 'b-', label="Right Sensor")
plt.legend(loc='upper right')

plt.title("Right Side")
plt.xlabel("Time")
plt.ylabel("Motor Speed / IR reading")
plt.show

plt.savefig('poelab3_left.png', dpi=fig.dpi)
