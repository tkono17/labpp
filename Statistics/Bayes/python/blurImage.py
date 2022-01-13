#!/usr/bin/env python3

import os, sys
import argparse
import logging
import math

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mplimg

logger = logging.getLogger(__name__)

def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input-image', dest='inputImage',
                        type=str, default='',
                        help='Input image file (.jpg)')
    parser.add_argument('-s', '--sigma-blur', dest='sigmaBlur',
                        type=float, default=1.0,
                        help='Sigma of the Gaussian blur [pixels]')
    parser.add_argument('-r', '--recover-mode', dest='recoverMode',
                        action='store_true', default=False,
                        help='Enable to run in recovery mode (unblur)')
    
    return parser.parse_args()

def bw(img):
    imgbw = np.mean(img, axis=2, dtype=int)
    return imgbw

def extractColor(img, color='BGR'):
    w = np.ndarray([ 0.0, 0.0, 0.0])
    if 'B' in color:
        w[0] += 1.0
    if 'G' in color:
        w[1] += 1.0
    if 'R' in color:
        w[2] += 1.0
    wsum = sum(w)
    w /= wsum
    img2 = np.average(img, axis=2, weights=w).astype(int)
    return img2

def ij2k(i, j, nr, nc):
    return (i*nc + j)

def k2ij(k, nr, nc):
    i = int(k / nc)
    j = int(k % nc)
    return (i, j)

def createGauss(mu, sigma):
    def func(r):
        a = 1.0/(np.sqrt(2*math.pi)*sigma)
        y = a * np.exp(-r*r/(2*sigma*sigma) )
        return y
    return func

def filterWindow(mu, sigma, z=3):
    d = int(sigma*z)
    n = 2*d + 1
    m = np.zeros( (n, n) )
    logger.debug('Filter window size is %dx%d (sigma=%5.2f)' % (n, n, sigma) )
    for i in range(n):
        for j in range(n):
            s1 = (-d + i)/sigma
            s2 = (-d + j)/sigma
            r = math.sqrt(s1**2 + s2**2)
            a = 1.0/(math.sqrt(2*math.pi)*sigma)
            y = a*math.exp(-(s1*s1 + s2*s2)/2.0)
            m[i,j] = y
    return m

def blur(img, sigma):
    from numpy.lib.stride_tricks import as_strided
    logger.debug('Create filter window')
    m = filterWindow(0.0, sigma)
    print(m)
    img2shape = tuple(np.subtract(img.shape, m.shape)+1)
    logger.debug('Prepare sub-matrices for convolution'+str(img2shape) )
    subMatrices = as_strided(img, m.shape+img2shape, img.strides+img.strides)
    img2 = np.einsum('ij,ijkl->kl', m, subMatrices)
    return img2

def recover(img, sigma):
    pass

def runBlurImage(args):
    img = None
    if os.path.exists(args.inputImage):
        img = plt.imread(args.inputImage)
    else:
        logger.warn('Cannot find file %s' % args.inputImage)
        return -1
    sigmaBlur = args.sigmaBlur
    
    logger.info('Plot image %s' % (args.inputImage) )
    fig = plt.figure()
    ax = fig.add_subplot(2, 2, 1)
    ax.imshow(img)

    imgbw = bw(img)
    ax = fig.add_subplot(2, 2, 2)
    ax.imshow(imgbw, cmap='gray')

    imgblur = blur(imgbw, sigmaBlur)
    print('Blurred image size: %s' % str(imgblur.shape) )
    ax = fig.add_subplot(2, 2, 3)
    logger.debug('Show blurred image')
    ax.imshow(imgblur, cmap='gray')

    logger.debug('Show last image')
    #ax = fig.add_subplot(2, 2, 4)

    plt.show()
        
if __name__ == '__main__':
    args = parseArgs()
    logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)
    runBlurImage(args)
    
    
