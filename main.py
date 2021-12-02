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

while 1:
    for event in pygame.event.get():
        if event.type in (QUIT, KEYDOWN):
            sys.exit()

    pygame.display.update()
    pygame.time.delay(100)
