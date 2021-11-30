import pygame

class Cell:
    def __init__(self, i, j) -> None:
        self.image = pygame.image.load('img/cell.png').convert()
        self.image = pygame.transform.scale(self.image, (100, 100))
        self.i = i
        self.j = j
