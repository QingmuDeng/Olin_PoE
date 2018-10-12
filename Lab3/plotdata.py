import pickle
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
%matplotlib notebook

points = pickle.load(open("20181011-203730", "rb"))

measurements = points[0]
leftmotor = points[1]
rightmotor = points[2]
# distance_threshold = 55.00
background_measurements = []
background_indices = []
foreground_measurements = []
foreground_indices = []
index_count = 0

for each in measurements:
#     print(each)
    if each > distance_threshold:
        background_measurements.append(each)
        background_indices.append(index_count)
    else:
        foreground_measurements.append(each)
        foreground_indices.append(index_count)

    index_count += 1

background_pan = []
background_tilt = []
foreground_pan = []
foreground_tilt = []

for each in background_indices:
#     print(each)
    background_pan.append(pan[each])
    background_tilt.append(tilt[each])

for each in foreground_indices:
    foreground_pan.append(pan[each])
    foreground_tilt.append(tilt[each])

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(foreground_measurements, foreground_pan, foreground_tilt, color='b')
ax.scatter(background_measurements, background_pan, background_tilt, color='r')

ax.set_xlabel('Distance')
ax.set_ylabel('Pan(degrees)')
ax.set_zlabel('Tilt(degrees)')
plt.xlim(20,80)
plt.show()
plt.savefig('poelab2.png', dpi=fig.dpi)
