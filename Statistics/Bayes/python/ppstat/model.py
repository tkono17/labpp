#-----------------------------------------------------------------------
# ppstat:
#-----------------------------------------------------------------------

import os
import logging
import pickle

import matplotlib.pyplot as plt

logger = logging.getLogger(__name__)

class StepRecord:
    def __init__(self, iteration, step, cost, acceptRate, proposalSigma,
                 img=None):
        self.iteration = iteration
        self.step = step
        self.cost = cost
        self.acceptRate = acceptRate
        self.proposalSigma = proposalSigma
        self.imageFile = ''
        self.image = img

class MarkovChain:
    def __init__(self, name, niterations, recordInterval):
        self.name = name
        self.niterations = niterations
        self.recordInterval = recordInterval
        self.records = []
    def shallRecord(self, iteration, step, stepMax=-1):
        s = False
        if step == 0 or (step % self.recordInterval) == 0:
            s = True
        if stepMax > 0 and step == (stepMax-1):
            s = True
        return s
    def workDir(self):
        return 'work_%s' % self.name
    def recordStep(self, record):
        workdir = self.workDir()
        if not os.path.isdir(workdir):
            os.mkdir(workdir)
        if type(record.image) != type(None):
            imgFile = '%s/%s_iter%d_step%d.jpg' %\
                (workdir, self.name, record.iteration, record.step)
            if not os.path.exists(imgFile):
                plt.imsave(imgFile, record.image, cmap='gray')
        record.image = None
        self.records.append(record)
    def save(self):
        w = self.workDir()
        if not os.path.isdir(w):
            logger.info('Create working directory for the Markov chain %s' % self.name)
            os.mkdir(w)
        fname1 = 'tmp_%s.pickle' % self.name
        fname2 = '%s.pickle' % self.name
        logger.info('Save Markov chain steps to %s' % fname2)
        f = open(fname1, 'wb')
        pickle.dump(self, f)
        f.close()
        os.rename(fname1, fname2)
