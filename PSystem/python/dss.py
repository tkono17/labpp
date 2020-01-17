#!/usr/bin/env python3
#-----------------------------------------------------------------------
# Dynamical System Simulation
#-----------------------------------------------------------------------
import os
import math
import random
import json

class Constants:
    def __init__(self):
        self.e = 1.6e-19
        self.kB = 1.38064852e-23
        self.g = 9.8

    def set(self, scaleSet):
        self.kB = 1.38064852e-23*(scaleSet.Theta/scaleSet.E)
        self.g = 9.8/scaleSet.A

    def printIt(self):
        print('-----------')
        print('  Constants')
        print('-----------')
        print('    e (electric charge): %3.2e' % self.e)
        print('    kB (Boltzmann constant): %3.2e' % self.kB)
        print('    g (gravitational acceleration): %3.2e' % self.g)

class ScaleSet:
    def __init__(self):
        e = 1.6e-19
        self.E = 1.0*e # 1 (eV)
        self.M = 1.0e+6*e # 1 (MeV)
        self.X = 200.0e-10 # 100 (Angstrom)
        self.V = math.sqrt(self.E/self.M)*3.0e+8
        self.T = self.X/self.V
        self.A = self.V/self.T
        self.F = self.E/self.X
        self.Theta = 1.0
        self.constants = Constants()

    def setFromEMX(self, E, M, X):
        self.E = E
        self.M = M
        self.X = X
        self.V = math.sqrt(self.E/self.M)*3.0e+8
        self.T = self.X/self.V
        self.A = self.V/self.T
        self.F = self.E/self.X
        self.Theta = 1.0
        self.constants.set(self)
        
    def printIt(self):
        print('Scale set:')
        print('----------')
        print('  E (energy)         : %3.2e (J)' % self.E)
        print('  M (mass)           : %3.2e (kg)' % self.M)
        print('  X (length)         : %3.2e (m)' % self.X)
        print('  T (time)           : %3.2e (s)' % self.T)
        print('  V (velocity)       : %3.2e (m/s)' % self.V)
        print('  A (acceleration)   : %3.2e (m/s^2)' % self.A)
        print('  F (force)          : %3.2e (N)' % self.F)
        print('  Theta (temperature): %3.2e (K)' % self.Theta)
        self.constants.printIt()

class SystemState:
    kIdle=1
    kPause=2
    kRunning=3

class ParticleProperty:
    def __init__(self):
        self.n = 1
        self.mass = 1.0
        self.radius = 10.0
        self.T = 1.0
        self.color = '#0000ff'

class SystemConfig:
    def __init__(self):
        #-----------------------------------------------------------
        # Scales
        #-----------------------------------------------------------
        self.scaleSet = ScaleSet()
        self.deltaT = 1.0e-2
        #-----------------------------------------------------------
        # System evolution model
        #-----------------------------------------------------------
        self.LJPotential_e = 1.0
        self.LJPotential_r0 = 20.0
        #-----------------------------------------------------------
        # Particle properties
        #-----------------------------------------------------------
        self.particleProperties = [
            ParticleProperty(), ParticleProperty(), ParticleProperty(), 
            ParticleProperty(), ParticleProperty(), 
        ]
        #-----------------------------------------------------------
        # System operation (not really configuration data)
        #-----------------------------------------------------------
        self.state = SystemState.kIdle
        self.startRequest = False
        self.stopRequest = False

    def setStartRequest(self, x):
        self.startRequest = x
        self.stopRequest = False

    def setStopRequest(self, x):
        self.stopRequest = x
        self.startRequest = False

    def printIt(self):
        print('SystemConfig')
        self.scaleSet.printIt()
        print('  deltaT = %3.2e' % self.deltaT)
        print('  LJPotential_e = %3.2e' % self.LJPotential_e)
        print('  LJPotential_r0 = %3.2e' % self.LJPotential_r0)
config = SystemConfig()

