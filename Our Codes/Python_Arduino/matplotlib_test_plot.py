import matplotlib.pyplot as plt
import numpy as np

x=np.arange(10)
y1=x+5
y2=x**2
plt.figure(figsize=(10,5))
plt.subplot(1,2,1)
plt.plot(x,y1,"red")
plt.subplot(1,2,2)
plt.plot(x,y2,"blue")

plt.show()
