# Import the required libraries
from tkinter import *
from PIL import Image, ImageTk

# Create an instance of tkinter frame or window
win=Tk()

# Set the size of the tkinter window
win.geometry("700x350")

# Load the image
image=Image.open('D:\\Oshadha\\Professional Projects\\UpThrust\\Our Codes\Python_Arduino\\index111.png')
img=image.resize((450, 350))
my_img=ImageTk.PhotoImage(img)
label=Label(win, image=my_img)
label.pack()

win.mainloop()
