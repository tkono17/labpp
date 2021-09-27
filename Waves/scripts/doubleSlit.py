#!/usr/bin/env python3

import os, sys
import math
import argparse
import pickle

import matplotlib.pyplot as plt
import numpy as np

import wsim

mm = wsim.mm
um = wsim.um
nm = wsim.nm
deg = wsim.deg
mrad = 1.0E-3

def doubleSlit(args):
    # Position of the planes (x-direction)
    x0 = args.z0
    x1 = args.z1
    x2 = args.z2
    w0 = args.w0
    w1 = args.w1
    w2 = args.w2
    # Aperture paremters
    a = args.a
    b = args.b
    # Misalignment parameters
    t1 = args.t1
    alpha1 = args.alpha1
    # Wavelength
    wl = 500*nm
    #
    setup = wsim.SlitSetup2()
    setup.source = wsim.Source1(w0, (x0, -w0/2.0), 90.0*deg)
    setup.source.waveLength = wl
    setup.screen = wsim.Screen1(w2, (x2, -w2/2.0), 90.0*deg)
    setup.addSlit(wsim.DoubleSlit1(w1, a, b, (x1, -w1/2.0), 90.0*deg+alpha1))
    setup.source.setIntensity(1.0)
    #
    dx0 = 10.0*nm
    dx1 = 10.0*nm
    dx2 = 100.0*um
    setup.source.setElementSize(dx0)
    for s in setup.slits:
        s.setElementSize(dx1)
    setup.screen.setElementSize(dx2)

    print('Source element size %f' % setup.source.elementSize)
    for i, s in enumerate(setup.slits):
        print('Slit %d element size %f' % (i, s.elementSize) )
    print('Screen element size %f' % setup.screen.elementSize)

    a0 = setup.source.amplitudes
    print('N amplitudes', len(a0))
    xm = 1.0E-2
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
    y = wsim.intensitySingleSlitX(x, b, wl, x2)
    r = y[int(len(y)/2.0)]/a2[int(len(a2)/2.0)]
    y /= r
    fig, ax = plt.subplots()
    #plt.yscale('log')
    ax.plot(x, a2)
    ax.plot(x, y, '--')
    plt.show()
    #plt.savefig('figures/doubleSlit_l%dmm_1m.png' % int(abs(x0)))

    print('intensity at x=0: %e' % a2[int(len(a2)/2)])
    if len(setup.slits)>0:
        a1 = setup.slits[0].allElementAmplitudes()
        print('intensity at slit1 x=0: %e' % a1[int(len(a1)/2)])

    f = open('a.pickle', 'wb')
    pickle.dump(setup, f)
    print('printout slit amplitudes')
    #print(setup.slits[0].amplitudes)
    f.close()


def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('--z0', dest='z0',
                        type=float, default=-10.0*mm, 
                        help='Z position of the 0th slit [mm]')
    parser.add_argument('--z1', dest='z1',
                        type=float, default=0.0*mm, 
                        help='Z position of the 1st slit [mm]')
    parser.add_argument('--z2', dest='z2',
                        type=float, default=1000.0*mm, 
                        help='Z position of the screen [mm]')
    parser.add_argument('--w0', dest='w0',
                        type=float, default=0.05*mm, 
                        help='Width of the light source [mm]')
    parser.add_argument('--w1', dest='w1',
                        type=float, default=100.0*mm, 
                        help='Width of the 1st slit [mm]')
    parser.add_argument('--w2', dest='w2',
                        type=float, default=200.0*mm, 
                        help='Width of the screen [mm]')
    parser.add_argument('-a', dest='a',
                        type=float, default=100.0*um, 
                        help='Gap between the two apertures of the double slit [mm]')
    parser.add_argument('-b', dest='b',
                        type=float, default=20.0*um, 
                        help='Aperture width of the slit [mm]')
    parser.add_argument('--waveLength', dest='waveLength',
                        type=float, default=500.0*nm, 
                        help='Wavelength [mm]')
    parser.add_argument('--t1', dest='t1',
                        type=float, default=0.0*um, 
                        help='Misalignment parameter for translation [mm]')
    parser.add_argument('--alpha1', dest='alpha1', 
                        type=float, default=0.0*nm, 
                        help='Misalignment parameter for rotation [mrad]')
    return parser.parse_args()

if __name__ == '__main__':
    args = parseArgs()
    doubleSlit(args)
