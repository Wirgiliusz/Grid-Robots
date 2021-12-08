import pygame
from pygame.locals import *
import sys
import os

from grid import Grid

pygame.init()
screen = pygame.display.set_mode((1600, 900))
screen.fill((48, 46, 46))

grid = Grid(screen, 6, 6, 
((1,5), (3,5), (5,5)), 
((2,3), (4,4)),
((0,0), ))
grid.drawGrid()

'''
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            sys.exit()
        elif event.type == KEYDOWN:
            if event.key == pygame.K_w:
                print("W pressed")
                grid.moveRobot("N")
            if event.key == pygame.K_s:
                print("S pressed")
                grid.moveRobot("S")
            if event.key == pygame.K_a:
                print("A pressed")
                grid.moveRobot("W")
            if event.key == pygame.K_d:
                print("D pressed")
                grid.moveRobot("E")

    if grid.play_animations:
        grid.updateRobotsPositions()


    pygame.display.update()
    pygame.time.delay(100)
'''

fifo_input_path = "/tmp/myfifo_c2p"
fifo_input = open(fifo_input_path, 'r')
print("FIFO opened")    
data = fifo_input.readline()
print("Read: ", data)
data = fifo_input.readline()
print("Read: ", data)

