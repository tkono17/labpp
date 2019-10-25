#!/usr/bin/env python3

import os, sys
import tkinter as tk
import dss
import dsgui

if __name__ == '__main__':
    #----------------------------------------------------------------
    # Configuration
    #----------------------------------------------------------------
    #--------------------------------------
    # System parameters
    #--------------------------------------
    systemX, systemY = 1000, 1000 # System frame
    boundaryPoints = [ [0, 0], [1000, 0], [1000, 1000], [0, 1000] ]
    nTypes = 2
    T = 1
    #--------------------------------------
    # Particle properties
    #--------------------------------------
    dss.config.particleProperties[0].n = 1
    dss.config.particleProperties[0].mass = 1.0
    dss.config.particleProperties[0].radius = 10.0
    dss.config.particleProperties[0].T = 1.0
    dss.config.particleProperties[0].color = '#0000ff'
    dss.config.particleProperties[1].n = 1
    dss.config.particleProperties[1].mass = 0.8
    dss.config.particleProperties[1].radius = 15.0
    dss.config.particleProperties[1].T = 2.0
    dss.config.particleProperties[1].color = '#001010'
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
    ds.openFile()
    panel = dsgui.GuiPanel(frameX, frameY, ds)
    panel.buildGui()
    ds.closeFile()

