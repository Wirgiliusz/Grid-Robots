from cell import Cell
from storage import Storage
from item import Item

class Grid:
    screen_size_x = 1600
    screen_size_y = 900
    cell_size = 100
    item_size = 40

    grid = []
    items_grid = []

    def __init__(self, screen, x, y, storages_coordinates, items_coordinates) -> None:
        self.screen = screen
        self.x = x
        self.y = y

        self.offset_x = int((self.screen_size_x - x*self.cell_size)/2)
        self.offset_y = int((self.screen_size_y - y*self.cell_size)/2)

        for i in range(self.x):
            tmp = []
            tmp_item = []
            for j in range(self.y):
                tmp.append(Cell(i, j))
                tmp_item.append(0)
            self.grid.append(tmp)
            self.items_grid.append(tmp_item)

        for coord in storages_coordinates:
            i = coord[0]
            j = coord[1]
            self.grid[i][j] = Storage(i, j)

        for coord in items_coordinates:
            i = coord[0]
            j = coord[1]
            self.items_grid[i][j] = Item(i, j)

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

    def drawItems(self):
        for row in self.items_grid:
            for item in row:
                if item:
                    self.screen.blit(item.image, self.itemGridToScreenPos(item.i, item.j))
        