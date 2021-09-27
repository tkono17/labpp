#!/usr/bin/env python3

import math
import random
import argparse

import numpy as np
import matplotlib.pyplot as plt

import ppstat

xRange = (0.0, 100.0)
muRange = (0.0, 100.0)
sigmaRange = (0.0, 100.0)
nX = 100
nMu = 1000
nSigma = 5000
#
nX = 50
nMu = 1000
nSigma = 2000
dx = (xRange[1] - xRange[0])/nX
dmu = (muRange[1] - muRange[0])/nMu
dsigma = (sigmaRange[1] - sigmaRange[0])/nSigma

def GausL(x, pars):
    m = pars[0]
    s = abs(pars[1])
    if s == 0.0:
        s = 1.0E-10
    z = (x - m)/s
    a = 1.0/(math.sqrt(2.0*math.pi)*s)
    y = a*np.exp(-z**2/2.0)
    if y < 1.0E-10:
        y = 1.0E-10
    return y

def Lfunc_p(f, x):
    def f2(p):
        return f(x, p)
    return f2

def inferLogP(x, n, parDist1, parValues):
    Lfunc = GausL
    n1, n2 = len(parValues[0]), len(parValues[1])
    parDist2 = np.array( (n1, n2) )
    f = Lfunc_p(GausL, x)
    v = np.array([f( (p1, p2) ) for p1 in parValues[0] for p2 in parValues[1]])
    parDist2 = v.reshape(n1, n2)
    ptot = np.sum(parDist2)
    #parDist2 /= ptot
    parDist2 = n*np.log(parDist2)
    return parDist2

def inferDist(xv, parDist1, parValues):
    n = len(xv)
    lpSum = np.zeros(parDist1.shape)
    for i in range(n):
        #x = xRange[0] + (i+0.5)*dx
        x = xv[i]
        print('x = %10.5f' % x)
        if xv[i] > 0.0:
            #print('xv = %10.5f' % xv[i])
            nbin = 1.0 #xv[i]
            lp = inferLogP(x, nbin, parDist1, parValues)
            lpSum += lp
    return lpSum

def genDist(pars, n):
    y = []
    for i in range(n):
        y.append(random.gauss(pars[0], pars[1]))
    return y

def bayesMain(args):
    parDist = np.ones( (nMu, nSigma) )/(nMu*nSigma)
    parValues = [
        np.arange(muRange[0], muRange[1], dmu),
        np.arange(sigmaRange[0], sigmaRange[1], dsigma), 
        ]
    parValues[0] += dmu/2.0
    parValues[1] += dsigma/2.0
    print('Parameter space %dx%d' % (len(parValues[0]), len(parValues[1])) )
    xDist = genDist( (50.0, 6.0), 2)
    parDist2 = inferDist(xDist, parDist, parValues)
    parDist2 = np.exp(parDist2)
    #
    fig = plt.figure()
    fig.add_subplot(3, 2, 1)
    plt.hist(xDist, bins=nX, range=xRange )

    fig.add_subplot(3, 2, 2)
    x, y = np.meshgrid(parValues[0], parValues[1])
    n = nMu*nSigma;
    x2 = x.reshape(n)
    y2 = y.reshape(n)
    z2 = [parDist2[a, b] for a in range(nMu) for b in range(nSigma) ]
    z = np.array(z2).reshape(nMu, nSigma)
    print(len(x), len(y), len(z))
    plt.pcolormesh(x, y, parDist2, cmap=plt.get_cmap('Greys'))
    fig.add_subplot(3, 2, 3)
    x = np.arange(muRange[0], muRange[1], dmu)
    plt.plot(x, np.sum(parDist, axis=1))

    fig.add_subplot(3, 2, 4)
    x = np.arange(sigmaRange[0], sigmaRange[1], dsigma)
    plt.plot(x, np.sum(parDist, axis=0))

    fig.add_subplot(3, 2, 5)
    x = np.arange(muRange[0], muRange[1], dmu)
    plt.plot(x, np.sum(parDist2, axis=1)*dsigma)

    fig.add_subplot(3, 2, 6)
    x = np.arange(sigmaRange[0], sigmaRange[1], dsigma)
    plt.plot(x, np.sum(parDist2, axis=0)*dmu)

    plt.savefig('test.png')
    plt.show()
    
def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--dataFile', dest='dataFile', 
                        type=str, default='', 
                        help='Name of the data file')

    return parser.parse_args()

if __name__ == '__main__':
    args = parseArgs()
    bayesMain(args)
    
