#!/usr/bin/env python3

import os, sys
import tkinter as tk
import dss
import dsgui

def globalConfig():
    #--------------------------------------
    # Particle properties
    #--------------------------------------
    eV = dss.config.scaleSet.constants.e
    dss.config.scaleSet.setFromEMX(E=1.0*eV, M=1.0e+6*eV, X=100.0e-10)
    dss.config.deltaT = 1.0e-15/dss.config.scaleSet.T
    r0 = 4.0e-10/dss.config.scaleSet.X
    dss.config.LJPotential_e = 0.2
    dss.config.LJPotential_r0 = r0
    dss.config.particleProperties[0].n = 100
    dss.config.particleProperties[0].mass = 938.0
    dss.config.particleProperties[0].radius = r0/4.0
    dss.config.particleProperties[0].T = 300.0
    dss.config.particleProperties[0].color = '#a3b4c5'
    dss.config.particleProperties[1].n = 0
    dss.config.particleProperties[1].mass = 938.0
    dss.config.particleProperties[1].radius = r0/4.0
    dss.config.particleProperties[1].T = 200.0
    dss.config.particleProperties[1].color = '#001010'
    
if __name__ == '__main__':
    #----------------------------------------------------------------
    # Configuration
    #----------------------------------------------------------------
    globalConfig()
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
    ds.setup()
    dss.config.printIt()
    ds.openFile()
    panel = dsgui.GuiPanel(frameX, frameY, ds)
    panel.buildGui()
    ds.closeFile()

