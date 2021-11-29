from cell import Cell

class Grid:
    screen_size_x = 1600
    screen_size_y = 900

    cell_size = 100

    cells = []

    def __init__(self, screen, x ,y) -> None:
        self.screen = screen
        self.x = x
        self.y = y

        offset_x = int((self.screen_size_x - x*self.cell_size)/2)
        offset_y = int((self.screen_size_y - y*self.cell_size)/2)

        for i in range(offset_x, self.screen_size_x - offset_x, self.cell_size):
            for j in range(offset_y, self.screen_size_y - offset_y, self.cell_size):
                    cell = Cell(i, j)
                    self.cells.append(cell)

    def drawGrid(self):        
        for cell in self.cells:
            self.screen.blit(cell.image, cell.pos)\
