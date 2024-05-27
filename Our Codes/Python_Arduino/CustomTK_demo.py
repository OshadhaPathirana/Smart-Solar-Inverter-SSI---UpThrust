#SSI Server Interface
###############################################################################################################

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
#import tkinter as tk
import numpy as np
import serial as sr
import time

import tkinter
import customtkinter
from PIL import Image,ImageTk

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

        #Text Labels
        label_expw1 = customtkinter.CTkLabel(master=root, text= "House 01 Excess Power: " ,  width=200, height=50, fg_color=("#546467"),font = ('calbiri',15),corner_radius=10)
        label_expw1.place(relx=0.8, rely=0.55, anchor=tkinter.CENTER)
        root.update()

        label_expw2 = customtkinter.CTkLabel(master=root, text= "House 02 Excess Power: " ,  width=200, height=50, fg_color=("#546467"),font = ('calbiri',15),corner_radius=10)
        label_expw2.place(relx=0.8, rely=0.65, anchor=tkinter.CENTER)
        root.update()

        #Live Excess Power Labels
        expw1 = str(h_data_1[0]-h_data_1[1])+" W"
        label_expw1 = customtkinter.CTkLabel(master=root, text= expw1 ,  width=60, height=50, fg_color=("#546467"),font = ('calbiri',12.5),corner_radius=10)
        label_expw1.place(relx=0.9, rely=0.55, anchor=tkinter.CENTER)
        root.update()
        expw2 = str(h_data_2[0]-h_data_2[1])+" W"
        label_expw2 = customtkinter.CTkLabel(master=root, text= expw2 ,  width=60, height=50, fg_color=("#546467"),font = ('calbiri',12.5),corner_radius=10)
        label_expw2.place(relx=0.9, rely=0.65, anchor=tkinter.CENTER)
        root.update()

    root.after(1,plot_data)

def plot_start():
    global cond
    cond = True
    s.reset_input_buffer()

def plot_stop():
    global cond
    cond = False


#-----Main GUI code-----
#root = tk.Tk()

root = customtkinter.CTk()
root.title(' SSI SERVER ')
#root.configure(background = 'light blue')
root.geometry("1050x800") # set the window size

customtkinter.set_appearance_mode("dark")
customtkinter.set_default_color_theme("blue")

#------create Plot object on GUI----------
# add figure canvas
fig = Figure(facecolor='#9aa6a8');
fig.suptitle('House 01                                       House 02 ', fontsize=16)

#graph 1-1
ax = fig.add_subplot(2,2,1)

ax.set_title('Generation 1');
#ax.set_xlabel('Time')
ax.set_ylabel('W')
ax.set_xlim(0,100)
ax.set_ylim(5,25)
ax.set_facecolor('#546467')
lines = ax.plot([],[],"k")[0]

#graph 1-2
ax1 = fig.add_subplot(2,2,3)

ax1.set_title('Consumption 1');
ax1.set_xlabel('Time')
ax1.set_ylabel('W')
ax1.set_xlim(0,100)
ax1.set_ylim(5,25)
ax1.set_facecolor('#546467')
lines1 = ax1.plot([],[],"white")[0]

#graph 2-1
ax2 = fig.add_subplot(2,2,2)

ax2.set_title('Generation 2');
#
ax2.set_ylabel('W')
ax2.set_xlim(0,100)
ax2.set_ylim(5,25)
ax2.set_facecolor('#546467')
lines2 = ax2.plot([],[],"k")[0]

#graph 2-2
ax3 = fig.add_subplot(2,2,4)

ax3.set_title('Consumption 2');
ax3.set_xlabel('Time')
ax3.set_ylabel('W')
ax3.set_xlim(0,100)
ax3.set_ylim(5,25)
ax3.set_facecolor('#546467')
lines3 = ax3.plot([],[],"white")[0]


canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.get_tk_widget().place(x = 10,y=80, width = 900,height = 625)
canvas.draw()

#Button Setup
root.update();
button_head = customtkinter.CTkButton(master=root, text=" Smart Solar Inverter [SSI] Server ", width=600, height=75,font = ('calbiri',35),fg_color="#546467",hover_color="#546467") #width=120, height=32,
button_head.place(relx=0.5, rely=0.025, anchor=tkinter.CENTER)

root.update();
button_st = customtkinter.CTkButton(master=root, text=" Start ", width=200, height=50,font = ('calbiri',20), command = lambda: plot_start()) #width=120, height=32,
button_st.place(relx=0.8, rely=0.2, anchor=tkinter.CENTER)

root.update();
button_stop = customtkinter.CTkButton(master=root, text=" Stop ", width=200, height=50, font = ('calbiri',20), command = lambda:plot_stop())
button_stop.place(relx=0.8, rely=0.3, anchor=tkinter.CENTER)

root.update();
button_sgi = customtkinter.CTkButton(master=root, text=" Stop Grid Injection ", width=200, height=50, font = ('calbiri',20))
button_sgi.place(relx=0.8, rely=0.45, anchor=tkinter.CENTER)

#Add the image Label
my_img=ImageTk.PhotoImage(Image.open('D:\\Oshadha\\Professional Projects\\UpThrust\\Our Codes\Python_Arduino\\logo.png').resize((300, 200)))
label_img=customtkinter.CTkLabel(root, image=my_img,text="")
label_img.place(relx=0.8, rely=0.85, anchor=tkinter.CENTER)
root.update()

#----start serial port----
s = sr.Serial('COM7',115200);
s.reset_input_buffer()


root.after(100,plot_data)
root.mainloop()
