import pygame

class Cell:
    def __init__(self, x, y) -> None:
        self.image = pygame.image.load('img/cell.png').convert()
        self.x = x
        self.y = y

