#!/usr/bin/env python3

import os, sys
import tkinter as tk
import dss
import dsgui
import argparse
import importlib
import time
import json

def globalConfig():
    #--------------------------------------
    # Particle properties
    #--------------------------------------
    eV = dss.config.scaleSet.constants.e
    dss.config.scaleSet.setFromEMX(E=1.0*eV, M=1.0e+6*eV, X=100.0e-10)
    dss.config.deltaT = 10.0e-15/dss.config.scaleSet.T
    r0 = 4.0e-10/dss.config.scaleSet.X
    dss.config.LJPotential_e = 0.2
    dss.config.LJPotential_r0 = r0
    dss.config.sigmaX = 2*r0
    dss.config.particleProperties[0].n = 300
    dss.config.particleProperties[0].mass = 938.0
    dss.config.particleProperties[0].radius = r0/4.0
    dss.config.particleProperties[0].T = 300.0
    dss.config.particleProperties[0].color = '#a3b4c5'
    dss.config.particleProperties[1].n = 0
    dss.config.particleProperties[1].mass = 938.0
    dss.config.particleProperties[1].radius = r0/4.0
    dss.config.particleProperties[1].T = 200.0
    dss.config.particleProperties[1].color = '#001010'

def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', '--example', dest='showExample', 
                        action='store_true', default=False, 
                        help='Show example')
    parser.add_argument('-c', '--config-file', dest='configFile', 
                        action='store', type=str, default='', 
                        help='Configuration file')
    parser.add_argument('-o', '--output-file', dest='outputFile', 
                        action='store', type=str, default='dss.json', 
                        help='Output file')
    parser.add_argument('-i', '--input-file', dest='inputFile', 
                        action='store', type=str, default='in.json', 
                        help='Input file')
    parser.add_argument('-n', '--number-of-steps', dest='numberOfSteps', 
                        action='store', type=int, default=0, 
                        help='Number of steps')
    parser.add_argument('-m', '--run-mode', dest='runMode', 
                        action='store', type=str, default='gui', 
                        help='Run mode. gui|batch|replay [gui]')
    parser.add_argument('-s', '--save-mode', dest='saveMode', 
                        action='store', type=int, default=0, 
                        help='Specify how often one wants to save data. A positive number means to save results every N steps. 0: default. The first 100 steps are always saved.')
    parser.add_argument('-r', '--replay', dest='replay', 
                        action='store_true', default=False, 
                        help='Replay from an existing json file (input-file)')
    parser.add_argument('-u', '--update-method', dest='updateMethod', 
                        action='store', type=str, default='runTimeStep', 
                        help='Select the update method: runTimeStep|MC')
    args = parser.parse_args()
    return args

def showExample():
    prog=os.path.basename(sys.argv[0])
    print('Examples:')
    print('  > %s -o out.json' % (prog) )
    print('  > %s -c config.py -o out.json -s 10 -u MC -m batch -n 100' % (prog) )
    print('  > %s -c config.py -o out.json -s 10 -u runTimeStep' % (prog) )
    print('  > %s -m replay -i in.json' % (prog) )
    print('  > %s -b -c config.py -o out.json -s 10' % (prog) )

if __name__ == '__main__':
    #----------------------------------------------------------------
    # Configuration
    #----------------------------------------------------------------
    globalConfig()
    args = parseArgs()

    if args.showExample:
        showExample()
        sys.exit(1)

    if args.configFile != '':
        fdir=os.path.dirname(args.configFile)
        fname=os.path.basename(args.configFile)
        if fdir=='':
            fdir='.'
        sys.path.append(fdir)
        mname = fname.replace('.py', '')
        myConfig = importlib.import_module(mname)
        exec('myConfig.globalConfig()')
        print('Overwrite globalConfig from %s' % args.configFile)

    #--------------------------------------
    # System parameters
    #--------------------------------------
    systemX, systemY = 1.0, 1.0 # System frame
    boundaryPoints = [ [0, 0], [systemX, 0], [systemX, systemY], [0, systemY] ]
    nTypes = 2 # Number of particle types
    T = 1 # Temperature
    #--------------------------------------
    # Graphical parameters
    #--------------------------------------
    frameX, frameY = 800, 800 # GUI frame
    #
    #----------------------------------------------------------------
    # Setup the system
    #----------------------------------------------------------------
    #

    ds = dss.PSystem(systemX, systemY, boundaryPoints, nTypes, T)
    # Set options
    ds.outputFilename = args.outputFile
    ds.saveMode = args.saveMode
    #
    if args.runMode == 'replay':
        print('Replay from the file %s' % args.inputFile)
        if not os.path.exists(args.inputFile):
            print('  The input file is mandatory but does not exist: %s' %\
                  args.inputFile)
            sys.exit(-1)
        print('Recreate RelaySystem')
        ds = dss.ReplaySystem(systemX, systemY, boundaryPoints, nTypes, T)
        ds.setup()
        panel = dsgui.GuiPanel(frameX, frameY, ds)
        panel.buildGui()
        #
        fin = open(args.inputFile, 'r')
        print('Reading data from JSON file from %s .....' % args.inputFile)
        data = json.loads(fin.read() )
        print('Loaded json data from')
        panel.jsonEvents = data['events']
        panel.startLoop()
        pass
    elif args.runMode == 'batch':
        ds.setup()
        dss.config.printIt()
        ds.openFile()
        ds.selectUpdateMethod(args.updateMethod)
        #
        ds.generateBalls()
        i=0
        while True:
            ds.update()
            i += 1
            if i >= args.numberOfSteps:
                break
    elif args.runMode == 'gui':
        ds.setup()
        dss.config.printIt()
        ds.openFile()
        ds.selectUpdateMethod(args.updateMethod)
        panel = dsgui.GuiPanel(frameX, frameY, ds)
        panel.buildGui()
        panel.startLoop()
    else:
        print('Unknown run mode %s. Run in GUI mode' % args.runMode)
        ds.setup()
        dss.config.printIt()
        ds.openFile()
        ds.selectUpdateMethod(args.updateMethod)
        panel = dsgui.GuiPanel(frameX, frameY, ds)
        panel.buildGui()
        panel.startLoop()
    ds.closeFile()

