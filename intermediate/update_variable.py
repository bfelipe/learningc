class Coordinates:
    def __init__(self, x = 1, y = 2):
        self.x = x
        self.y = y

# we pass a reference or a pointer to our coord instance
def update_x(coord: Coordinates, x: int):
    print(id(coord))
    coord.x = x

coord = Coordinates()
print(id(coord))
update_x(coord, 3)
print(coord.x)