import pygame
from pygame.locals import *
import sys
import os
import signal

from grid import Grid


os.environ['SDL_VIDEO_WINDOW_POS'] = "%d,%d" % (-15,0)
pygame.init()
screen = pygame.display.set_mode((970, 1020))
screen.fill((48, 46, 46))


fifo_input_path = "/tmp/myfifo_c2p"
fifo_input = open(fifo_input_path, 'r')
print("[INPUT] FIFO opened") 
fifo_output_path = "/tmp/myfifo_p2c"
fifo_output = open(fifo_output_path, 'w')
print("[OUTPUT] FIFO opened") 

paths = []
def readPaths():
    path_data = fifo_input.readline()
    if len(path_data) > 1:
        print("-> Reading messages from [INPUT] FIFO:")
        path_points = path_data.split(" ")
        if len(path_points) > 1:
            path_coordinates = []
            for i in range(0, len(path_points)-1, 2):
                x = int(path_points[i])
                y = int(path_points[i+1])
                path_coordinates.append((x,y))
            print("Path msg: ", path_coordinates, "\n")
            paths.append(path_coordinates)

def sendFinishMessage(robot_coordinates):
    robot_x = robot_coordinates[0]
    robot_y = robot_coordinates[1]
    fifo_output = open(fifo_output_path, 'w')

    message = str(robot_x) + " " + str(robot_y) + "\n"
    print("<- Sending messages to [OUTPUT] FIFO: ")
    fifo_output.write(message)
    print("Robot finished: ", message)

    fifo_output.close()

print("-> Reading messages from [INPUT] FIFO:")

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


print("<- Sending messages to [OUTPUT] FIFO:")
print("Acknowledge msg: success")
fifo_output.write("success")

fifo_output.close()
print("[OUTPUT] FIFO closed") 


grid = Grid(screen, grid_size_x, grid_size_y, 
(storages_coordinates), 
(items_coordinates),
(robots_coordinates))
grid.drawGrid()

play = False
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            sys.exit()
        elif event.type == KEYDOWN:
            play = True
            
    if play:
        if grid.play_animations:
            grid.updateRobotsPositions()
        else:
            if paths:
                for path_coordinates in paths:
                    grid.checkRobotOnItem(path_coordinates[0])
                    grid.checkRobotOnStorage(path_coordinates[0])

                    if len(path_coordinates) > 1:
                        moved = grid.moveRobotFromCoordToCoord(path_coordinates[0], path_coordinates[1])
                        if moved:
                            path_coordinates.pop(0)
                    else:
                        sendFinishMessage(path_coordinates[0])
                        paths.remove(path_coordinates)

        readPaths()

    pygame.display.update()
    pygame.time.delay(1)

fifo_input.close()
print("[INPUT] FIFO closed") 
