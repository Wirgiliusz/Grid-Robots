import pygame
from random import randint

class Item:
    def __init__(self, i, j) -> None:
        images = ('img/item_box.png', 'img/item_circle.png', 'img/item_triangle.png')
        self.img_idx = randint(0, 2)
        self.image = pygame.image.load(images[self.img_idx]).convert_alpha()
        self.image = pygame.transform.scale(self.image, (40, 40))
        self.i = i
        self.j = j
