textfile = open("14-1.txt","r")
text = textfile.readlines()

# X ranges of lines from 440 to 510
# All X values will be subtractet by the reduction
reduction = 240
# Y ranges from 10 to 170

#In order to accomodate big Piles we make the scan much wider (+200) and redice the reduciton by a bit

scan = [[False for y in range(170)] for x in range(710-reduction)]
ls = [[]]

#Converting input to a usable list:
for x in text:
    #print(x)
    pairs = x.rstrip().split(" -> ") # Pair of X and Y coordinates
    for i in pairs:
        pair = i.split(",")
        pair[0] = int(pair[0])
        pair[1] = int(pair[1])
        ls[-1].append(pair)
        #print(pair)
    ls.append([])
    #print("\n")

      
#print(ls)

for segment in ls:
    for line in range(len(segment)):
        if(segment[line] == segment[-1]):
            scan[segment[line][0]-reduction][segment[line][1]] = True
            continue #Skips the last Coordinate as it doesn't lead anywhere

        start = [segment[line][0]-reduction,segment[line][1]]
        end = [segment[line +1][0]-reduction,segment[line +1][1]]
        #print(start)
        #print(end)
        if(start[0] != end[0] and start[1] != end[1]):
            input("Diagonall Line Detectet")
        if(start[0] < end[0]): # X line ---->
            for offset in range(end[0]-start[0]):
                #print(start[0]+offset)
                scan[start[0]+offset][start[1]] = True
        if(start[0] > end[0]): # X line <----
            for offset in range(start[0]-end[0]):
                scan[start[0]-offset][start[1]] = True
        if(start[1] < end[1]): # Y line ||V||
            for offset in range(end[1]-start[1]):
                scan[start[0]][start[1]+offset] = True
        if(start[1] > end[1]): # Y line ||A||
            for offset in range(start[1]-end[1]):
                scan[start[0]][start[1]-offset] = True
print("Pure wall:")
print(scan)

#Searching for the lowest point greedily to place a floor:
floor = 0
for x in range(len(scan)):
    for y in range(len(scan[0])):
     if scan[x][y] and y > floor :
         floor = y
         print("Lowering floor to "+str(floor))
floor += 2

for x in range(len(scan)):
    scan[x][floor] = True

print("Floor is " + str(floor))

sandoob = False # Whether a grain has fallen out of bounds
placedgrains = 0
while(not(sandoob)):
    grainpos = [500-reduction,0]
    restingpos = False
    while(not(restingpos)):
        if(scan[500-reduction][0]):#Grain would Diplace one at Point of Origin
          break
        if not(scan[grainpos[0]][grainpos[1]+1]):#Down 1
            grainpos[1] += 1
            #print("d")
        elif not(scan[grainpos[0]-1][grainpos[1]+1]): #Down 1 Left 1
            grainpos[0] -= 1 
            grainpos[1] += 1
            #print("l")
        elif not(scan[grainpos[0]+1][grainpos[1]+1]): #Down 1 Right 1
            grainpos[0] += 1 
            grainpos[1] += 1
            #print("r")
        else:#Grain has reached a final position
            restingpos = True
            
    if restingpos:
        placedgrains += 1
        scan[grainpos[0]][grainpos[1]] = True  
    else:
        sandoob = True

print(placedgrains)
