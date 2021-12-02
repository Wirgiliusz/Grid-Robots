import pygame
import sys
from pygame.locals import *

from grid import Grid


pygame.init()
screen = pygame.display.set_mode((1600, 900))
screen.fill((48, 46, 46))

grid = Grid(screen, 6, 6, 
((1,5), (3,5), (5,5)), 
((2,3), (4,4)),
((0,0), (2,0)))
grid.drawGrid()

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            sys.exit()
        elif event.type == KEYDOWN:
            if event.key == pygame.K_w:
                print("W pressed")
                grid.updateRobotsPositions()


    pygame.display.update()
    pygame.time.delay(100)