class Ball:
    objectIds = []
    def __init__(self, mass, radius, position, momentum, ptype):
        self.objectId = 0
        oid = 1
        if len(Ball.objectIds)>0:
            oid = Ball.objectIds[-1] + 1
        Ball.objectIds.append(oid)
        self.objectId = oid
        self.mass = mass
        self.radius = radius
        self.position = position
        self.momentum = momentum
        self.ptype = ptype
    def energy(self):
        e = (self.momentum.abs())**2/(2.0*self.mass)
        return e
    def velocity(self):
        v = self.momentum/self.mass
        return v
    pass

class Vector:
    def __init__(self, v=[]):
        self.values = v
    def size(self):
        return len(self.values)
    def length(self):
        return len(self.values)
    def __getitem__(self, i):
        n = len(self.values)
        if i>=0 and i<n:
            return self.values[i]
        else:
            return None
    def __setitem__(self, i, v):
        n = len(self.values)
        if i>=0 and i<n:
            self.values[i] = v
        else:
            print('Tryied to set value to invalid element %d while size=%d'\
                  % (i, n) )
    def normalize(self):
        a = self.abs()
        if a > 0:
            self.values = list(map(lambda x: x/a, self.values) )
    def abs(self):
        a = sum(map(lambda x: x*x, self.values) )
        return math.sqrt(a)
    def dot(self, v1):
        n = len(self.values)
        x = 0.0
        for i in range(n):
            x += self.values[i]*v1[i]
        return x
    def __add__(self, v):
        n = len(self.values)
        w = []
        for i in range(n):
            w.append(self.values[i]+v[i])
        return Vector(w)
    def __sub__(self, v):
        n = len(self.values)
        w = []
        for i in range(n):
            w.append(self.values[i]-v[i])
        return Vector(w)
    def __truediv__(self, a):
        v = self.values
        if a != 0.0:
            v = list(map(lambda x: x/a, self.values) )
        return Vector(v)
    def __mul__(self, a):
        v = list(map(lambda x: x*a, self.values) )
        return Vector(v)
    def __rmul__(self, a):
        v = list(map(lambda x: x*a, self.values) )
        return Vector(v)
    def __neg__(self):
        v = list(map(lambda x: -x, self.values) )
        return Vector(v)
    pass
    
class Wall:
    def __init__(self, p1, p2):
        self.position1 = p1
        self.position2 = p2
        pass
    def tangentVector(self):
        v = self.position2 - self.position1
        v.normalize()
        return v
    def innerNormal(self):
        v = self.tangentVector()
        v.values = [-v.values[1], v.values[0] ]
        return v
    def refPoint(self):
        return self.position1
    def isInnerSide(self, ball):
        v = self.innerNormal()
        vb = ball.position - self.refPoint()
        p = v.dot(vb)
        if p > 0.0:
            # The region is on the left-hand side of the wall
            return True
        else:
            return False
    pass

class Collision:
    def __init__(self):
        pass
    def checkCollision(b1, b2):
        r = b1.radius + b2.radius
        dx = b1.position - b2.position
        c = False
        if dx.abs() < r:
            x12 = b2.position - b1.position
            p12 = b2.momentum - b1.momentum
            if p12.dot(x12) <= 0:
                c =  True
        return c

    def checkInsideWall(b1, wall):
        c = wall.isInnerSide(b1)
        if c:
            # Check the distance between the ball and the wall against the radius
            v = wall.tangentVector()
            p0 = wall.refPoint()
            q = b1.position
            a = q - p0
            aa = a.abs()
            av = a.dot(v)
            d = math.sqrt( (aa*aa) - (av*av) )
            if d < b1.radius:
                c = False
        return c

    def updateCollision(b1, b2):
        P = b1.momentum + b2.momentum
        V = P/(b1.mass + b2.mass)
        p1 = b1.momentum - b1.mass*V
        p2 = b2.momentum - b2.mass*V
        d12 = b2.position - b1.position # Vector(b1->b2)
        d12.normalize()
        #
        pa = p1.dot(d12)*d12
        p1 = p1 - 2.0*pa
        #
        d21 = -d12
        pa = p2.dot(d21)*d21
        p2 = p2 - 2.0*pa
        #
        b1.momentum = p1 + b1.mass*V
        b2.momentum = p2 + b2.mass*V
        pass

    def updateAtWall(b1, wall):
        v1 = wall.tangentVector()
        v2 = wall.innerNormal()
        # Update momentum
        p1 = b1.momentum
        pb = p1.dot(v1)*v1
        pa = p1 - pb
        pa = -pa
        pa = -(p1.dot(v2)*v2)
        p1 = pa + pb
        b1.momentum = p1
        # Update position
        pass
    pass

