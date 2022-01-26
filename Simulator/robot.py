import pygame

class Robot:
    animation_step = 0
    movement = False

    def __init__(self, i, j) -> None:
        self.empty_robot_image = pygame.image.load('img/robot.png').convert_alpha()
        self.empty_robot_image = pygame.transform.scale(self.empty_robot_image, (100, 100))
        self.item_robot_image_0 = pygame.image.load('img/robot_item_box.png').convert_alpha()
        self.item_robot_image_0 = pygame.transform.scale(self.item_robot_image_0, (100, 100))
        self.item_robot_image_1 = pygame.image.load('img/robot_item_circle.png').convert_alpha()
        self.item_robot_image_1 = pygame.transform.scale(self.item_robot_image_1, (100, 100))
        self.item_robot_image_2 = pygame.image.load('img/robot_item_triangle.png').convert_alpha()
        self.item_robot_image_2 = pygame.transform.scale(self.item_robot_image_2, (100, 100))

        self.image = self.empty_robot_image
        self.i = i
        self.j = j

    def moveToCoord(self, i_new, j_new):
        if abs(self.i - i_new) > 0.1:
            movement_i = True
        else:
            movement_i = False
        if abs(self.j - j_new) > 0.1:
            movement_j = True
        else:
            movement_j = False

        if movement_i:
            self.animation_positions_i = [x * (i_new - self.i)/10 + self.i for x in range(1, 11)]
            self.animation_positions_j = [self.j] * 10
        elif movement_j:
            self.animation_positions_j = [x * (j_new - self.j)/10 + self.j for x in range(1, 11)]
            self.animation_positions_i = [self.i] * 10

        self.movement = True

    def playAnimation(self):
        if self.animation_step < 10:
            self.i = self.animation_positions_i[self.animation_step]
            self.j = self.animation_positions_j[self.animation_step]
            self.animation_step += 1
        else:
            self.animation_step = 0
            self.movement = False
        
    def changeImage(self, has_item, img_idx):
        if has_item:
            if img_idx == 0:
                self.image = self.item_robot_image_0
            elif img_idx == 1:
                self.image = self.item_robot_image_1
            if img_idx == 2:
                self.image = self.item_robot_image_2
        else:
            self.image = self.empty_robot_image

