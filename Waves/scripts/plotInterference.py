#!/usr/bin/env python3

import math
import wsim

import numpy as np
import matplotlib.pyplot as plt

def plotSingleSlitPattern(b, wl, l):
    n = 10000
    xm = 100.0
    dx = 2*xm/n
    x = np.arange(-xm, xm, dx)
    y = wsim.intensitySingleSlitX(x, b, wl, l)
    plt.plot(x, y)
    plt.show()
    
if __name__ == '__main__':
    b = 0.05
    wl = 500.0E-6
    r = 1000.0
    plotSingleSlitPattern(b, wl, r)
