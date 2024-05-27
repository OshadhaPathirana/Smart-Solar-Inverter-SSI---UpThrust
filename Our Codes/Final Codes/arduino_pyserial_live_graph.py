from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
#import matplotlib.pyplot
import tkinter as tk
import numpy as np
import serial as sr
import time

#------global variables
data = np.array([])
data1 = np.array([])
data2 = np.array([])
data3 = np.array([])
cond = False

h_data_1=[0,10]
h_data_2=[0,0]

#-----plot data-----
def plot_data():
    global cond, data, data1, data2, data3, h_data_1, h_data_2

    if (cond == True):

        #for i in range(10):
        l = s.readline()   # read a byte
            #if l:
        string = l.decode()
                #print(string)
                #add data of each house to lists. Power Generation, Consumption.
        if string[0]=='1':
            h_data_1[0]=int(string[2:4]) #3-4
            h_data_1[1]=int(string[5:7]) #5-6

        if string[0]=='2':
            h_data_2[0]=int(string[2:4])
            h_data_2[1]=int(string[5:7])

        #data limiting
        if(len(data) < 100):
            data= np.append(data,h_data_1[0])
        else:
            data[0:99] = data[1:100]
            data[99] = h_data_1[0]

        #data1 limiting
        if(len(data1) < 100):
            data1= np.append(data1,h_data_1[1])
        else:
            data1[0:99] = data1[1:100]
            data1[99] = h_data_1[1]

        #data2 limiting
        if(len(data2) < 100):
            data2= np.append(data2,h_data_2[0])
        else:
            data2[0:99] = data2[1:100]
            data2[99] = h_data_2[0]

        #data3 limiting
        if(len(data3) < 100):
            data3= np.append(data3,h_data_2[1])
        else:
            data3[0:99] = data3[1:100]
            data3[99] = h_data_2[1]
        #print(len(data))

        lines.set_xdata(np.arange(0,len(data)))
        lines.set_ydata(data)

        lines1.set_xdata(np.arange(0,len(data)))
        lines1.set_ydata(data1)

        lines2.set_xdata(np.arange(0,len(data)))
        lines2.set_ydata(data2)

        lines3.set_xdata(np.arange(0,len(data)))
        lines3.set_ydata(data3)

        canvas.draw()

    root.after(1,plot_data)

def plot_start():
    global cond
    cond = True
    s.reset_input_buffer()

def plot_stop():
    global cond
    cond = False


#-----Main GUI code-----
root = tk.Tk()
root.title('Real Time Plot')
root.configure(background = 'light blue')
root.geometry("1050x800") # set the window size

#------create Plot object on GUI----------
# add figure canvas
fig = Figure();
ax = fig.add_subplot(2,2,1)

#ax.plt.axes(xlim=(0,100),ylim=(0, 120)); #displaying only 100 samples
#graph 1-1
ax.set_title('Generation 1');
#ax.set_xlabel('Time')
ax.set_ylabel('W')
ax.set_xlim(0,100)
ax.set_ylim(10,60)
lines = ax.plot([],[],"r")[0]

#graph 1-2
ax1 = fig.add_subplot(2,2,2)

ax1.set_title('Consumption 1');
#ax1.set_xlabel('Time')
ax1.set_ylabel('W')
ax1.set_xlim(0,100)
ax1.set_ylim(10,60)
lines1 = ax1.plot([],[],"g")[0]

#graph 2-1
ax2 = fig.add_subplot(2,2,3)

ax2.set_title('Generation 2');
ax2.set_xlabel('Time')
ax2.set_ylabel('W')
ax2.set_xlim(0,100)
ax2.set_ylim(10,60)
lines2 = ax2.plot([],[],"b")[0]

#graph 2-2
ax3 = fig.add_subplot(2,2,4)

ax3.set_title('Consumption 2');
ax3.set_xlabel('Time')
ax3.set_ylabel('W')
ax3.set_xlim(0,100)
ax3.set_ylim(10,60)
lines3 = ax3.plot([],[],"k")[0]


canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.get_tk_widget().place(x = 10,y=10, width = 800,height = 700)
canvas.draw()

#canvas = FigureCanvasTkAgg(fig1, master=root)  # A tk.DrawingArea.
#canvas.get_tk_widget().place(x = 515,y=10, width = 500,height = 400)
#canvas.draw()

#----------create button---------
root.update();
start = tk.Button(root, text = "  Start  ", font = ('calbiri',20),command = lambda: plot_start())
start.place(x = 850, y = 10 )

root.update();
stop = tk.Button(root, text = "  Stop  ", font = ('calbiri',20), command = lambda:plot_stop())
stop.place(x = start.winfo_x()+start.winfo_reqwidth() + 50, y = 10)

root.update();
stop = tk.Button(root, text = "  Stop Grid Injection ", font = ('calbiri',20), )#command = lambda:plot_stop())
stop.place(x = 850, y = 100)

#----start serial port----
s = sr.Serial('COM7',115200);
s.reset_input_buffer()



root.after(100,plot_data)
root.mainloop()
