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
print("[INPUT] FIFO opened") 
fifo_output_path = "/tmp/myfifo_p2c"
fifo_output = open(fifo_output_path, 'w')
print("[OUTPUT] FIFO opened") 


print("Reading messages from [INPUT] FIFO:")

data = fifo_input.readline()
grid_size = data.split(" ")
grid_size_x = int(grid_size[0])
grid_size_y = int(grid_size[1])
print("Grid size msg: ", grid_size_x, " ", grid_size_y)

data = fifo_input.readline()
storages_points = data.split(" ")
storages_coordinates = []
for i in range(0, len(storages_points)-1, 2):
    x = int(storages_points[i])
    y = int(storages_points[i+1])
    storages_coordinates.append((x,y))
storages_coordinates = tuple(storages_coordinates)
print("Storages points msg: ", storages_coordinates)

data = fifo_input.readline()
items_points = data.split(" ")
items_coordinates = []
for i in range(0, len(items_points)-1, 2):
    x = int(items_points[i])
    y = int(items_points[i+1])
    items_coordinates.append((x,y))
items_coordinates = tuple(items_coordinates)
print("Items points msg: ", items_coordinates)

data = fifo_input.readline()
robots_points = data.split(" ")
robots_coordinates = []
for i in range(0, len(robots_points)-1, 2):
    x = int(robots_points[i])
    y = int(robots_points[i+1])
    robots_coordinates.append((x,y))
robots_coordinates = tuple(robots_coordinates)
print("Robots points msg: ", robots_coordinates)


print("Sending messages to [OUTPUT] FIFO:")

print("Acknowledge msg: success")
fifo_output.write("success")

fifo_output.close()
print("[OUTPUT] FIFO closed") 


grid = Grid(screen, grid_size_x, grid_size_y, 
(storages_coordinates), 
(items_coordinates),
(robots_coordinates))
grid.drawGrid()

paths = []
print("Reading messages from [INPUT] FIFO:")
path_data = fifo_input.readline()
path_points = path_data.split(" ")
path_coordinates = []
for i in range(0, len(path_points)-1, 2):
    x = int(path_points[i])
    y = int(path_points[i+1])
    path_coordinates.append((x,y))
print("Path msg: ", path_coordinates)
paths.append(path_coordinates)

path_data = fifo_input.readline()
path_points = path_data.split(" ")
path_coordinates = []
for i in range(0, len(path_points)-1, 2):
    x = int(path_points[i])
    y = int(path_points[i+1])
    path_coordinates.append((x,y))
print("Path msg: ", path_coordinates)
paths.append(path_coordinates)

fifo_input.close()
print("[INPUT] FIFO closed") 



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
        print("Robots movement")
        grid.updateRobotsPositions()
    else:
        print("Robots stopped")
        if paths:
            for path_coordinates in paths:
                grid.checkRobotOnItem(path_coordinates[0])
                grid.checkRobotOnStorage(path_coordinates[0])

                if len(path_coordinates) > 1:
                    print("Moving on path: ", path_coordinates)

                    moved = grid.moveRobotFromCoordToCoord(path_coordinates[0], path_coordinates[1])
                    print(moved)
                    if moved:
                        path_coordinates.pop(0)
                else:
                    paths.remove(path_coordinates)
                    if not paths:
                        fifo_output = open(fifo_output_path, 'w')
                        print("[OUTPUT] FIFO opened") 

                        print("Sending messages to [OUTPUT] FIFO:")

                        print("End msg: end")
                        fifo_output.write("end")

                        fifo_output.close()
                        print("[OUTPUT] FIFO closed") 


    pygame.display.update()
    pygame.time.delay(10)

