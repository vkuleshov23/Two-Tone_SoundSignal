import matplotlib.pyplot as plt
import sys
import numpy as np

for i in range(1, len(sys.argv)):
    words = open(sys.argv[i]).read().split()
    y = [float(v) for k, v in enumerate(words)]
    plt.plot(y)

plt.grid()
plt.show()