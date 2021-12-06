import pygame

class Storage:
    def __init__(self, i, j) -> None:
        self.image = pygame.image.load('img/storage.png').convert()
        self.image = pygame.transform.scale(self.image, (100, 100))
        self.i = i
        self.j = j
