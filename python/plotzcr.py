import matplotlib.pyplot as plt
import numpy as np

newLines = []
res_txt = open("/home/josep/Escritorio/PAV/PAV-P2/python/res.txt","r")
lines = res_txt.readlines()
for x in range(len(lines)):
    newLines.append(lines[x].split("\t"))
    
newestList = []
for x in range(len(newLines)):
    newestList.append(newLines[x][3])

lastesList = []
for x in range(len(newestList)):
    lastesList.append(float(newestList[x].replace("\n","")))

plt.plot(lastesList, 'b-', label='Audio P2')
plt.legend(loc="upper right")
plt.grid()
plt.ylabel('ZCR')
plt.xlabel('Muestras')
plt.title('ZCR')
plt.show()
