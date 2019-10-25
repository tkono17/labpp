#!/usr/bin/env python3

import tkinter as tk
import dss

gDS=None
gTkroot=None

def setupGui():
    root = tk.Tk()
    root.title('Dynamical System Simulation')
    root.geometry('800x600')
    canvas = tk.Canvas(root, width=600, height=600)
    canvas.create_rectangle(0, 0, 600, 600, fill='#a0c080')
    canvas.place(x=200, y=0)
    # Init button
    svInit = tk.StringVar()
    svInit.set('Init')
    buttonInit = tk.Button(root, textvariable=svInit, 
                            foreground='#0000ff', 
                            command=buttonInitClicked)
    buttonInit.place(x=50, y=50)
    # Start button
    svStart = tk.StringVar()
    svStart.set('Start')
    buttonStart = tk.Button(root, textvariable=svStart, 
                            foreground='#0000ff', 
                            command=buttonStartClicked)
    buttonStart.place(x=50, y=100)
    # Quit button
    svQuit = tk.StringVar()
    svQuit.set('Quit')
    buttonQuit = tk.Button(root, textvariable=svQuit, 
                            foreground='#0000ff', 
                            command=buttonQuitClicked)
    buttonQuit.place(x=50, y=130)
    #
    global gTkroot
    gTkroot = root
    root.mainloop()

def drawSystem(ds):
    print(gTkroot)
    print('keys=', gTkroot.children.keys() )
    for k in gTkroot.children.keys():
        print(k)
        print('winfo_name:', gTkroot.children[k].winfo_name())
        print('winfo_screen:', gTkroot.children[k].winfo_screen())
    canvas = gTkroot.children['!canvas']
    for w in ds.walls:
        canvas.create_line(w.position1[0], w.position1[1], w.position2[0], w.position2[1], fill='black')
        

def buttonInitClicked():
    print('Generate initial state')
    gDS = dss.System1()
    gDS.setup()
    drawSystem(gDS)

def buttonStartClicked():
    print('ButtonStartClicked')

def buttonQuitClicked():
    print('Quitting ...')
    gTkroot.quit()

def runSim():
    root = setupGui()

if __name__=='__main__':
    runSim()
