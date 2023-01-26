import copy
import math
textfile = open("8-1.txt","r")
text = textfile.read()
destructivetext = copy.deepcopy(text) #We will eat this copy as to not harm the original
readpos = 0
# In the Array each outer container is a Row so it will be [Row][Collum / Position]
Forest = [[int(0) for c in range(99)] for r in range(99)] # 9801 Tres
Scenicscore = [[int(1) for c in range(99)] for r in range(99)] # 1 is visible | 0 is Obscured

r = 0 # Row     ----
c = 0 # Column  ||||
for x in text:

    if (False == x.isdigit()): # End of line
        r=r+1
        c=0
    else:
        Forest [r][c] = int(x)
        c = c+1

Scenicscores = list()
#for x in range(7769,7770):# 0 To 9800 (99^2)
for x in range(9801):
    r = math.floor(x/99) 
    c = x-(99*math.floor(x/99))
    maxheight = Forest[r][c]
    #print("\n"+str(r) + "-"+str(c))
    
    #Looking Up
    if(not(x <= 98)):#Is not Top Row
        scenic = 0
        for d in range(r-1,-1,-1): #Range to reach the Top index 0
            #print(d,end=" ")
            if(Forest[d][c] < maxheight): # Tree seen
                #print("\n" + str(Forest[r-d][c]) +"<"+str(maxheight))
                scenic = scenic+1
            else:
                scenic = scenic+1
                break
        if(scenic == 0):
            scenic = 1
        #print("Up:"+str(scenic))
        Scenicscore [r][c] = Scenicscore [r][c] * scenic
        
    #Looking Left    
    if(not((x+1)%99 == 1)):#Is not Left Row
        scenic = 0
        for d in range(c-1,-1,-1): #Range to reach the Left index 0
            #print(d,end=" ")
            if(Forest[r][d] < maxheight): # Tree seen
                 scenic = scenic+1
            else:
                scenic = scenic+1
                break
        if(scenic == 0):
            scenic = 1
        #print("Left:"+str(scenic))
        Scenicscore [r][c] = Scenicscore [r][c] * scenic
        
    #Looking Right      
    if(not((x+1)%99 == 0)):#Is not Right Row
        scenic = 0
        for d in range(c+1,99,1): #Range to reach the Right index 98
            #print(d,end=" ")
            if(Forest[r][d] < maxheight): # Tree seen
                scenic = scenic+1
            else:
                scenic = scenic+1
                break
        if(scenic == 0):
            scenic = 1
        #print("Right:"+str(scenic))
        Scenicscore [r][c] = Scenicscore [r][c] * scenic
        
    if(not(x > 9701)):#Is not Bottom Row
        maxheight = Forest[r][c]
        scenic = 0
        for d in range(r+1,99,1): #Range to reach the Bottom index 98
            if(Forest[d][c] < maxheight): # Tree seen
                 scenic = scenic+1
            else:
                scenic = scenic+1
                break
        if(scenic == 0):
            scenic = 1
        Scenicscore [r][c] = Scenicscore [r][c] * scenic
      
    if(Scenicscore [r][c] >= 335500):
        print("Pos " +str(r)+"-"+str(c))
        print(Forest[r][c])
        print(Scenicscore [r][c])
    
maxscenic = 1
for x in Scenicscore:
    for Tree in x:
        if(Tree >= maxscenic):
            maxscenic = Tree
        #print(Tree,end="")
    #print("\n")

#6720 too low
#253800 too low
#296100 Wrong
print(maxscenic)
