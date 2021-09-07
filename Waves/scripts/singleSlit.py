#!/usr/bin/env python3

import math
import pickle

import matplotlib.pyplot as plt
import numpy as np

import wsim

mm = wsim.mm
um = wsim.um
nm = wsim.nm
deg = wsim.deg
mrad = 1.0E-3

def singleSlit():
    # Length of the planes (x-direction)
    l0, l1, l2 = -100.0, 0.0*mm, 1000.0*mm
    # Width of each plane (y-direction)
    w0, w1, w2 = 0.05*mm, 100.0*mm, 200*mm
    # Misalignment parameters
    t1 = 100.0*um
    alpha1 = 20.0*mrad
    t1,alpha1 = 0.0, 0.0
    # Slit parameters
    b = 50.0*um
    # Wavelength
    wl = 500*nm
    #
    setup = wsim.SlitSetup2()
    setup.source = wsim.Source1(w0, (l0, -w0/2.0), 90.0*deg)
    setup.source.waveLength = wl
    setup.screen = wsim.Screen1(w2, (l2, -w2/2.0), 90.0*deg)
    setup.addSlit(wsim.SingleSlit1(w1, b, (l1, -w1/2.0+t1), 90.0*deg+alpha1))
    setup.source.setIntensity(1.0)
    #
    dx0 = 10.0*nm
    dx1 = 10.0*nm
    dx2 = 100.0*um*2
    setup.source.setElementSize(dx0)
    for s in setup.slits:
        s.setElementSize(dx1)
    setup.screen.setElementSize(dx2)

    a0 = setup.source.amplitudes
    print('N amplitudes', len(a0))
    xm = -1E-2
    dx = 2*xm/len(a0)
    phase0 = np.arange(-xm, xm, dx*10E+3)
    print('phase0 shape', phase0.shape)
    setup.source.phases = phase0

    print('Start calculating')
    wsim.updateAmplitudes(setup)
    print('Calculation done')
    x = setup.screen.allElements()
    a = setup.screen.allElementAmplitudes()
    x = np.array(x) + setup.screen.location[1]
    a = np.array(a)
    a2 = a*a/2.0
    #
    y = wsim.intensitySingleSlitX(x, b, wl, l2)
    fig, ax = plt.subplots()
    #plt.yscale('log')
    ax.plot(x, a2)
    ax.plot(x, y, '--')
    plt.show()

    f = open('a.pickle', 'wb')
    pickle.dump(setup, f)
    print('printout slit amplitudes')
    print(setup.slits[0].amplitudes)
    f.close()


def parseArgs():
    return None

if __name__ == '__main__':
    args = parseArgs()
    singleSlit()
