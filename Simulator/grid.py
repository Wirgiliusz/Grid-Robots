from cell import Cell
from storage import Storage
from item import Item
from robot import Robot

class Grid:
    screen_size_x = 970
    screen_size_y = 1020
    cell_size = 100
    item_size = 40

    grid = []
    items_grid = []
    robots_grid = []

    play_animations = False

    def __init__(self, screen, grid_coordinates, 
    storages_coordinates, 
    items_coordinates, 
    robot_coordinates) -> None:
        self.screen = screen
        self.x = grid_coordinates[0]
        self.y = grid_coordinates[1]

        self.offset_x = int((self.screen_size_x - self.x*self.cell_size)/2)
        self.offset_y = int((self.screen_size_y - self.y*self.cell_size)/2)

        for i in range(self.x):
            tmp = []
            tmp_item = []
            tmp_robot = []
            for j in range(self.y):
                tmp.append(Cell(i, j))
                tmp_item.append(0)
                tmp_robot.append(0)
            self.grid.append(tmp)
            self.items_grid.append(tmp_item)
            self.robots_grid.append(tmp_robot)

        for coord in storages_coordinates:
            i = coord[0]
            j = coord[1]
            self.grid[i][j] = Storage(i, j)

        for coord in items_coordinates:
            i = coord[0]
            j = coord[1]
            self.items_grid[i][j] = Item(i, j)

        for coord in robot_coordinates:
            i = coord[0]
            j = coord[1]
            self.robots_grid[i][j] = Robot(i, j)

    def gridToScreenPos(self, i, j):
        x = self.offset_x + i*self.cell_size
        y = self.offset_y + j*self.cell_size
        return (x, y)

    def itemGridToScreenPos(self, i, j):
        x = self.offset_x + i*self.cell_size + (self.cell_size-self.item_size)/2
        y = self.offset_y + j*self.cell_size + (self.cell_size-self.item_size)/2
        return (x, y)

    def drawGrid(self):  
        for row in self.grid:
            for obj in row:
                self.screen.blit(obj.image, self.gridToScreenPos(obj.i, obj.j))
        self.drawItems()
        self.drawRobots()

    def drawItems(self):
        for row in self.items_grid:
            for item in row:
                if item:
                    self.screen.blit(item.image, self.itemGridToScreenPos(item.i, item.j))

    def drawRobots(self):
        for row in self.robots_grid:
            for robot in row:
                if robot:
                    self.screen.blit(robot.image, self.gridToScreenPos(robot.i, robot.j))

    def updateRobotsPositions(self):
        played_animation = False
        for row in self.robots_grid:
            for robot in row:
                if robot:
                    if robot.movement:
                        robot.playAnimation()
                        played_animation = True
        self.drawGrid()
        if not played_animation:
            self.play_animations = False


    def moveRobot(self, dir):
        for row in self.robots_grid:
            for robot in row:
                if robot:
                    if not robot.movement:
                        if dir == "N":
                            robot.moveToCoord(robot.i, robot.j-1)
                        elif dir == "S":
                            robot.moveToCoord(robot.i, robot.j+1)
                        elif dir == "W":
                            robot.moveToCoord(robot.i-1, robot.j)
                        elif dir == "E":
                            robot.moveToCoord(robot.i+1, robot.j+1)
        self.play_animations = True

    def moveRobotFromCoordToCoord(self, robot_coord, destination_coord):
        moved = False
        robot_i = robot_coord[0]
        robot_j = robot_coord[1]
        dest_i = destination_coord[0]
        dest_j = destination_coord[1]
        robot = self.robots_grid[robot_i][robot_j]
        robot_on_dest = self.robots_grid[dest_i][dest_j]
        if robot and not robot_on_dest:
            if not robot.movement:
                robot.moveToCoord(dest_i, dest_j)
                self.robots_grid[dest_i][dest_j] = self.robots_grid[robot_i][robot_j]
                self.robots_grid[robot_i][robot_j] = 0
                moved = True
        
        self.play_animations = True
        return moved

    def checkRobotOnItem(self, robot_coord):
        robot_i = robot_coord[0]
        robot_j = robot_coord[1]

        if self.items_grid[robot_i][robot_j]:
            self.robots_grid[robot_i][robot_j].changeImage(True, self.items_grid[robot_i][robot_j].img_idx)
            self.items_grid[robot_i][robot_j] = 0
            self.drawGrid()

    def checkRobotOnStorage(self, robot_coord):
        robot_i = robot_coord[0]
        robot_j = robot_coord[1]

        if isinstance(self.grid[robot_i][robot_j], Storage) and isinstance(self.robots_grid[robot_i][robot_j], Robot):
            self.robots_grid[robot_i][robot_j].changeImage(False, 0)
            self.drawGrid()
            return True
