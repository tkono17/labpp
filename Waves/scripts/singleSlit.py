#!/usr/bin/env python3

import math

import wsim

mm = wsim.mm
um = wsim.um
nm = wsim.nm
deg = wsim.deg

def singleSlit():
    setup = wsim.SlitSetup2()
    setup.source = wsim.Source1(0.1*mm, (0.0*mm, -0.05*mm), 90.0*deg)
    setup.screen = wsim.Screen1(500.0*mm, (1000.0*mm, -250.0*mm), 90.0*deg)
    setup.addSlit(wsim.SingleSlit1(100.0*mm, 0.1*mm, (50.0*mm, -0.05*mm), 90.0*deg))
    #
    dx0 = 100.0*um
    dx1 = 10.0*nm*10
    dx2 = 100.0*um*10
    setup.source.setElementSize(dx0)
    for s in setup.slits:
        s.setElementSize(dx1)
    setup.screen.setElementSize(dx2)

    v0 = setup.source.allElementPositions()
    v1 = setup.slits[0].allElementPositions()
    v2 = setup.screen.allElements()

    print('Source [%d elements, size=%10.6f [mm]' % (len(v0), setup.source.elementSize) )
    print('Slit [%d elements, size=%10.6f [mm]' % (len(v1), setup.slits[0].apertures[0].elementSize) )
    print('Screen [%d elements, size=%10.6f [mm]' % (len(v2), setup.screen.elementSize) )

    print('Start calculating')
    wsim.calculate(setup)
    print('Calculation done')

def parseArgs():
    return None

if __name__ == '__main__':
    args = parseArgs()
    singleSlit()
