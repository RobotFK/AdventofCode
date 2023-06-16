with open("18-1.txt","r") as file:
    text = file.readlines()

cubes = set()
off_by_one = [[1, 0, 0], [-1, 0, 0], [0, 1, 0], [0, -1, 0], [0, 0, 1], [0, 0, -1]]
class cube:
    def __init__(self, x, y, z):
        self.x = int(x)
        self.y = int(y)
        self.z = int(z)
        self.covered  = 0
        self.covered_space = 0
    def __eq__(self, other): #Allows comparision
        return(all(( self.x == other.x , self.y == other.y,self.z == other.z)))
    def __hash__(self): # Allows comparision while still allowing hashing  (for sets)
        return hash((self.x, self.y, self.z))
    def pos (self):
        return self.x,self.y,self.z
    def scan(self, scanset): #scans in relation to a set of other cubes
        self.covered  = 0
        for offset in off_by_one:
            if any((self.x + offset[0],self.y + offset[1],self.z + offset[2])== c.pos() for c in scanset):
                self.covered += 1
    def scan_space(self, scanset):#scans in reltation to a 3D array
        self.covered_space  = 0
        for offset in off_by_one:
            if self.x + offset[0] not in range(len(scanset)): continue
            if self.y + offset[1] not in range(len(scanset[0])): continue
            if self.z + offset[2] not in range(len(scanset[0][0])): continue
            #print(offset)
            #print(self.pos())
            if scanset[self.x + offset[0]][self.y + offset[1]][self.z + offset[2]] != 0:
                self.covered_space  +=1
        #if self.covered_space != 6: print(self.covered_space)
       
    #Todo write flooding algorithem
    
cubes = set(cube(*(line.strip("\n").split(","))) for line in text)

for icube in cubes:
    #continue #Allows for Faster Debugging of Part 2
    icube.scan(cubes)

sum_exposed = 0
counter = [0]*7
for icube in cubes:
    counter[icube.covered] += 1
    if icube.covered < 6:
        sum_exposed += 6 - icube.covered 

print("Part1:"+str(sum_exposed))
print("Part1 Distribution:"+str(counter))
#This is an 18*19*19 space
space = [[[0 for z in range(20)]for y in range(20)]for x in range(19)] #Back to the old style...

for icube in cubes:
    x, y, z = icube.pos()
    space[x][y][z] = 1

#Next we iterate through 2D slices trying to find airpockets
for x, x_slice in enumerate(space): #2D slices
    if x in [0, len(space)-1]: continue
    for y, y_slice in enumerate(x_slice): #1D sclices
        #print(y_slice)
        if not any(z for z in y_slice if z == 1): continue
        if y in [0, len(x_slice)-1]: continue
        #We search extra rudementairy for possible holes if there are empty spaces lower and higher than cubes in the 1D slice
        z_min = next(z for z,value in enumerate(y_slice) if value == 1)
        z_max = 19-next(z for z,value in enumerate(reversed(y_slice)) if value == 1) #Dont ask why 19
        space[x][y] = [2 if i > z_min and i < z_max and icube == 0 else icube for i,icube in enumerate(y_slice)]#Detects a 0 inbetween 1's and change value of theses Entrys to 2
    #print("\n")
    
cropped = True
amount = 0
while(cropped):#We slowly elminate any falsly marked spaces by iterating over positions being potential holes and check if they touch the outside
    cropped = False
    for x, x_slice in enumerate(space): #2D slices
        if x in [0, len(space)-1]: continue
        for y, y_slice in enumerate(x_slice): #1D sclices
            if y in [0, len(x_slice)-1]: continue
            if not any(z for z in y_slice if z == 2): continue
            for z, z_slice in enumerate(y_slice):#Individual positions
                if z in [0, len(y_slice)-1]: continue
                if z_slice != 2: continue
                for offset in off_by_one:
                    if space[x + offset[0]][y + offset[1]][z + offset[2]] == 0:
                        space[x][y][z] = 0
                        cropped = True
                        amount+= 1
                        break
    if amount :print("Cropped:"+ str(amount))
    amount = 0
                    
for icube in cubes:
    icube.scan_space(space)
    
sum_exposed_space = 0
exposed_counter = [0]*7
for icube in cubes:
    exposed_counter[icube.covered_space] += 1
    if icube.covered_space < 6:
        sum_exposed_space += 6 - icube.covered_space

print("Part2:"+str(sum_exposed_space))
print("Part2 Distribution:"+str(exposed_counter))
#2237 Too high
#2186 Too high
