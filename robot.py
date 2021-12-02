import pygame

class Robot:
    def __init__(self, i, j) -> None:
        self.image = pygame.image.load('img/robot.png').convert_alpha()
        self.image = pygame.transform.scale(self.image, (100, 100))
        self.i = i
        self.j = j
