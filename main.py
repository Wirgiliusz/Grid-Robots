import pygame
from pygame.locals import *

from cell import Cell



pygame.init()
screen = pygame.display.set_mode((1600, 900))

cell1 = Cell(10, 10)
cell2 = Cell(200, 200)

screen.blit(cell1.image, (cell1.x, cell1.y))
screen.blit(cell2.image, (cell2.x, cell2.y))



while 1:
    for event in pygame.event.get():
        if event.type in (QUIT, KEYDOWN):
            sys.exit()

    pygame.display.update()
    pygame.time.delay(100)