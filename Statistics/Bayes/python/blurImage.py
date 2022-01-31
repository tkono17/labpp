#!/usr/bin/env python3

import os, sys
import argparse
import logging
import math
import random
import pickle

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mplimg

from ppstat import *

logger = logging.getLogger(__name__)

zScale = 256

def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--inputImage', dest='inputImage',
                        type=str, default='',
                        help='Input image file (.jpg)')
    parser.add_argument('-b', '--batchMode', dest='batchMode',
                        action='store_true', default=False,
                        help='Run in batch mode (no plots)')
    parser.add_argument('-s', '--sigmaBlur', dest='sigmaBlur',
                        type=float, default=1.0,
                        help='Sigma of the Gaussian blur [pixels]')
    parser.add_argument('-r', '--recoveryMode', dest='recoveryMode',
                        action='store_true', default=False,
                        help='Enable to run in recovery mode (unblur)')
    parser.add_argument('--width', dest='width',
                        type=int, default=0,
                        help='Width of the sub-image to analyze')
    parser.add_argument('--height', dest='height',
                        type=int, default=0, 
                        help='Height of the sub-image to analyze')
    parser.add_argument('--offset', dest='offset',
                        type=str, default='0,0', 
                        help='Offset (top-left) of the sub-image (comma-separated value like; x,y)')
    parser.add_argument('--niterations', dest='niterations',
                        type=int, default=10,
                        help='Number of iterations during the recovery')
    parser.add_argument('--recordInterval', dest='recordInterval',
                        type=int, default=100,
                        help='Interval of recording updates')
    parser.add_argument('--proposalSigma', dest='proposalSigma', 
                        type=float, default=0.1,
                        help='Sigma of the proposal function for the pixel value')
    parser.add_argument('--adaptProposalSigma', dest='adaptProposalSigma',
                        action='store_true', default=False,
                        help='Enable to adapt the proposal sigma to keep accept rate near 50%')
    parser.add_argument('--adaptKernelSize', dest='adaptKernelSize',
                        action='store_true', default=False,
                        help='Enable to adapt the kernel size when proposal sigma is large')
    parser.add_argument('--initialCondition', dest='initialCondition',
                        type=str, default='zeros',
                        help='Initial condition to recover the image (zeros|ones|original)')
    parser.add_argument('--scanOrder', dest='scanOrder',
                        type=str, default='rowColumn',
                        help='Order to scan pixels for updating the Markov chain (rowColumn|random)')
    parser.add_argument('-n', '--name', dest='name',
                        type=str, default='chain', 
                        help='Name of this Markov chain (will be saved to a pickle file)')
    parser.add_argument('--zScale', dest='zScale',
                        type=int, default=256,
                        help='Resolution of the z-scale (2: binary, 256: 8 bits)')
    parser.add_argument('--logLevel', dest='logLevel',
                        type=str, default='INFO', 
                        help='Log level (DEBUG|INFO|WARNING|ERROR)')

    
    return parser.parse_args()

def bw(img):
    imgbw = np.mean(img, axis=2, dtype=int)
    #s = float(zScale)/256
    #imgbw = (imgbw*s).astype(int)
    #logger.debug('Image z-resolution: %d (scale by %7.5f)' % (zScale, s))
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

def blur(img, sigma, proposalSigma=None):
    from numpy.lib.stride_tricks import as_strided
    logger.debug('Create filter window')
    m = filterWindow(0.0, sigma)
    if proposalSigma != None:
        if proposalSigma > 1.0:
            sigma2 = 0.4
            m2 = filterWindow(0.0, sigma2)
            ds = np.subtract(m.shape, m2.shape)
            ds1, ds2 = int(ds[0]/2), int(ds[1]/2)
            logger.debug('Adaptive kernel size %d:%d by %d:%d' % (*m.shape, *ds))
            if ds1 > 0 and ds2 > 0:
                m1 = np.zeros(m.shape)
                m1[ds1:-ds1,ds2:-ds2] = m2
                m = m1
        pass
    #print(m)
    img2shape = tuple(np.subtract(img.shape, m.shape)+1)
    logger.debug('Prepare sub-matrices for convolution'+str(img2shape) )
    subMatrices = as_strided(img, m.shape+img2shape, img.strides+img.strides)
    img2 = np.einsum('ij,ijkl->kl', m, subMatrices)
    return img2

def acceptChange(cost, cost0, T=1.0):
    acc = False
    dE = cost - cost0
    if dE < -100.0:
        acc = True
    elif dE > 100.0:
        acc = False
    else:
        p = math.exp(-dE/T)
        p = p/(1+p)
        r = random.uniform(0.0, 1.0)
        if r < p:
            acc = True
    return acc

