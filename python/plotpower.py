import matplotlib.pyplot as plt
import numpy as np

newLines = []
res_txt = open("/home/josep/Escritorio/PAV/PAV-P2/python/res.txt","r")
lines = res_txt.readlines()
for x in range(len(lines)):
    newLines.append(lines[x].split("\t"))
    
newestList = []
for x in range(len(newLines)):
    newestList.append(float(newLines[x][1]))

plt.plot(newestList, 'b-', label='Audio de P2')
plt.legend(loc="upper right")
#plt.xlim(0,440)
plt.grid()
plt.ylabel('dB')
plt.xlabel('Muestras')
plt.title('Potencia media')
plt.show()
