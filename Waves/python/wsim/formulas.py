#-----------------------------------------------------------------------
# Wave Simulation
# model.py [mm]
#-----------------------------------------------------------------------

import math
import numpy as np

def intensitySingleSlitA(theta, b, wl, r):
    """Intensity at distance r from a single slit of width b, as a function of 
    the angle."""
    k = math.pi/wl
    kb = (k*b/2.0)*np.sin(theta)
    y = (1.0/r)*np.sin(kb)/kb
    return (y**2)/2.0

def intensitySingleSlitX(x, b, wl, l):
    """Intensity from a single slit of width b, as a function of the position 
    on the screen at a distance of l."""
    theta = np.arctan2(x, l)
    r = np.sqrt(x**2 + l**2)
    y = intensitySingleSlitA(theta, b, wl, r)
    return y
    
