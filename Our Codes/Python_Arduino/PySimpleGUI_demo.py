# hello_psg.py

import PySimpleGUI as sg

layout = [[sg.Text("Hello from PySimpleGUI")], [sg.Button("OK")], [sg.Button("close")]]

# Create the window
window = sg.Window("Demo", layout, margins=(200, 100))

# Create an event loop
while True:
    event, values = window.read()
    # End program if user closes window or
    # presses the OK button
    if event == "OK" or event == sg.WIN_CLOSED:
        break

    elif event == "close" or event == sg.WIN_CLOSED:
        break

window.close()