class Potential:
    def __init__(self):
        pass
    def presetNeighbors(self, balls):
        pass
    def energy(self, p, iball):
        return 0.0
    def forceOnParticle(self, p1, balls):
        return 0.0

class GravitationalPotential(Potential):
    """Gravitational potential"""
    def __init__(self, pSystem=None):
        # U(r) = -G*m1*m2/r
        # F(r) = -G*m1*m2*x/r^2
        self.pSystem = pSystem
    def forceOnParticle(self, p1, balls):
        ss = config.scaleSet
        m = p1.mass
        g = ss.constants.g
        f = Vector([0.0, -m*g])
        return f
class CoulombPotential(Potential):
    """Coulomb potential"""
    def __init__(self, pSystem=None):
        # U(r) = k*q1*q2/r
        # F(r) = k*q1*q2*x/r^2
        self.pSystem = pSystem
    def forceOnParticle(self, p1, balls):
        ftotal = Vector([0.0, 0.0])
        fs = []
        r2 = p1.radius
        ss = config.scaleSet
        alpha = 1.0/137.0
        hc=197e-9/(ss.X) # assumes scale E=1 eV
        for p in balls:
            if p.objectId == p1.objectId: continue
            x = p1.position - p.position
            r = x.abs()
            if r < 0.001: continue
            f = alpha*hc/(r*r) * x
            fs.append(f)
        for f in fs:
            ftotal += f
        return ftotal

class LJPotential(Potential):
    """Lennard-Jones potential"""
    def __init__(self, pSystem=None):
        # U(r) = e*( (r0/r)**12 - (r0/r)**6)
        # F(r) = -e*( x*(r0/r)**14 - x*(r0/r)**8)
        self.e = config.LJPotential_e
        self.r0 = config.LJPotential_r0
        self.pSystem = pSystem
        self.neighbors = []
    def presetNeighbors(self, balls):
        self.neighbors = []
        for b1 in balls:
            v = []
            for b2 in balls:
                if b2.objectId == b1.objectId: continue
                x = b1.position - b2.position
                r = x.abs()
                if r < 0.001: continue
                if r<(self.r0*3):
                    v.append(b2)
            self.neighbors.append(v)
        pass
    def energy(self, p, iball):
        e = 0.0
        #print('Energy of iball=%d neighbors=%d' % (iball, len(self.neighbors[iball]) ) )
        for p2 in self.neighbors[iball]:
            x = p.position - p2.position
            r = x.abs()
            if r < 0.01*self.r0:
                r = 0.01*self.r0
            rr = self.r0/r
            e += self.e*(math.pow(rr, 12)-2.0*math.pow(rr, 6) )
        return e
    def forceOnParticle(self, p1, balls):
        ftotal = Vector([0.0, 0.0])
        fs = []
        r2 = self.r0/3.0
        r2 = p1.radius
        for p in balls:
            if p.objectId == p1.objectId: continue
            x = p1.position - p.position
            r = x.abs()
            if r < 0.001: continue
            if r < r2 and r > 0.001:
                x = (r2/r)*x
            rr = self.r0/x.abs()
            #f = self.e*(12*x*math.pow(rr, 14) - 6*x*math.pow(rr, 8) )
            f = 12*self.e*(math.pow(rr, 12)-math.pow(rr, 6))/(r*r)*x
            fs.append(f)
        for f in fs:
            ftotal += f
        return ftotal

