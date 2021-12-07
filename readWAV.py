from scipy.io import wavfile
import matplotlib.pyplot as plt

import sys
import numpy as np

rate1,dat1 = wavfile.read(sys.argv[1])
with open(sys.argv[2], 'w') as f:
    for i in range(len(dat1)):
        print(dat1[i], file=f)
