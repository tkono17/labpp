#-----------------------------------------------------------------------
# Wave Simulation
# model.py [mm]
#-----------------------------------------------------------------------

import math
import numpy as np

mm = 1.0
um = 1.0E-3
nm = 1.0E-6
deg = math.pi/180.0

class Aperture1:
    def __init__(self, length, location):
        self.length = length
        self.location = location
        self.elementSize = 0.0
        self.nElements = 0
    def setElementSize(self, s):
        self.nElements = int(self.length/s)
        if self.nElements <= 0.0: self.nElements = 1
        self.elementSize = self.length/self.nElements
        self.amplitudes = [0.0]*self.nElements
        self.phases = [0.0]*self.nElements
    def allElements(self, offset=0.0):
        v = []
        print('Aperture n %d %f' % (self.nElements, self.length))
        for i in range(self.nElements):
            v.append(self.elementCenter(i)+offset)
        return v
    def elementCenter(self, i):
        if i >= 0 and i < self.nElements:
            return self.elementSize*(i+0.5)
        else:
            return None

class Slit1:
    def __init__(self, length, location, angle, planeType='Slit'):
        """planeType: None, Slit, Source, Screen"""
        self.length = length
        self.location = location
        self.angle = angle
        self.apertures = []
        self.planeType = 'Screen'
        self.elementSize = 0.0
        self.nElements = 0
        self.amplitudes = []
        self.phases = []
    def addAperture(self, location, length):
        a = Aperture1(length, location)
        self.apertures.append(a)
        self.planeType = 'Slit'
    def setElementSize(self, s):
        if self.planeType in ('Slit'):
            for a in self.apertures:
                a.setElementSize(s)
            self.nElements = 0.0
            self.elementSize = 0.0
        elif self.planeType in ('Screen', 'Source'):
            print(self.length)
            print(s)
            self.nElements = int(self.length/s)
            if self.nElements <= 0.0: self.nElements = 1
            self.elementSize = self.length/self.nElements
            self.amplitudes = [0.0]*self.nElements
            self.phases = [0.0]*self.nElements
    def nApertures(self):
        return len(self.apertures)
    def allElements(self):
        v = []
        if self.planeType in ('Source', 'Screen'):
            for i in range(self.nElements):
                v.append(self.elementSize*(i+0.5))
        elif self.planeType in ('Slit'):
            for a in self.apertures:
                v += a.allElements(a.location)
        return v
    def allElementPositions(self):
        v = self.allElements()
        x0, y0 = self.location[0], self.location[1]
        c = math.cos(self.angle)
        s = math.sin(self.angle)
        v2 = map(lambda x: (x0+x*c, y0+x*s), v)
        return list(v2)
    def allElementAmplitudes(self):
        v = []
        if self.planeType in ('Source', 'Screen'):
            v = self.amplitudes
        elif self.planeType in ('Slit'):
            for a in self.apertures:
                v += a.amplitudes
        return v
    def allElementPhases(self):
        v = []
        if self.planeType in ('Source', 'Screen'):
            v = self.phases
        elif self.planeType in ('Slit'):
            for a in self.apertures:
                v += a.phases
        return v
    def updateAmplitudes(self, vamplitude, vphase):
        print('Update amplitude for %s n=%d' % (self.planeType, len(vamplitude)))
        if self.planeType in ('Source', 'Screen'):
            self.amplitudes = list(vamplitude)
            self.phases = list(vphase)
        elif self.planeType in ('Slit'):
            offset = 0
            for a in self.apertures:
                n = a.nElements
                a.amplitudes = list(vamplitude[offset:offset+n])
                a.phases = list(vphase[offset:offset+n])
                offset += n
        

class SingleSlit1(Slit1):
    def __init__(self, length, a, location, angle):
        super().__init__(length, location, angle)
        x1 = length/2.0 - a/2.0
        self.addAperture(x1, a)

class DoubleSlit1(Slit1):
    def __init__(self, length, a, b, location, angle):
        super().__init__(length, location, angle)
        x1 = length/2.0 - (a+b)/2.0
        self.addAperture(x1, a)
        x2 = length/2.0 + (b-a)/2.0
        self.addAperture(x2, a)

class Source1(Slit1):
    def __init__(self, length, location, angle):
        super().__init__(length, location, angle)
        self.planeType = 'Source'
        self.waveLength = 500.0*nm
        self.intensity = 1.0
    def setIntensity(self, x):
        self.intensity = x
    def setElementSize(self, s):
        super().setElementSize(s)
        a = math.sqrt(self.intensity)/(self.nElements)#*self.length)
        self.amplitudes = [a]*self.nElements
        self.phases = [0.0]*self.nElements
class Screen1(Slit1):
    def __init__(self, length, location, angle):
        super().__init__(length, location, angle)
        self.planeType = 'Screen'

class SlitSetup2:
    def __init__(self):
        self.source = None
        self.slits = []
        self.screen = None
    def setSource(self, s):
        self.source = s
    def setScreen(self, s):
        self.screen(s)
    def addSlit(self, s):
        self.slits.append(s)

