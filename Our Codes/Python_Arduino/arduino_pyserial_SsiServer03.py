import serial
import time

# make sure the 'COM#' is set according the Windows Device Manager
ser = serial.Serial('COM7', 115200, timeout=1)
time.sleep(2)

#house data lists
h_data_1=[0,10]
h_data_2=[0,0]

#while true:
for i in range(10):
    line = ser.readline()   # read a byte
    if line:
        string = line.decode()
        print(string)
        #add data of each house to lists. Power Generation, Consumption.
        if string[0]=='1':
            h_data_1[0]=int(string[2:4]) #3-4
            h_data_1[1]=int(string[5:7]) #5-6
        if string[0]=='2':
            h_data_2[0]=int(string[2:4])
            h_data_2[1]=int(string[5:7])

    # Calculate excess power values
    h1_ex=(h_data_1[1]-h_data_1[0])
    h2_ex=(h_data_2[1]-h_data_2[0])
    #print(h1_ex)
    #print(h2_ex)

    #Compare excess power values
    if h1_ex>0 and h2_ex>0:
        #ser.write(L)
        print("Both +")
    elif h1_ex<0 and h2_ex<0:
        #ser.write(L)
        print("Both -")
    elif ((h1_ex>0 and h2_ex<0) or (h1_ex<0 and h2_ex>0)):
        #ser.write(H)
        print("Relays ON")
