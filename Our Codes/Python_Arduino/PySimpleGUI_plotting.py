import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import PySimpleGUI as sg
import matplotlib.pyplot
#import matplotlib.pyplot

fig = matplotlib.figure.Figure(figsize=(3, 2), dpi=100)
#fig = plt.figure(figsize=(3, 2), dpi=100)
t = np.arange(0, 3, .01)
fig.suptitle('Generation')
fig.add_subplot(111).plot(t, 2 * np.sin(2 * np.pi * t),"g",t,4 * np.sin(4 * np.pi * t))

fig2 = matplotlib.figure.Figure(figsize=(3, 2), dpi=100)
fig2.add_subplot(111).plot(t, 4 * np.sin(4 * np.pi * t))

fig3 = matplotlib.figure.Figure(figsize=(3, 2), dpi=100)
fig3.add_subplot(111).plot(t, 4 * np.sin(4 * np.pi * t))

matplotlib.use("TkAgg")

def draw_figure(canvas, figure):
    figure_canvas_agg = FigureCanvasTkAgg(figure, canvas)
    figure_canvas_agg.draw()
    figure_canvas_agg.get_tk_widget().pack(side="top", fill="both", expand=1)
    return figure_canvas_agg

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

# Add the plot to the window
draw_figure(window["-CANVAS-"].TKCanvas, fig)
draw_figure(window["-CANVAS-"].TKCanvas, fig2)
draw_figure(window["-CANVAS-"].TKCanvas, fig3)

event, values = window.read()

window.close()