def proposeValue(i, j, dy, mat):
    # Hyper-parameters
    eta = 1.0
    beta = 0.15
    #
    a0 = mat[i,j]
    a1, a2, a3, a4 = 0, 0, 0, 0
    z = eta
    if i > 0:
        a1 = mat[i-1, j]
        z += beta
    if i < (mat.shape[0]-1):
        a2 = mat[i+1, j]
        z += beta
    if j > 0:
        a3 = mat[i, j-1]
        z += beta
    if j < (mat.shape[1]-1):
        a4 = mat[i, j+1]
        z += beta
    ymean = (eta*a0 + beta*(a1 + a2 + a3 + a4))/z
    #dy1 = dy
    #step = 256/zScale
    #if dy1 < step: dy1 = step
    y = random.gauss(ymean, dy)
    if y < 0.0: y = 0
    elif y >= 255: y = 255
    #s = float(zScale)/256
    #y1 = int(y * s)
    return y
        
def recover(img, sigma,
            proposalSigma=0.1, adaptProposalSigma=False,
            adaptKernelSize=False, 
            chainName='chain', niterations=1,
            recordInterval=1000,
            initialCondition='original',
            scanOrder='rowColumn'):
    nr, nc = img.shape[0], img.shape[1]
    dy = proposalSigma
    #dyMin, dyMax = 0.001, 100.0
    #dyMin, dyMax = 1.0, 100.0
    dyMin, dyMax = 1.0, 3.0
    acceptRateMin, acceptRateMax = 0.1, 0.7
    if initialCondition == 'original':
        imgrec = np.copy(img)
    elif initialCondition == 'zeros':
        imgrec = np.zeros(img.shape)
    elif initialCondition == 'ones':
        imgrec = np.ones(img.shape)
    else:
        imgrec = np.zeros(img.shape)

    dy2 = None
    if args.adaptKernelSize:
        dy2 = dy
    imgrecblurred = blur(imgrec, sigma, dy2)
    ds = np.subtract(img.shape, imgrecblurred.shape)
    ds1, ds2 = int(ds[0]/2), int(ds[1]/2)

    imgsub = img[ds1:-ds1,ds2:-ds2]

    costTooLarge = nr*nc*100
    chain = MarkovChain(chainName, niterations, recordInterval)
    
    cost = np.sum( (imgrecblurred - imgsub)**2)
    cost0 = cost

    def rowColumnGenerator(k, nr, nc):
        if k < (nr*nc):
            i = int(k / nc)
            j = int(k % nc)
            return (i, j)
        else:
            return None
    def randomGenerator(k, nr, nc):
        if k < (nr*nc):
            i = random.randrange(0, nr)
            j = random.randrange(0, nc)
            return (i, j)
        else:
            return None

    nr2 = nr - 2*ds1
    nc2 = nc - 2*ds2
    logger.info('Recover (boundary size: %d, %d, inner=%dx%d)' %\
                (ds1, ds2, nr2, nc2) )
    generator = rowColumnGenerator
    if scanOrder == 'rowColumn':
        generator = rowColumnGenerator
    elif scanOrder == 'random':
        generator = randomGenerator

    # # Fill zeros for boundaries
    # range1 = list(range(ds1)) + list(range(nr-ds1,nr))
    # range2 = list(range(ds2)) + list(range(nc-ds2,nc))
    # for i in range1:
    #     for j in range(nc):
    #         imgrec[i, j] = 0.0
    # for i in range(nr):
    #     for j in range2:
    #         imgrec[i, j] = 0.0

    for iter in range(niterations):
        step = 0
        nTries = 0
        nAccepts = 0

        for k in range(nr*nc):
            ij = generator(k, nr, nc)
            i, j = ij[0], ij[1]
            #i, j = ds1 + i2, ds2 + j2
            y0 = imgrec[i,j]
            y = proposeValue(i, j, dy, imgrec)
            logger.debug('proposed value: %d->%d' % (y0, y) )
            imgrec[i,j] = y
            imgrecblurred = blur(imgrec, sigma)
            cost = np.sum( (imgrecblurred - imgsub)**2)
            logger.debug('(i,j)=(%d,%d) => cost=%10.5f' % (i, j, cost))
            nTries += 1
            acc = acceptChange(cost, cost0)
            if acc:
                cost0 = cost
                nAccepts += 1
            else:
                imgrec[i,j] = y0
            if chain.shallRecord(iter, step, nr*nc):
                acceptRate = float(nAccepts)/nTries
                nTries, nAccepts = 0, 0
                    
                logger.info('Record step iteration=%d step=%d (i,j)=(%d,%d) => cost=%10.5f, acceptRate=%6.2f, dy=%7.3f' %\
                            (iter, step, i, j, cost, acceptRate, dy))
                    
                record = StepRecord(iter, step, cost, acceptRate, dy, imgrec)
                chain.recordStep(record)
                if args.adaptProposalSigma and cost < costTooLarge:
                    if acceptRate < acceptRateMin and dy > dyMin:
                        dy /= 2.0
                    elif acceptRate > acceptRateMax and dy < dyMax:
                        dy *= 2.0
            step += 1
    chain.save()
    return imgrec

