import matplotlib.pyplot as plt
import numpy as np
import sys

file = sys.argv[1]
#title = sys.argv[2]
n = range(int(sys.argv[1]))
title = sys.argv[int(sys.argv[1])+2]

first = False
for i in n:
    datapath = sys.argv[i+2]
    data = np.loadtxt(datapath, delimiter=',', skiprows=1)
    X = data[:,0]
    Y = data[:,1]
    if first:
        plt.plot(X,Y,label = sys.argv[i+2][:-4], marker = '*')      
    else:
        first = True
        plt.plot(X,Y,label = sys.argv[i+2][:-4], marker = '*')

plt.xlabel('input size (n)')
plt.ylabel('time (ms)')
plt.title(title)
plt.ylim(ymin=0)
plt.xlim(xmin=0)
plt.legend(shadow = False, fancybox = True)

plt.xticks(np.arange(min(X), max(X)+1, 3000))
plt.savefig(title +'.pdf')
plt.show()
plt.close()