class PSystem:
    class EventData:
        def __init__(self, data):
            self.data = data
        def save(self, fname):
            f = open(fname, 'w')
            f.write(json.dumps(self.data) )
            f.close()
        pass
    def __init__(self, sx, sy, boundaryPoints, numberOfTypes, T):
        print('Creating System1 object')
        self.sx = sx
        self.sy = sy
        self.boundaryPoints = boundaryPoints
        self.numberOfTypes = numberOfTypes
        self.numberOfBalls = 0
        #
        self.scaleSet = config.scaleSet
        #
        for i in range(self.numberOfTypes):
            self.numberOfBalls += config.particleProperties[i].n
        self.walls = []
        self.balls = []
        self.T = T
        self.deltaT = config.deltaT
        self.timeStep = 0
        #
        self.updateMethod = 0
        self.sigmaX = config.LJPotential_r0
        self.outputFilename = 'dss.json'
        self.outputFile = None
        self.potentials = [
            GravitationalPotential(self), 
            #CoulombPotential(self), 
            LJPotential(self), 
            ]
        pass
    def derivs(self, t, y):
        n2 = y.size()
        n = len(self.balls)
        balls = []
        iball = 0
        for b in self.balls:
            pos = Vector([0]*2)
            mom = Vector([0]*2)
            pos[0] = y[iball*4]
            pos[1] = y[iball*4+1]
            mom[0] = y[iball*4+2]
            mom[1] = y[iball*4+3]
            b2 = Ball(mass=b.mass, radius=b.radius,
                      position=pos, momentum=mom, ptype=b.ptype)
            b2.objectId = b.objectId
            balls.append(b2)
            iball = iball + 1
        dydx = Vector([0.0]*n2)
        for potential in self.potentials:
            potential.presetNeighbors(balls)
        iball = 0
        for b in balls:
            f = Vector([0.0, 0.0])
            v = b.velocity()
            for potential in self.potentials:
                pnballs = None
                if hasattr(potential, 'neighbors'):
                    pnballs = potential.neighbors[iball]
                if pnballs!=None:
                    neighboringBalls = pnballs
                else:
                    neighboringBalls = balls
                f += potential.forceOnParticle(b, neighboringBalls)
            dydx[iball*4] = v[0]
            dydx[iball*4+1] = v[1]
            dydx[iball*4+2] = f[0]
            dydx[iball*4+3] = f[1]
            iball += 1
        return dydx
    def solveEOM(self, dt):
        n = len(self.balls)
        n2 = n*4
        y = Vector([0.0]*n2)
        dydx0 = Vector([0.0]*n2)
        dydx = Vector([0.0]*n2)
        y1 = Vector([0.0]*n2)
        y2 = Vector([0.0]*n2)
        y3 = Vector([0.0]*n2)
        y4 = Vector([0.0]*n2)
        #
        iball=0
        for b in self.balls:
            pos = b.position
            mom = b.momentum
            y[iball*4] = pos[0]
            y[iball*4+1] = pos[1]
            y[iball*4+2] = mom[0]
            y[iball*4+3] = mom[1]
            iball += 1
        #
        dt2 = dt/2.0
        dydx0 = self.derivs(0, y)
        k1 = Vector([0]*n2)
        k2 = Vector([0]*n2)
        k3 = Vector([0]*n2)
        k4 = Vector([0]*n2)
        for i in range(n2):
            k1[i] = dydx0[i]*dt
            y1[i] = y[i] + k1[i]/2
        dydx = self.derivs(dt/2.0, y1)
        for i in range(n2):
            k2[i] = dydx[i]*dt
            y2[i] = y[i] + k2[i]/2
        dydx = self.derivs(dt/2.0, y2)
        for i in range(n2):
            k3[i] = dydx[i]*dt
            y3[i] = y[i] + k3[i]
        dydx = self.derivs(dt, y3)
        for i in range(n2):
            k4[i] = dydx[i]*dt
        k = k1 + 2*(k2+k3) + k4
        for i in range(n2):
            y2[i] = y[i] + k[i]/6.0
        iball = 0
        for b in self.balls:
            b.position[0] = y2[iball*4]
            b.position[1] = y2[iball*4+1]
            b.momentum[0] = y2[iball*4+2]
            b.momentum[1] = y2[iball*4+3]
            #
            #b.position[1] = y[iball*4+1]
            #b.momentum[1] = y[iball*4+3]
            iball += 1
        pass
    def runTimeStep(self, dt):
        n = len(self.balls)
        ib = 0
        self.solveEOM(dt)

        for i in range(n):
            b1 = self.balls[i]
            for j in range(i+1, n):
                b2 = self.balls[j]
                if Collision.checkCollision(b1, b2):
                    Collision.updateCollision(b1, b2)
        for w in self.walls:
            for b in self.balls:
                if not Collision.checkInsideWall(b, w):
                    Collision.updateAtWall(b, w)
        pass
    def selectUpdateMethod(self, methodName):
        self.updateMethod = 0
        if methodName == 'MC':
            self.updateMethod = 1
    def runMC(self):
        n = len(self.balls)
        iball = random.randint(0, n-1)
        p = self.balls[iball]
        pos = p.position
        x1, y1 = pos[0], pos[1]
        x2 = random.gauss(pos[0], self.sigmaX)
        y2 = random.gauss(pos[1], self.sigmaX)
        xlimit = self.sx - p.radius
        ylimit = self.sy - p.radius
        if x1 < p.radius:
            x1 = p.radius
        elif x1 > xlimit:
            x1 = xlimit
        if y1 < p.radius:
            y1 = p.radius
        elif y1 > ylimit:
            y1 = ylimit
        kT = self.scaleSet.constants.kB*self.T
        e = 0.0
        e2 = 0.0
        for potential in self.potentials:
            potential.presetNeighbors(self.balls)
            e += potential.energy(p, iball)
        pos[0] = x2
        pos[1] = y2
        for potential in self.potentials:
            potential.presetNeighbors(self.balls)
            e2 += potential.energy(p, iball)
        dE = e2 - e
        #print('dE=%f kT=%f' % (dE, kT) )
        ok = False
        if dE<0.0:
            ok = True
        else:
            #print('calculate r=exp(%f)' % (-dE/kT) )
            r = math.exp(-dE/kT)
            s = random.uniform(0, 1.0)
            if s<r:
                ok = True
        if ok:
            # New state has lower energy
            # print('Jump to new state, dE=%f' % dE)
            pass
        else:
            pos[0], pos[1] = x1, y1
        #print('done runMC')
    def update(self):
        if self.updateMethod == 1:
            self.runMC()
            pass
        else:
            self.runTimeStep(self.deltaT)
        saveEvent = False
        m = 1
        if self.timeStep >= 100:
            if self.saveMode==0:
                m = int(math.log10(self.timeStep) )
                m = 10**m
                if (self.timeStep%m)==0:
                    saveEvent = True
            elif self.saveMode>0:
                if (self.timeStep%self.saveMode)==0:
                    saveEvent = True
        if saveEvent:
            self.save()
        self.timeStep += 1

    def setup(self):
        points = self.boundaryPoints
        self.walls = [
            Wall(Vector(points[0]), Vector(points[1]) ), 
            Wall(Vector(points[1]), Vector(points[2]) ), 
            Wall(Vector(points[2]), Vector(points[3]) ), 
            Wall(Vector(points[3]), Vector(points[0]) )
        ]
        pass

    def generateBall(self, particleType=0):
        pp = config.particleProperties[particleType]
        pos = self.generatePosition(particleType)
        mom = self.generateMomentum(particleType)
        ball = Ball(pp.mass, pp.radius, pos, mom, particleType)
        return ball
        
    def generateBall2(self, i, n, particleType=0):
        pp = config.particleProperties[particleType]
        pos = self.generatePosition2(particleType, i, n)
        mom = self.generateMomentum(particleType)
        ball = Ball(pp.mass, pp.radius, pos, mom, particleType)
        return ball
        
    def generateBalls(self):
        self.balls = []
        iball = 0
        nballs = 0
        for itype in range(self.numberOfTypes):
            nballs += config.particleProperties[itype].n
        for itype in range(self.numberOfTypes):
            for i in range(config.particleProperties[itype].n):
                ball = self.generateBall2(iball, nballs, itype)
                self.balls.append(ball)
                iball += 1
        print('Created %d balls' % len(self.balls) )

    def generatePosition2(self, ptype, i, n):
        pp = config.particleProperties[ptype]
        r = pp.radius
        sx, sy=(self.sx, self.sy)
        pos = Vector([0, 0])
        nrow = math.ceil(math.sqrt(n) )
        ncol = nrow
        dx = sx/ncol
        dy = sy/nrow
        irow = math.floor(i/ncol)
        icol = math.floor(i%ncol)
        pos[0] = (icol+0.5)*dx
        pos[1] = (irow+0.5)*dy
        return pos

    def generatePosition(self, ptype):
        pp = config.particleProperties[ptype]
        r = pp.radius
        sx, sy=(self.sx, self.sy)
        rsep = config.LJPotential_r0*0.9
        pos = Vector([0, 0])
        done=False
        while (not done):
            x1 = random.uniform(r, sx-r)
            x2 = random.uniform(r, sy-r)
            pos = Vector([x1, x2])
            done = True
            for b in self.balls:
                dx = b.position - pos
                print('dx = %f, rsep = %f' % (dx.abs(), rsep) )
                if dx.abs() < rsep:
                    print('Try generating the position again')
                    done = False
                    break
                else:
                    print('Generated position looks good')
        print('Add %d-th ball' % len(self.balls) )
        return pos

    def generateMomentum(self, ptype):
        pp = config.particleProperties[ptype]
        m = pp.mass
        t = pp.T
        e = self.scaleSet.constants.kB*t
        p = math.sqrt(2.0*m*e)
        theta = random.uniform(0, 2.0*math.pi)
        x1 = p*math.cos(theta)
        x2 = p*math.sin(theta)
        return Vector([x1, x2])
    def run(self):
        pass
    def openFile(self):
        if self.outputFile == None:
            self.outputFile = open(self.outputFilename, 'w')
            self.outputFile.write('{\n')
            self.outputFile.write('  "events": [\n')
            self.iEvent = 0
            print('DSS open output file: %s' % self.outputFilename)
    def save(self):
        if self.outputFile != None:
            v = []
            for b in self.balls:
                bdata = {
                    'mass': b.mass, 
                    'radius': b.radius, 
                    'position': b.position.values, 
                    'momentum': b.momentum.values, 
                    'ptype': b.ptype
                }
                v.append(bdata)
            jdata = {
                'timeStep': self.timeStep, 
                'balls': v
            }
            if self.iEvent!=0:
                self.outputFile.write(',\n')
            self.outputFile.write(json.dumps(jdata) )
            print('Save event data (time=%d)' % self.timeStep)
            self.iEvent += 1
    def closeFile(self):
        if self.outputFile != None:
            print('DSS close output file: %s' % self.outputFilename)
            self.outputFile.write(']\n')
            self.outputFile.write('}\n')
            self.outputFile.close()
            self.outputFile = None

        
if __name__ == '__main__':
    print('No main program')
