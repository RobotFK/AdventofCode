import time
with open("23-1.txt","r") as file:
    text = file.readlines()
#While 23-2 is possible with minimal Changes, I dont have the Pactience to wait, and will instead just slightly optimise
#Calc may take a minute
'''
The Board is:
0---> +x
|
|
V
+y
'''
#Static 8 positions around (x,y)
adjacent = [[-1,-1], [ 0,-1],[ 1,-1], 
            [-1, 1], [ 0, 1],[ 1, 1],
            [-1, 0],[ 1, 0]]
# N S W E at the start , rotates 1 each step (x,y)
directions= [[[-1,-1], [ 0,-1],[ 1,-1]], 
            [[-1, 1], [ 0, 1],[ 1, 1]],
            [[-1,-1], [-1, 0],[-1, 1]],
            [[ 1,-1], [ 1, 0],[ 1, 1]]]
#Yeah, this list essentailly means that we are converting from list to objects and back to list
#Still this seems like a good crutch/upgreade
board= list()
min_x= int()
min_y= int()

def rotate_direction():
    global directions
    temp = directions[0]
    directions.pop(0)
    directions.append(temp)
    return True

def umap(arg = 0): #Update Map (and print it)
        global board
        global min_x
        global min_y
        #Added a buffer for to stop the looking of elves to be confused by being out of bounds
        min_x = min(i_e.pos[0] for i_e in elves)-1
        max_x = max(i_e.pos[0] for i_e in elves)+1
        min_y = min(i_e.pos[1] for i_e in elves)-1
        max_y = max(i_e.pos[1] for i_e in elves)+1
        board = [ ['.']*(1+max_x-min_x) for i in range(1+max_y-min_y)]#+1 on both axis to account for the 0index that isn't counted in the difference
        for e in elves:
            board[e.pos[1]-min_y][e.pos[0]-min_x] = '#'
        if arg == 1:
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
        global board
        self.n_pos = (None,None)
        alone = True
        #The Double loop is recommended by ChatGPT and speeds up this function by about 400%
        #print(self.pos)
        
        if board[self.pos[1]-min_y][self.pos[0]-min_x]!= '#':breakpoint()
        if not any(
            (board[self.pos[1] + adj[1]-min_y][self.pos[0] + adj[0]-min_x]) == '#'
            for adj in adjacent if (self.pos[1] + adj[1]-min_y) >= 0 and (self.pos[1] + adj[1]-min_y) <= len(board)-1 and (self.pos[0] + adj[0]-min_x) >= 0 and (self.pos[0] + adj[0]-min_x) <= len(board[0])-1
        ):
            return False
        #print("Elf is not Alone")
        # Each 1/4 of the compass is a "direction" split up into 3 "segments" to look around 
        for direction in directions: #Looks a bit strange but allows me to ignore the roation of the searchpatern
            for segment in direction:
                #print("Is somebody at"+str(segment))
                #Checks if there is an elf in the segment, skips the direction if that is the case
                if board[self.pos[1]-min_y+segment[1]][self.pos[0]-min_x+segment[0]] == '#':
                    self.n_pos = (None,None)
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

if input("Map?(y/n)") == 'y':umap(1)
start = time.process_time()
print("Looking:")
move_counter = 1
mround = 0
umap()
while move_counter != 0:
    '''
    This Loop contains the following segments
    -Sort elveslist to slightly improve efficency
    -Each Elf looks for a spot he wants to go to
    -Each Elf attempts to move
    -Directions are rotated once
    '''
    s = time.process_time()
    #if mround == 30:break #Manuall Stop for debugging purposes
    mround+=1
    
    elves.sort(key=lambda x: x.pos[0])
    slook = time.process_time()
    for e in elves:
        e.look()
    elook = time.process_time()-slook
    move_counter = 0
    for e in elves:
        if e.move():move_counter+=1
    umap()
    
    rotate_direction()
    print("R: "+str(mround),end=" | ")
    print(str(move_counter)+"/"+str(len(elves))+" moved",end=" | ")
    print("R.Time: "+str(time.process_time() - s),end="\n\n")
    
end = time.process_time()
if input("Map?(y/n)") == 'y':umap(1)
print("Total Runtime: "+str(end - start))
print("Reached Round:"+str(mround))
