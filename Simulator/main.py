import pygame
from pygame.locals import *
import sys
import os

from grid import Grid

pygame.init()
screen = pygame.display.set_mode((1600, 900))
screen.fill((48, 46, 46))

fifo_input_path = "/tmp/myfifo_c2p"
fifo_input = open(fifo_input_path, 'r')
print("FIFO opened") 

data = fifo_input.readline()
grid_size = data.split(" ")
print(grid_size)
grid_size_x = int(grid_size[0])
grid_size_y = int(grid_size[1])
print(grid_size_x, " ", grid_size_y)

data = fifo_input.readline()
storages_points = data.split(" ")
print(storages_points)
storages_coordinates = []
for i in range(0, len(storages_points)-1, 2):
    x = int(storages_points[i])
    y = int(storages_points[i+1])
    storages_coordinates.append((x,y))
storages_coordinates = tuple(storages_coordinates)
print(storages_coordinates)

data = fifo_input.readline()
items_points = data.split(" ")
print(items_points)
items_coordinates = []
for i in range(0, len(items_points)-1, 2):
    x = int(items_points[i])
    y = int(items_points[i+1])
    items_coordinates.append((x,y))
items_coordinates = tuple(items_coordinates)
print(items_coordinates)

data = fifo_input.readline()
robots_points = data.split(" ")
print(robots_points)
robots_coordinates = []
for i in range(0, len(robots_points)-1, 2):
    x = int(robots_points[i])
    y = int(robots_points[i+1])
    robots_coordinates.append((x,y))
robots_coordinates = tuple(robots_coordinates)
print(robots_coordinates)


fifo_input.close()
print("FIFO closed") 

grid = Grid(screen, grid_size_x, grid_size_y, 
(storages_coordinates), 
(items_coordinates),
(robots_coordinates))
grid.drawGrid()

'''
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

