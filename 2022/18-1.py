with open("18-1.txt","r") as file:
    text = file.readlines()

class cube:
    def __init__(self, x, y, z):
        self.x = int(x)
        self.y = int(y)
        self.z = int(z)
        self.covered  = 0
    def pos (self):
        return self.x,self.y,self.z
    def scan(self):
        for offset in [[1, 0, 0], [-1, 0, 0], [0, 1, 0], [0, -1, 0], [0, 0, 1], [0, 0, -1]]:
            if any((self.x + offset[0],self.y + offset[1],self.z + offset[2])== c.pos() for c in cubes):
                self.covered += 1
                #print("collision")
                
    
        
cubes = set(cube(*(line.strip("\n").split(","))) for line in text)

for cube in cubes:
    cube.scan()

sum_exposed = 0
for cube in cubes:
    if cube.covered < 6:
        sum_exposed += 6 - cube.covered 

print(sum_exposed)
