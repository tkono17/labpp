#-----------------------------------------------------------------------
# Wave Simulation
# tools.py
#-----------------------------------------------------------------------

import math

import numpy as np

from wsim.model import *


def calculate(setup):
    v0 = setup.source.allElementPositions()
    v1 = setup.slits[0].allElementPositions()
    v2 = setup.screen.allElementPositions()
    v2b = setup.screen.allElements()

    v0 = np.array(v0)
    v1 = np.array(v1)
    v2 = np.array(v2)

    l = setup.source.waveLength
    k = math.pi/l
    def calcPathLength(p2, p1, p0):
        p12 = p2 - p1
        p10 = p1 - p0
        return (p10+p12)

    for i, p2 in enumerate(v2):
        a = 0.0
        for p1 in v1:
            for p0 in v0:
                length = calcPathLength(p2, p1, p0)
                phase = length*k
    return 1.0

def updateAmplitudes(setup):
    l = setup.source.waveLength
    k = math.pi/l

    sp = setup.source
    layers = setup.slits + [setup.screen]
    for layer in layers:
        x1 = layer.allElementPositions()
        x0 = sp.allElementPositions()
        a0 = sp.allElementAmplitudes()
        p0 = sp.allElementPhases()
        x1, x0, a0, p0 = np.array(x1), np.array(x0), np.array(a0), np.array(p0)
        a0 = a0#*sp.elementSize
        a1, p1 = [], []
        for p in x1:
            dx = p - x0
            vl = np.sqrt(np.sum(dx**2, axis=1))
            vre0 = a0*np.cos(p0)
            vim0 = a0*np.sin(p0)
            vcos = np.cos(k*vl+p0)/vl
            vsin = np.sin(k*vl+p0)/vl
            vre = vcos*a0 - vsin*vim0
            vim = vsin*a0 + vcos*vim0
            yre = np.sum(vre)
            yim = np.sum(vim)
            amp1 = np.sqrt(yre**2 + yim**2)
            phase1 = np.arctan2(yim, yre)
            a1.append(amp1)
            p1.append(phase1)
        layer.updateAmplitudes(a1, p1)
        sp = layer
    return None


