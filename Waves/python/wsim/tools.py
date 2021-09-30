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
    k = 2.0*math.pi/l

    sp = setup.source
    print('Sp type: %s' % sp.planeType)
    layers = list(setup.slits) + [setup.screen]
    for ilayer, layer in enumerate(layers):
        x1 = layer.allElementPositions()
        x0 = sp.allElementPositions()
        a0 = sp.allElementAmplitudes()
        p0 = sp.allElementPhases()
        x1, x0, a0, p0 = np.array(x1), np.array(x0), np.array(a0), np.array(p0)
        print('Update amplitudes at layer %d (%d sub-regions with element size=%10.6f)' % \
              (ilayer+1, x1.shape[0], sp.elementSize) )
        print('a0 = ', a0[0])
        if sp.planeType == 'Source' or True:
            a0 *= sp.elementSize
        a1, p1 = [], []
        print('a0*dy, dy, sum(a0) = ', a0[0], sp.elementSize, np.sum(a0))
        print('sum(a0)=', np.sum(a0))
        print('n(x1), layersize=', len(x1), layer.elementSize)
        aa=True
        #
        todx = False
        if layer.planeType == 'Screen':
            todx = True
        for p in x1:
            dx = p - x0
            vl = np.sqrt(np.sum(dx**2, axis=1))
            vre0 = a0*np.cos(p0)
            vim0 = a0*np.sin(p0)
            vcos = np.cos(k*vl)
            vsin = np.sin(k*vl)
            vre = vre0*vcos - vim0*vsin
            vim = vre0*vsin + vim0*vcos
            #
            yre = np.sum(vre)
            yim = np.sum(vim)
            if aa:
                print('vre0, yre, yim, n = ', vre0[0], yre, yim, len(vre0))
                aa = False
            amp1 = np.sqrt(yre**2 + yim**2)
            phase1 = np.arctan2(yim, yre)
            J = 1.0
            if todx:
                # change of variables (theta -> x)
                dxmean = dx[int(dx.shape[0]/2)]
                theta = math.atan2(dxmean[1], dxmean[0])
                r = math.sqrt(np.sum(dxmean**2))
                J = 1.0/(r*math.cos(theta))
            amp1 *= J
            if layer.planeType != 'Screen':
                amp1 /= layer.elementSize
                pass
            a1.append(amp1)
            p1.append(phase1)
        layer.updateAmplitudes(a1, p1)
        sp = layer
    return None


