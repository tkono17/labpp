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
    def calcPhase(p2, p1, p0):
        p12 = p2 - p1
        p10 = p1 - p0
        phase = (p12 + p10)*k
        return phase

    for p2 in v2:
        phaseSum = 0.0
        for p1 in v1:
            for p0 in v0:
                phaseSum += calcPhase(p2, p1, p0)
    return 1.0



