import copy # Allows deepcopies of List (not just a copy of the Pointer but of the actual data)
import math #Flooring
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
    #t = Type of display
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
    while peak + 8 > len(chamber):#There are less than (7 + 1(due to index)) free rows above the current recorded Chamber
        #print("ceiling up")
        chamber.append([0]*7)
        
floors = list()
floorsloop = list()       
#input("Startwhile?")
def main(blocklimit):
    global chamber
    global floors
    placed_blocks = 0
    floor_blocks = 0
    jet_index = 0
    spawncycle = 0
    loop_rep_height = 0 #Height of all repetitions combined
    floors = list()
    resetfield()
    floorsets = set()
    while(placed_blocks != blocklimit ):#2022
        if placed_blocks != 0 and (placed_blocks/(blocklimit-blocklimit%10)) in [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]:
            print("Main at "+str((placed_blocks/(blocklimit-blocklimit%10))*100)+"%")
        spawncycle %= 5
        spawnblock(spawncycle)
        spawncycle += 1
        jet_index %= len(text)
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
        floor_blocks += 1
        #continue
        #Floor Detection:
        for y in range(max(len(chamber)-49,1),len(chamber)-1):#Searches the last 50 Layers of all if len(chamber) is less than ~50
            if chamber[y] == chamber[0]:
                print("Found Floor " + str(y) + " at Towerheight of "+ str(len(chamber) - 8))
                floors.append([y,placed_blocks,floor_blocks,copy.deepcopy(chamber[0:y-1])])
                # Floor saves in the format
                #[Index]
                #[  Y_of_found floor,
                #   placed blocks,
                #   delta of placed_blocks,
                #   [Copy of Enveloped segment]
                #   ]
                carryover = chamber[y:]
                if len(floors) >= 10 and False: #Debugging
                    print("Carryover:")
                    nulll = [print(''.join(['.' if item == 0 else '#' for item in y])) for y in reversed(carryover)]
                    print("-------")
                floor_blocks = 0
                resetfield()
                for y in enumerate(carryover):
                    if y[1] != [0]*7 :#If the Row is not all 0's
                        if y[0] >= len(chamber):
                            raise_ceiling()    
                        chamber[y[0]]= y[1]
                break
        floorsets = set((item[0], item[2]) for item in floors) #Thx Chatgpt
        if len(floorsets) != len(floors) and len(floorsloop) == 0 :
            loop_extract(list(f[0] for f in floors).index(113)) #Parameter handed over is the start of the loop
            remainging_blocks = blocklimit-placed_blocks
            if remainging_blocks > rep_blocks:
                print("It looks like you are trying to build a very tall tower, skipping:")
                skipped_cycels = math.floor(remainging_blocks/rep_blocks) -2 #-1 To allow for the current cycle to run its course   
                loop_rep_height = skipped_cycels * sum_rep_height
                placed_blocks += skipped_cycels * rep_blocks
                print("Skipped " +str(skipped_cycels)+ " Cycels")
                print("Now only " +str(blocklimit-placed_blocks)+" blocks remaining")
                #breakpoint()
        raise_ceiling()
    if len(floors) == 0:
        sum_height = len(chamber) - 8
        print("Final Height of the "+str(blocklimit)+"Block Tower is "+ str(len(chamber) - 8) + " + ")# Subtracting the 7 rows above and the Floor
    elif loop_rep_height == 0:
        sum_height = sum(floor[0] for floor in floors) + len(chamber) - 8
        print("Final Height of the "+str(blocklimit)+" Block Tower is "+ str(sum_height) + " in "+ str(len(floors)) +" (+1) Segments")
    else:
        sum_height = sum(floor[0] for floor in floors) + len(chamber) - 8
        print("Final Height of the "+str(blocklimit)+" Block Tower is "+ str(sum_height + loop_rep_height) + " with "+ str(skipped_cycels*rep_blocks) +" blocks being placed virtually ")
    return(sum_height + loop_rep_height)

#For 4k+ blocks we get the Following heights:
#146 38 24 223 103 	113 55 102 77 153 72 674 94 59 174 366 293 250 152 
#			113 55 102 77 153 72 674 94 59 174 366 293 250 152 
#The repeating sequenece has a Sum of 2634 Y height and a Volume of 1695 Blocks

#In Typical dev fashion (xkcd/1319/ ) I wish to automise this extraction ( and take ages due to stuff being challengenging and riddeld with hunts for errors)
floorsloop = list()
rep_blocks = 0
sum_rep_height = 0
def loop_extract(start_index):
    global floorsloop
    global rep_blocks
    global sum_rep_height
    preloopfloors = floors.index(floors[-1])
    for floor in floors[start_index:]:
        if not any(map(lambda seg: seg[0:1] == floor[0:1], floorsloop)): #Works only if no 2 Segments have the same height
            floorsloop.append(floor[:3])
    sum_rep_height = sum(floor[0] for floor in floorsloop) 
    rep_blocks = sum(floor[2] for floor in floorsloop)
    print("Temp Loopextraction stats:")
    print("Each Rep is adding "+ str(sum_rep_height) +" heigh due to "+ str(rep_blocks)+" blocks", end="\n\n")

main(1000000000000)

#1293078056346 Too low
#1553982304035 Too high
#1553982301423 Too high
#1553982294510 Wrong
#1553982300884
