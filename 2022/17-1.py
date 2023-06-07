with open("17-1.txt","r") as file:
    text = file.readline()
    
#To Atomise this task we treat each block as a group of Single Rocks that act on their own and only communication if one comes to a stop
#Movements will have to be validated First by checking for collisions

#The Field Will be a 2D array(list x list) that will be expanding as it needs to be
#The Field will always have a height of 3 + 4 heigher than the highest Set rock
#Chamber[Y][X]
# Fieldvalue -> Meaning
# 0 = Air
# 1 = Set Rock
# 2 = Falling Rock

#Functions return true if they live up to their name ,but only if they could fail
def resetfield():
    global chamber
    chamber = [[0 for y in range(7)] for x in range(8)]
    for x in range(len(chamber[0])):#Adding a Floor at y=0
        chamber[0][x] = 1


def print_chamber(t = 0):
    #Types
    global chamber
    #On Default the Y axis is Flipped
    if t == 1:#Inverted
        for y in reversed(chamber):
            print(y)
    if t == 11:#Inverted  and High Vis
        nulll = [print(''.join(['.' if item == 0 else '#' for item in y])) for y in reversed(chamber)]
    else:
        for y in chamber:
            print(y)
    
def spawnblock(rocktype):
    global chamber
    if rocktype == 0: 
        chamber[-4][2:6]= [2] * 4 # @@@@
    elif rocktype == 1: 
        chamber[-2][3]  =  2      # .@.
        chamber[-3][2:5]= [2] * 3 # @@@
        chamber[-4][3]  =  2      # .@.
    elif rocktype == 2: 
        chamber[-2][4]  =  2      # ..@
        chamber[-3][4]  =  2      # ..@
        chamber[-4][2:5]= [2] * 3 # @@@
    elif rocktype == 3:
        chamber[-1][2]  =  2      # @
        chamber[-2][2]  =  2      # @
        chamber[-3][2]  =  2      # @
        chamber[-4][2]  =  2      # @
    elif rocktype == 4: 
        chamber[-3][2:4]= [2] * 2 # @@
        chamber[-4][2:4]= [2] * 2 # @@
    #print_chamber()
    return
        
def jet(direction):# Movement is either +1 or -1
    global chamber
    collision = False
    if direction == '>':#Movin Right
        for y in chamber:
            if 2 in y: #Line with a fallin Rock
                #print(y)
                collision = next((True for x in range(len(y)-1) if y[x] == 2 and y[x+1] == 1),False) # Returns True if something mathces the restrictions
                if y[-1] == 2:
                    collision = True
                if collision:
                    break
    if direction == '<':#Movin Left
        for y in chamber:
            if 2 in y: #Line with a fallin Rock
                collision = next((True for x in range(1,len(y)) if y[x] == 2 and y[x-1] == 1),False) # Returns True if something mathces the restrictions
                if y[0] == 2:
                    collision = True
                if collision:
                    break
    if collision:
        #print("Collision Detected")
        return False
    if direction == '>':
        chamber = [[3 if x > 0 and chamber[y][x-1] == 2 else chamber[y][x] for x in range(7)] for y in range(len(chamber))]                   
    if direction == '<':
        chamber = [[3 if x < 6 and chamber[y][x+1] == 2 else chamber[y][x] for x in range(7)] for y in range(len(chamber))]
    chamber = [[0 if pos == 2 else pos for pos in y] for y in chamber]
    chamber = [[2 if pos == 3 else pos for pos in y] for y in chamber]
    return True
        
def fall():
    global chamber
    collision = False
    #input("Falling")
    for y in range(len(chamber)):
        if 2 in chamber[y]: #Line with a fallin Rock (Will never by y=0)
            collision = bool(next((x+1 for x in range(7) if chamber[y][x] == 2 and chamber[y-1][x] == 1), False)) # x+1 to stop the x = 0 being evaluated as False
            if collision: # If at any point a Stone is hindered all falling rocks are stopped
                break
    if collision:# Transform the Falling Rocks into Set Rocks
        #print("Setting Stones")
        chamber = [[1 if pos == 2 else pos for pos in y] for y in chamber]
        return False
    else:#Translate Falling Rocks one Down (We copy in 3 steps)
        #chamber = [[3 if chamber[y+1][x] == 2 else chamber[y][x] for x in range(7)] for y in range(len(chamber))]
        chamber = [[3 if y < len(chamber)-1 and chamber[y+1][x] == 2 else chamber[y][x] for x in range(7)] for y in range(len(chamber))]
        chamber = [[0 if pos == 2 else pos for pos in y] for y in chamber]
        chamber = [[2 if pos == 3 else pos for pos in y] for y in chamber]
    #print_chamber(1)
    return True

def raise_ceiling():
    global chamber
    peak = max([i[0] for i in enumerate(chamber) if 1 in i[1]]) # Thight and fast
    #print(peak)
    while peak + 8 > len(chamber):#There are less than (7 + Floor) free rows above the current recorded Chamber
        #print("ceiling up")
        chamber.append([0]*7)
        
    
placed_blocks = 0
jet_index = 0
spawncycle = 0
resetfield()
input("Startwhile?")
while(placed_blocks != 2022):#2022
    if placed_blocks % 100 == 0:
        print("Placedblocks: "+str(placed_blocks))
    #input("Outer")
    spawncycle %= 5
    spawnblock(spawncycle)
    spawncycle += 1
    jet_index %= len(text)
    #print(text[jet_index])
    jet(text[jet_index])
    jet_index += 1
    while (fall()):
        #print_chamber(11)
        #input("^Postfall^")
        jet_index %= len(text)
        #print("Jet:"+str(text[jet_index]))
        jet(text[jet_index])
        jet_index += 1
        #print_chamber(11)
        #print("^Post Jet: "+str(jet_index))
    raise_ceiling()
    placed_blocks += 1
    #print_chamber(1)
    #if placed_blocks >= 20:
    #    print(str(placed_blocks) +" - "+ str(len(chamber) - 8))
        
#print_chamber(1)
print("Final Height of the Tower is "+ str(len(chamber) - 8)) # Subtracting the 7 rows above and the Floor

#2514 too low
