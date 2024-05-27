import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import serial
import time

ser = serial.Serial('COM7', 115200, timeout=1)
time.sleep(2)

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

xs = []
ys = []
zs = []

h_data_1=[0,10]
h_data_2=[0,0]



# This function is called periodically from FuncAnimation
def animate(i, xs, ys,zs):

    #for i in range(10):
    line = ser.readline()   # read a byte
    if line:
        string = line.decode()
        #print(string)
        #add data of each house to lists. Power Generation, Consumption.
        if string[0]=='1':
            h_data_1[0]=int(string[2:4]) #3-4
            h_data_1[1]=int(string[5:7]) #5-6

        if string[0]=='2':
            h_data_2[0]=int(string[2:4])
            h_data_2[1]=int(string[5:7])
            #data.append(h_data_2[1])

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime("%H:%M:%S"))
    ys.append(h_data_2[1])
    zs.append(h_data_2[0])

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]
    zs = zs[-20:]

    # Draw x and y lists

    ax.clear()
    ax.plot(xs,ys,"r",label="Generation")

    #ax1 = fig.add_subplot(2, 1, 2)
    ax.plot(xs,zs,"g",label="Consumption")
    ax.legend(loc="upper left")

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Power Generation & Consumption')
    plt.ylabel('kW')

# Set up plot to call animate() function periodically
for j in range(50):
    #ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys,zs), interval=500)
    ani = animation.FuncAnimation(fig, animate, fargs=(xs,ys,zs), interval=100)
    plt.show()
