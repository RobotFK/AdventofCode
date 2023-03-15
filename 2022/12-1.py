import math
import sys
import copy
textfile = open("12-1.txt","r")
text = textfile.readlines()

#Dont do this by hand kids, automate it to remove the chance for human error -_- !
#start = tuple([0,20,0])
#end = tuple([133,20,sys.maxsize])

height = [[-1 for r in range(41)] for c in range(154)]
#Height is saved as a Number between 0 and 25
#Positions are therefore Height[x-pos][y-pos] with the topleft being 0,0

dist_to_start = [[999 for r in range(41)] for c in range(154)] # Using 999 instead of sys.maxsize
dist_to_start[start[0]][start[1]] = 0 #Starting spot

max_x = 153
max_y = 40

#Preparing to Implement the D* algorithm
openset = set() # Set of all Points on the border of our known space
closedset = set() # Points with a known distance to the start and surrounding unexamined points
openset.add(start)

for c in range(154):
    for r in range(41):
        if (text[r][c] == 'S'):
            height[c][r] = 0
            start = tuple([c,r,0])
        elif (text[r][c] == 'E'):
            height[c][r] = 25
            end = tuple([c,r,sys.maxsize]) #This was what caused issues
        else:
            height[c][r] = ord(text[r][c])-97

def testneighbor(pos,pos_height,x_off,y_off):# Checks if a connection to the given offset could and shoud be created
    npos = tuple([pos[0]+x_off,pos[1]+y_off,pos[2]+1])
    if (npos[0] <= max_x): # Valid index
        if (height[npos[0]][npos[1]] <= pos_height+1 and #Is valid height
            #height[npos[0]][npos[1]] >= pos_height and #Disables descending
            not npos in openset):#Is not already in openset
            for x in closedset: #Needed as npos cant be sure about its distance to start yet :/
                if(x[0] == npos[0] and x[1] == npos[1]):
                    return False
            return True
                
def openneighbors (pos,pos_height): #Pos contains x-cord,y-cord,distace_to_start
    #print(pos)
    #Right
    if (pos[0]+1 <= max_x):
        if(testneighbor(pos,pos_height,1,0)):
            openset.add(tuple([pos[0]+1,pos[1],pos[2]+1]))
    #Left
    if (pos[0]-1 >= 0):
        if(testneighbor(pos,pos_height,-1,0)):
            openset.add(tuple([pos[0]-1,pos[1],pos[2]+1]))
    #Down
    if (pos[1]+1 <= max_y):
        if(testneighbor(pos,pos_height,0,1)):
            openset.add(tuple([pos[0],pos[1]+1,pos[2]+1]))
    #Up
    if (pos[1]-1 >= 0):
        if(testneighbor(pos,pos_height,0,-1)):
            openset.add(tuple([pos[0],pos[1]-1,pos[2]+1]))
            
end_in_closedset = False
while (not end_in_closedset):
    #copy openset to c_openset
    #This is needed as to not append to a set we are iterating through
    c_openset = copy.deepcopy(openset)
    openset.clear()
    #expand each of c_openset
    for x in c_openset:
        openneighbors(x,height[x[0]][x[1]])
        #move each of c_openset to closedset
        closedset.add(x)
    #print("Currently "+ str(len(openset))+ " Pos in Openset")
    #Check if we exhausted all options or if we have reached the end
    if (not len(openset)):
        print("Error, Openset empty.\nAborting")
        break
    for x in closedset:
        if(x[0] == end[0] and x[1] == end[1]):
            end_in_closedset = True

for x in closedset:
    dist_to_start[x[0]][x[1]] = x[2]

print(dist_to_start)

#Carves of Points that are not relevant to the optimal path, as they lead nowhere
pathcomplete = False
carved = 0
while (not pathcomplete):
    print("Carvingprogress:"+ str(carved))
    if(carved >= 8000):
        break #backup killswitch       
    pathcomplete = True
    c_closedset = copy.deepcopy(closedset)
    for x in c_closedset:
        higher = False
        lower = False
        if (x[0] == end[0] and x[1] == end[1]):
            #print("Found End")
            continue
        if (x[0] == start[0] and x[1] == start[1]):
            #print("Found start")
            continue
        #print(x)
        #Right
        if (x[0]+1 <= max_x): # Inside bounds
            if(dist_to_start[x[0]+1][x[1]]== x[2]+1):
                if(height[x[0]+1][x[1]] <= height[x[0]][x[1]]+1):
                    higher = True
                    #print("r h")
        #Left
        if (x[0]-1 >= 0): # Inside bounds
            if(dist_to_start[x[0]-1][x[1]]== x[2]+1):
                if(height[x[0]-1][x[1]] <= height[x[0]][x[1]]+1):
                    higher = True
                    #print("l h")
        #Down
        if (x[1]+1 <= max_y): # Inside bounds
            if(dist_to_start[x[0]][x[1]+1]== x[2]+1):
                if(height[x[0]][x[1]+1] <= height[x[0]][x[1]]+1):
                    higher = True
                    #print("d h")
        #Up
        if (x[1]-1 >= 0): # Inside bounds
            if(dist_to_start[x[0]][x[1]-1]== x[2]+1):
                if(height[x[0]][x[1]-1] <= height[x[0]][x[1]]+1):
                    higher = True
                    #print("u h")
        
        if(not(higher)):
            #print("carving")
            dist_to_start[x[0]][x[1]] = 998 # As Punishment for being optional we ignore this node 
            closedset.remove(x)
            carved +=1
        
    if (not len(closedset)):
        print("Error, Closedset empty.\nAborting")
        break
    if(closedset != c_closedset):#True if we carved something
        pathcomplete = False
    #break
print("Carved:"+ str(carved))
print(dist_to_start)
print(dist_to_start[end[0]][end[1]])
#473 Too high
