import serial
import time

import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import PySimpleGUI as sg
import matplotlib.pyplot

# make sure the 'COM#' is set according the Windows Device Manager
ser = serial.Serial('COM7', 115200, timeout=1)
time.sleep(2)

#house data lists
h_data_1=[0,10]
h_data_2=[0,0]

data=[]

# Define the window layout
layout = [
    [sg.Text("Plot test")],
    [sg.Canvas(key="-CANVAS-")],
    [sg.HSeparator()],
    [sg.Button("Ok")],
]

# Create the form and show it without the plot
window = sg.Window(
    "SSI Server interface",
    layout,
    location=(0, 0),
    finalize=True,
    element_justification="left",
    font="Helvetica 11",
)

while True:

    data=[]

    for i in range(10):
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
                data.append(h_data_2[1])

    #PySimpleGUI---------------

    fig = matplotlib.figure.Figure(figsize=(3, 2), dpi=100)
    #fig = plt.figure(figsize=(3, 2), dpi=100)
    fig.suptitle('Generation')
    fig.add_subplot(111).plot(data,"g")

    matplotlib.use("TkAgg")

    def draw_figure(canvas, figure):
        figure_canvas_agg = FigureCanvasTkAgg(figure, canvas)
        figure_canvas_agg.draw()
        figure_canvas_agg.get_tk_widget().pack(side="top", fill="both", expand=1)
        return figure_canvas_agg



    event, values = window.read()
    # End program if user closes window or
    # presses the OK button
    if event == "OK" or event == sg.WIN_CLOSED:
        break

    elif event == "close" or event == sg.WIN_CLOSED:
        break


    # Add the plot to the window
    draw_figure(window["-CANVAS-"].TKCanvas, fig)

window.close()
