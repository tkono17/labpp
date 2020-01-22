#!/usr/bin/env python3

import os, sys
import tkinter as tk
import dss

class GuiPanel:
    def __init__(self, frameX, frameY, ds=None):
        self.frameX = frameX
        self.frameY = frameY
        self.ds = ds
        self.replayEventNumber = -1
        pass

    def buttonQuitClicked(self):
        print('Button[Quit] clicked')
        self.root.quit()

    def buttonInitClicked(self):
        print('Button[Init] clicked')
        print('Number of walls: %d' % len(self.ds.walls) )
        self.generateBalls()
        self.drawWalls()
        self.drawBalls()

    def generateBalls(self):
        self.ds.generateBalls()
    def drawWalls(self):
        self.canvas.delete('wall')
        for wall in self.ds.walls:
            x1, y1 = wall.position1[0], wall.position1[1]
            x2, y2 = wall.position2[0], wall.position2[1]
            (x1, y1) = self.systemToGui( (x1, y1) )
            (x2, y2) = self.systemToGui( (x2, y2) )
            print('Create wall: ', (x1, y1, x2, y2) )
            self.canvas.create_line(x1, y1, x2, y2, fill='#000000', tag='wall')
    def drawBalls(self):
        self.canvas.delete('ball')
        rmin = 3.0e-3
        for ball in self.ds.balls:
            p = ball.position
            r = ball.radius
            if r < rmin: r = rmin
            x1, y1 = self.systemToGui( (p[0]-r, p[1]-r) )
            x2, y2 = self.systemToGui( (p[0]+r, p[1]+r) )
            pp = dss.config.particleProperties[ball.ptype]
            c = pp.color
            self.canvas.create_oval(x1, y1, x2, y2, fill=c, tag='ball')
        
    def update(self):
        # if self.nupdate >= 10000:
        #     print('Finish loop')
        #     return
        #print('GUI update n=', self.nupdate)
        self.ds.update()
        if (self.nupdate%1)==0:
            self.drawBalls()
        if dss.config.stopRequest:
            print('Stop running due to StopRequest')
        else:
            self.canvas.after(10, self.update)
            self.nupdate += 1

    def replay(self):
        if self.replayEventNumber < 0:
            self.drawWalls()
            self.replayEventNumber = 0
        if self.replayEventNumber >= len(self.jsonEvents):
            return
        e = self.jsonEvents[self.replayEventNumber]
        t = e['timeStep']
        print('  Replay timeStep=%d' % t)
        balls = e['balls']
        self.ds.setBalls(balls)
        self.drawBalls()
        #
        self.replayEventNumber += 1
        interval = 100
        if hasattr(dss.config, 'replayInterval'):
            interval = dss.config.replayInterval
        self.canvas.after(interval, self.replay)

    def buttonStartClicked(self):
        print('Button[Start] clicked')
        dss.config.setStartRequest(True)
        self.nupdate = 0
        self.canvas.after(50, self.update)

    def buttonPauseClicked(self):
        print('Button[Pause] clicked')
        dss.config.setStopRequest(True)

    def buttonResumeClicked(self):
        print('Button[Resume] clicked')
        print('Nothing happens')

    def buttonStopClicked(self):
        print('Button[Stop] clicked')
        dss.config.setStopRequest(True)

    def buttonReplayClicked(self):
        print('Button[Replay] clicked')
        dss.config.setStopRequest(True)
        self.canvas.after(50, self.replay)
        print('Replay has finished')

    def buildGui(self):
        sideX = 200
        self.root = tk.Tk()
        self.root.title('Dynamical System Simulation')
        self.root.geometry('%dx%d' % (self.frameX+sideX, self.frameY) )
        self.canvas = tk.Canvas(master=self.root, 
                                width=self.frameX, height=self.frameY, )
        self.canvas.create_rectangle(0, 0, self.frameX, self.frameY, 
                                     fill='#a0c080')
        self.canvas.place(x=sideX, y=0)
        #
        self.buttonQuit = tk.Button(master=self.root, 
                                    text='Quit', 
                                    foreground='#0000ff', 
                                    command=self.buttonQuitClicked)
        self.buttonQuit.place(x=130, y=20)
        #
        self.buttonInit = tk.Button(master=self.root, 
                                    text='Generate initial state', 
                                    foreground='#0000ff', 
                                    command=self.buttonInitClicked)
        self.buttonInit.place(x=20, y=100)
        #
        self.buttonStart = tk.Button(master=self.root, 
                                    text='Start|Restart', 
                                    foreground='#0000ff', 
                                    command=self.buttonStartClicked)
        self.buttonStart.place(x=20, y=150)
        #
        self.buttonPause = tk.Button(master=self.root, 
                                    text='Pause', 
                                    foreground='#0000ff', 
                                    command=self.buttonPauseClicked)
        self.buttonPause.place(x=20, y=200)
        #
        self.buttonResume = tk.Button(master=self.root, 
                                      text='Resume', 
                                      foreground='#0000ff', 
                                      command=self.buttonResumeClicked)
        self.buttonResume.place(x=120, y=200)
        #
        self.buttonStop = tk.Button(master=self.root, 
                                    text='Stop', 
                                    foreground='#0000ff', 
                                    command=self.buttonStopClicked)
        self.buttonStop.place(x=20, y=250)
        #
        self.buttonReplay = tk.Button(master=self.root, 
                                      text='Replay', 
                                      foreground='#0000ff', 
                                      command=self.buttonReplayClicked)
        self.buttonReplay.place(x=20, y=250)
        #
    def startLoop(self):
        self.root.mainloop()
        self.replayEventNumber = -1

    def systemToGui(self, pos):
        marginX, marginY=10, 10
        x, y = pos
        sx, sy = self.ds.sx, self.ds.sy
        fx, fy = self.frameX, self.frameY
        rx = float(fx-2.0*marginX)/sx
        ry = float(fy-2.0*marginY)/sy
        x = marginX + rx*x
        y = fy - ry*y - marginY
        return (int(x), int(y) )
        
if __name__ == '__main__':
    print('No main program')

