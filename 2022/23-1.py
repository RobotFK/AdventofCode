import time
with open("23-1.txt","r") as file:
    text = file.readlines()
'''
The Board is:
0---> +x
|
|
V
+y
'''
#Static 8 positions around
adjacent = [[-1,-1], [ 0,-1],[ 1,-1], 
            [-1, 1], [ 0, 1],[ 1, 1],
            [-1, 0],[ 1, 0]]
# N S W E at the start , rotates 1 each step
directions= [[[-1,-1], [ 0,-1],[ 1,-1]], 
            [[-1, 1], [ 0, 1],[ 1, 1]],
            [[-1,-1], [-1, 0],[-1, 1]],
            [[ 1,-1], [ 1, 0],[ 1, 1]]]

def rotate_direction():
    global directions
    temp = directions[0]
    directions.pop(0)
    directions.append(temp)
    return True

def pmap(): #Print Map
        min_x = min(i_e.pos[0] for i_e in elves)
        max_x = max(i_e.pos[0] for i_e in elves)
        min_y = min(i_e.pos[1] for i_e in elves)
        max_y = max(i_e.pos[1] for i_e in elves)
        board = [ ['.']*(1+max_x-min_x) for i in range(1+max_y-min_y)]#+1 on both axis to account for the 0index that isn't counted in the difference
        for e in elves:
            board[e.pos[1]-min_y][e.pos[0]-min_x] = '#'
        for r in board:
            r = "".join(r)
            print(r)
    
class Elf:
    def __init__ (self, pos_x,pos_y):
         self.pos = (pos_x,pos_y)
         self.n_pos = (None,None) #New or Next
         
    def __str__(self):
            return "Elf:" +str(self.pos[0])+"-"+str(self.pos[1])+" -> "+str(self.n_pos[0])+"-"+str(self.n_pos[1])+" "
        
    def __hash__(self):
        return hash(self.pos)
    
    def look (self):#Returns whether we have found a place the Elf wants to move to
        global adjacent
        self.n_pos = (None,None)
        alone = True
        #The Double loop is recommended by ChatGPT and speeds up this function by about 400%
        if not any(
            (self.pos[0] + adj[0], self.pos[1] + adj[1]) == elve.pos
            for adj in adjacent
            for elve in elves
        ):return False
        #print("Elf is not Alone")
        # Each 1/4 of the compass is a "direction" split up into 3 "segments" to look around 
        for direction in directions: #Looks a bit strange but allows me to ignore the roation of the searchpatern
            for segment in direction:
                segment = (self.pos[0] + segment[0], self.pos[1] + segment[1])
                #print("Is somebody at"+str(segment))
                #Checks if there is an elf in the segment, skips the direction if that is the case
                if segment in [s.pos for s in elves]: #Not in is around 0,1% faster than if not [...] in
                    self.n_pos = (None,None)
                    #print("Cant move direction "+str(direction[1]))
                    break
                self.n_pos = (self.pos[0]+direction[1][0],self.pos[1]+direction[1][1])
            #We can only leave the segment loop if none of the segments contained an Elf
            if self.n_pos != (None,None):
                return True  
        #print("No Free Direction")
        return False
    
    def move(self):
        if self.n_pos == (None,None):return False
        #print("Want to go")
        bump = False
        for elve in elves:
            if elve == self:
                continue
            if elve.n_pos == self.n_pos:
                #print("Collision with"+str(elve.pos))
                elve.n_pos == (None,None)
                bump = True
        if bump == False:self.pos = self.n_pos
        return not(bump)

elves = list()
for y,line in enumerate(text):
    for x,value in enumerate(line):
        if value == '#':elves.append(Elf(x,y))

if input("Map?(y/n)") == 'y':pmap()
start = time.process_time()
print("Looking:")
for mround in range(10):
    '''
    This Loop contains the following segments
    -Sort elveslist to slightly improve efficency
    -Each Elf looks for a spot he wants to go to
    -Each Elf attempts to move
    -Directions are rotated once
    '''
    elves.sort(key=lambda x: x.pos[0])
    print("Round "+str(mround+1))
    for e in elves:
        e.look()
    move_counter = 0
    for e in elves:
        if e.move():move_counter+=1
    print(str(move_counter)+"/"+str(len(elves))+"moved")
    rotate_direction()
    print("Runtime: "+str(time.process_time() - start),end="\n\n")
    
end = time.process_time()
if input("Map?(y/n)") == 'y':pmap()
print("Total Runtime: "+str(end - start))
min_x = min(i_e.pos[0] for i_e in elves)
max_x = max(i_e.pos[0] for i_e in elves)
min_y = min(i_e.pos[1] for i_e in elves)
max_y = max(i_e.pos[1] for i_e in elves)
print("Squre is "+str(1+max_x-min_x)+"x"+str(1+max_y-min_y)+" with "+ str(len(elves))+" Elves")
print("Answer:"+str(((1+max_x-min_x)*(1+max_y-min_y))-len(elves)))