def rename(fname, suf):
    fname2 = None
    i = fname.rfind('.')
    if i>=0:
        base = fname[:i]
        suffix = fname[i:]
        fname2 = '%s%s%s' % (base, suf, suffix)
    return fname2

def runBlurImage(args):
    img = None
    if os.path.exists(args.inputImage):
        img = plt.imread(args.inputImage)
    else:
        logger.warning('Cannot find file %s' % args.inputImage)
        return -1
    sigmaBlur = args.sigmaBlur

    imgbw = img
    if len(img.shape) == 3 and img.shape[2] == 3:
        imgbw = bw(img)

    if (args.width>0 and args.height>0) and\
       (args.width < img.shape[1] and args.height < img.shape[0]):
        offset = [ int(x) for x in args.offset.split(',') ]
        logger.info('Extract sub-image (c, r)=[%d,%d] (offset=%d,%d) from [%d,%d]' %\
                    (args.width, args.height, offset[0], offset[1],
                     img.shape[1], img.shape[0]) )
        imgbw = imgbw[offset[1]:offset[1]+args.height,
                      offset[0]:offset[0]+args.width]

    #-----------------------------------------------
    # Main job
    #-----------------------------------------------
    workdir='work_%s' % args.name
    if not os.path.isdir(workdir):
        logger.info('Create work directory %s' % workdir)
        os.mkdir(workdir)
    if args.recoveryMode:
        logger.info('Run recovery')
        imgrec = recover(imgbw, sigmaBlur,
                         initialCondition=args.initialCondition,
                         scanOrder=args.scanOrder, 
                         chainName=args.name, niterations=args.niterations,
                         proposalSigma=args.proposalSigma,
                         adaptProposalSigma=args.adaptProposalSigma,
                         adaptKernelSize=args.adaptKernelSize, 
                         recordInterval=args.recordInterval)
        logger.info('Save recovered images')
        fname_rec = rename(os.path.basename(args.inputImage), '_rec')
        fpath = os.path.join(workdir, fname_rec)
        plt.imsave(fpath, imgrec, cmap='gray')
    else:
        imgblur = blur(imgbw, sigmaBlur)
        fname_bw = rename(args.inputImage, '_bw')
        fname_blurred = rename(args.inputImage, '_blurred')
        logger.info('Save BW and blurred images')
        if fname_bw!=None:
            plt.imsave(os.path.join(workdir, fname_bw), imgbw, cmap='gray')
        if fname_blurred!=None:
            plt.imsave(os.path.join(workdir, fname_blurred), imgblur, cmap='gray')

    #-----------------------------------------------
    # Plot results
    #-----------------------------------------------
    if not args.batchMode:
        fig = plt.figure()
        if args.recoveryMode:
            logger.debug('Show last image')
            ax = fig.add_subplot(2, 2, 1)
            ax.imshow(img)

            ax = fig.add_subplot(2, 2, 2)
            ax.imshow(imgbw, cmap='gray')

            ax = fig.add_subplot(2, 2, 2)
            ax.imshow(imgrec, cmap='gray')

        else:
            logger.info('Plot image %s' % (args.inputImage) )
            ax = fig.add_subplot(2, 2, 1)
            ax.imshow(img)

            ax = fig.add_subplot(2, 2, 2)
            ax.imshow(imgbw, cmap='gray')
            
            ax = fig.add_subplot(2, 2, 3)
            ax.imshow(imgblur, cmap='gray')
        plt.show()
        
if __name__ == '__main__':
    args = parseArgs()
    logLevel = logging.INFO
    if args.logLevel == 'DEBUG':
        logLevel = logging.DEBUG
    elif args.logLevel == 'INFO':
        logLevel = logging.INFO
    elif args.logLevel == 'WARNING':
        logLevel = logging.WARNING
    elif args.logLevel == 'ERROR':
        logLevel = logging.ERROR
    logging.basicConfig(stream=sys.stdout, level=logLevel)
    zScale = args.zScale
    runBlurImage(args)
    
