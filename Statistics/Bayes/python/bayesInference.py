#!/usr/bin/env python3

import argparse
import random
import math
import numpy as np
import matplotlib.pyplot as plt

v_range = (0, 50)
theta_range = (0, 80)
x_range = (0, 100)

def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output-file', dest='outputFile', 
                                     type=str, default='b.png', 
                                     help='Output file (figure) name')
    parser.add_argument('--vMean', dest='vMean', 
                                     type=float, default=15.0, 
                                     help='Mean of the velocity [m/s2]')
    parser.add_argument('--vSigma', dest='vSigma', 
                                     type=float, default=2.0, 
                                     help='Sigma of the velocity [m/s2]')
    parser.add_argument('--thetaMean', dest='thetaMean', 
                                     type=float, default=45.0, 
                                     help='Mean of the angle (degrees)')
    parser.add_argument('--thetaSigma', dest='thetaSigma', 
                                     type=float, default=5.0, 
                                     help='Sigma of the angle (degrees)')
    parser.add_argument('-n', '--nTrials', dest='nTrials', 
                                     type=int, default=1000, 
                                     help='Number of trials')
    return parser.parse_args()

def createData(v0, theta0, sigma_v, sigma_theta, n):
    v = np.zeros(n)
    theta = np.zeros(n)

    for i in range(n):
        v[i] = random.gauss(v0, sigma_v)
        theta[i] = random.gauss(theta0, sigma_theta)
    return v, theta

def calculateDistance(v, theta):
    g = 9.8
    x = (v**2) * np.sin(2*theta)/g
    return x

def plot(v, theta, d, yrec):
    fig = plt.figure()
    ax1 = fig.add_subplot(2, 3, 1)
    ax1.hist(v, bins=20, range=v_range)

    ax1 = fig.add_subplot(2, 3, 2)
    ax1.hist(theta, bins=20, range=theta_range)

    ax1 = fig.add_subplot(2, 3, 3)
    ax1.hist(d, bins=20, range=x_range)

    ax = fig.add_subplot(2, 3, 4)
    ax.scatter(v, theta)
    plt.xlim(0, v_range[1])
    plt.ylim(0, theta_range[1])

    ax = fig.add_subplot(2, 3, 5)
    x1, x2 = np.ones(yrec.shape[0]), np.ones(yrec.shape[1])
    #yrec2 = np.swapaxes(yrec, 0, 1)
    #yrec2 = np.flip(yrec2, axis=0)
    yrec2 = yrec
    ax.imshow(yrec2)

    return fig

def createTransferMatrix21(bins1, ranges1, bins2, ranges2):
    nx = bins1[0]*bins1[1]
    ny = bins2
    print('nx=%d, ny=%d' % (nx, ny) )
    t = np.zeros( (nx, ny) )
    xrange1 = ranges1[0]
    xrange2 = ranges1[1]
    yrange = ranges2
    dx1 = (xrange1[-1] - xrange1[0])/bins1[0]
    dx2 = (xrange2[-1] - xrange2[0])/bins1[1]
    dy = (yrange[-1] - yrange[0])/bins2
    for m in range(nx):
        i = m % bins1[1]
        j = m / bins1[1]
        x1 = xrange1[0] + i*dx1
        x2 = xrange2[0] + j*dx2
        y = calculateDistance(x1, x2)
        k = int( (y-yrange[0])/dy)
        #print('m=%d, i=%d, j=%d, k=%d' % (m, i, j, k) )
        #print('  v=%5.2f theta=%5.2f -> l=%5.2f' % (x1, x2, y) )
        if k < 0 or k >= ny: continue
        t[m,k] += 1.0
    t /= nx
    return t

def infer(x, t, bins):
    y = t*x
    y.resize(bins)
    return y

def toHist(xarray, bins, range):
    h = np.zeros(bins)
    x0 = range[0]
    dx = (range[1]-range[0])/bins
    for x in xarray:
        i = int( (x-x0)/dx)
        if i>=0 and i<bins:
            h[i] += 1.0
    return h

def runBallProblem(args):
    degToRadian = math.pi/180.0
    v0 = args.vMean
    theta0 = args.thetaMean*degToRadian
    sigma_v = args.vSigma
    sigma_theta = args.thetaSigma*degToRadian
    nv, ntheta = 20, 40
    nx = 20

    # Data generation
    v, theta = createData(v0, theta0, sigma_v, sigma_theta, args.nTrials)
    x = calculateDistance(v, theta)
    print('nv=%d, ntheta=%d -> nx=%d' % (len(v), len(theta), len(x) ))
    xhist = toHist(x, nx, x_range)

    # Transfer matrix
    theta_range_rad = list(map(lambda x: x*degToRadian, theta_range))
    t = createTransferMatrix21( (nv, ntheta), [ v_range, theta_range_rad ], 
                                nx, x_range)
    # Bayes inference
    yrec = infer(xhist, t, (ntheta, nv))

    #print(yrec)

    thetaDeg = theta/degToRadian
    fig = plot(v, thetaDeg, x, yrec)
    fig.savefig(args.outputFile)
    #fig.show()
    
if __name__ == '__main__':
    args = parseArgs()
    runBallProblem(args)
